//	Filename:  Test_Functions.c
//	Author:  Paul Wittibschlager
//	Date Created:  08/27/2018
//	Company:  Valtronic, USA
//	Project:   Lumitex OraGlo Control Board
//	Project number:  DD400004
//	Description:  This file contains functions to support power on self test (POST),
//                built in self test (BIST) and the test interface (TI)

#include "mcc_generated_files/mcc.h"
#include "Test_Functions.h"
#include "Support_Functions.h"
#include "UI_Functions.h"
#include "flashmem.h"
#include "pushbuttons.h"
#include "ledboard.h"
#include "analog.h"
#include "tachometer.h"
#include "hw_watchdog.h"
#include "lcd.h"
#include "rgbled.h"
#include "eeprom.h"
#include "mca.h"
#include "rtcc.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#define    FCY    16000000UL  // required by libpic30.h    
#include <libpic30.h>

#define FW_NAME "MuReva Control Board Software"
#define FW_PART_NUMBER "Valtronic P/N DD400004-SW1"
#define FW_VERSION "Version 2.1.2"     // Format: Major.Minor.Patch


//Hardware values that are needed 
#define RES_24V_PULLUP 25500.0f
#define RES_24V_PULLDOWN 1470.0f
#define RES_5V_PULLUP 1470.0f
#define RES_5V_PULLDOWN 735.0f
#define RES_33V_PULLUP 1470.0f
#define RES_33V_PULLDOWN 1470.0f
#define CONTROL_BOARD_MCP9808_ADDRESS 0x00
#define MP_BOARD_24LC02_ADDRESS 0x00


//Test Limits from DD400004-SDS1
#define SW_SPEC_REVA_7_6_1 0.25f
#define SW_SPEC_REVA_7_6_2 0.15
#define SW_SPEC_REVA_7_6_3 0.15
#define SW_SPEC_REVA_7_6_9 75.0f

// Create these constants just in case the compiler isn't sophisticated
// enough to combine multiple instances of the same string literal
static const char *PASS = "Pass"; 
static const char *FAIL = "Fail"; 


static bool Test24VDC(float *voltage, char *message);
static bool Test33VDC(float *voltage, char *message);
static bool TestAlarm(char *message);
static bool TestControlBoardTemp(float *temperature, char *message, bool *timeout);
static void DisplayTableOfImages(void);
static void SetAlarm(void);
static void SetUART1Baud(void);
static void TestUART1(void);
static void TestSWWdog(void);

static void Print_to_LCD(void);

static void ImageFlashSubmenu(void);

static void ReadFWVersion(char *data);

static uint16_t PerformCommonTests(char *message, bool retest, bool print_out);

static void EEPROMSubmenu(void);

static void RTCCSubmenu(void);

// Declaration made here only so that we can avoid adding it to the
// automatically generated files
extern void SPI2_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData );


/*
 * Description:  This function measures the 24VDC input power supply, applies limits and determines pass/fail
 * Preconditions:  The ADC acquisition system has been setup and running, providing the averaged, analog input values
 * Arguments:  measured analog value is returned in the Value field
 *             The low and high limits are returned in the message parameter
 *             'message' should allow for up to 255 characters
 * Returns:  false = Fail, true = Pass
 */
static bool Test24VDC(float *voltage, char *message)
{
    *voltage = ((RES_24V_PULLDOWN+RES_24V_PULLUP)/RES_24V_PULLDOWN) * GetADCReadingCh10();  //calculate the 24V value
    const float Limit_High = 24.0f*(1.0f+SW_SPEC_REVA_7_6_1);
    const float Limit_Low = 24.0f*(1.0f-SW_SPEC_REVA_7_6_1);

    if (message != NULL)
    {
        sprintf(message, "Limit_Low = %.3f  Limit_High = %.3f", 
                (double)Limit_Low, (double)Limit_High);
    }
    
    //Apply the limits and determine pass/fail
    return (*voltage >= Limit_Low) && (*voltage <= Limit_High);
}


/*
 * Description:  This function measures the 5VDC power supply, applies limits and determines pass/fail
 * Preconditions:  The ADC acquisition system has been setup and running, providing the averaged, analog input values
 * Arguments:  measured analog value is returned in the Value field
 *             The low and high limits are returned in the message parameter.
 *             'message' should allow for up to 255 characters
 * Returns:  false = Fail, true = Pass
 */
bool Test5VDC(float *voltage, char *message)
{
    *voltage = ((RES_5V_PULLDOWN+RES_5V_PULLUP)/RES_5V_PULLDOWN) * GetADCReadingCh9();  //calculate the 5V value
    const float Limit_High = 5.0f*(1.0f+SW_SPEC_REVA_7_6_2);
    const float Limit_Low = 5.0f*(1.0f-SW_SPEC_REVA_7_6_2);
    
    if (message != NULL)
    {
        sprintf(message, "Limit_Low = %.3f  Limit_High = %.3f",
                (double)Limit_Low, (double)Limit_High);
    }
 
    //Apply the limits and determine pass/fail
    return (*voltage >= Limit_Low) && (*voltage <= Limit_High);
}


/*
 * Description:  This function measures the 3.3VDC power supply, applies limits and determines pass/fail
 * Preconditions:  The ADC acquisition system has been setup and running, providing the averaged, analog input values
 * Arguments:  measured analog value is returned in the Value field
 *             The low and high limits are returned in the message parameter.
 *             'message' should allow for up to 255 characters
 * Returns:  false = Fail, true = Pass
 */
static bool Test33VDC(float *voltage, char *message)
{
    *voltage = ((RES_33V_PULLDOWN+RES_33V_PULLUP)/RES_33V_PULLDOWN) * GetADCReadingCh8();  //calculate the 3.3V value
    const float Limit_High = 3.3f*(1.0f+SW_SPEC_REVA_7_6_3);
    const float Limit_Low = 3.3f*(1.0f-SW_SPEC_REVA_7_6_3);

    if (message != NULL)
    {
        sprintf(message, "Limit_Low = %.3f  Limit_High = %.3f",
                (double)Limit_Low, (double)Limit_High);
    }
    //Apply the limits and determine pass/fail
    return (*voltage >= Limit_Low) && (*voltage <= Limit_High);
}


/*
 * Description:  This function tests that the alarm is connected and determines pass/fail, alarm will chirp
 * Preconditions:  The digital IO must be configured for the alarm 
 * Arguments:  Message should allow for up to 255 characters, test failure details will be placed here
 * Returns:  false = Fail, true = Pass
 */
static bool TestAlarm(char *message)
{
    if (message != NULL)
    {
        strcpy(message, "Test Alarm ");  //initialize the message string
    }
    ALARM_ON_SetHigh();
    __delay_us(12725); // Turn the alarm on for just a short chirp
    bool alarm_detected_when_on = (ALARM_DETECTED_GetValue() == 1);
    if (!alarm_detected_when_on)
    {
        strcat(message, "Alarm not detected when on.  ");
    }  
    
    ALARM_ON_SetLow();
    __delay_us(12725); 
    bool alarm_detected_when_off = (ALARM_DETECTED_GetValue() == 0);
    if (!alarm_detected_when_off)
    {
        strcat(message, "Alarm not detected when off.  ");
    }  
    
    strcat(message, "Alarm should chirp.  " );
    
    return alarm_detected_when_on && alarm_detected_when_off;
}


/*
 * Description:  This function tests the control board temp sensor and applies limits to the measured temperature
 *               to determine the pass/fail.
 * Preconditions:  The I2C module that communicates to the MCP9808 sensor needs to be initialized
 * Arguments:  The measured temperature is returned in the Value field
 *             Message should allow for up to 255 characters, test failure details will be placed here
 * Returns:  false = Fail, true = Pass
 */
static bool TestControlBoardTemp(float *temperature, char *message, bool *timeout)
{
    uint8_t one_transfer[2] = {0x00, 0x00};   //small packet of just two bytes, used to access the registers of the MCP9808

    if (message != NULL)
    {    
        strcpy(message, "Test Control Board Temp ");  //initialize the message string
    }
    int slave_device_address = 0x30;  //I2C address of the MCP9808 temperature sensor
    slave_device_address |= (CONTROL_BOARD_MCP9808_ADDRESS*2);  //OR in the address
    slave_device_address >>= 1;  //shift it right by one since this is what the I2C1_MasterWrite function requires
    I2C1_MESSAGE_STATUS status; //status of I2C interface
    
    one_transfer[0] = 0x05;  //This is the address of the temperature register
    I2C1_MasterWrite(one_transfer, 1, slave_device_address, &status);  //put out on the I2C bus
    const uint16_t I2C_TIMEOUT_PERIOD = 1000; // in milliseconds
    const uint16_t DELAY = 10;
    *timeout = false;
    uint16_t time = 0;
    while ((status == I2C1_MESSAGE_PENDING) && !(*timeout))
    {
        __delay_ms(DELAY); //Hang up here until message has arrived
        time += DELAY;
        if (time > I2C_TIMEOUT_PERIOD)
        {
            *timeout = true;
        }
    } 
   
    bool pass;
    if (*timeout)
    {
        printf("Control board temperature reading problem\r\n");
        pass = false;
    }
    else
    {
        __delay32(150);  //the system is hanging up here, not able to do the next transfer for some reason, adding this delay solved the problem

        I2C1_MasterRead(one_transfer, 2, slave_device_address, &status);  //read data
        time = 0;
        while ((status == I2C1_MESSAGE_PENDING) && !(*timeout))
        {
            __delay_ms(DELAY); //Hang up here until message has arrived
            time += DELAY;
            if (time > I2C_TIMEOUT_PERIOD)
            {
                *timeout = true;
            }
        } 
        if (*timeout)
        {
            printf("Control board temperature reading problem\r\n");
            pass = false;
        }
        else
        {
            *temperature = (one_transfer[0] & 0x0F) * 256 + one_transfer[1];
            *temperature /= 16;
            if ((one_transfer[0] & 0x10) == 0x10)  //Determine if temperature value is positive or negative
            { 
                *temperature -= 256;
            }  

            pass = (*temperature < SW_SPEC_REVA_7_6_9 );
            if (!pass && (message != NULL))
            {
                sprintf(message, "Expected Temperature < %.1fC", (double)SW_SPEC_REVA_7_6_9);
            }          
        }
    }

    return pass;
}


/*
 *   Executes tests that are common to both POST and BIST
 */
static uint16_t PerformCommonTests(char *message, bool retest, bool print_out)
{
    uint16_t test_result = 0;  // Holds the bitwise pass/fail results for all (1=pass, 0=fail)
    //-------------------------------------------------------------------
    float value;  //Temporary variable to retrieve test voltages/temperatures from individual POST tests
    bool result = Test24VDC(&value, message);  //Run the individual test
    if (!result && retest)
    {
        result = Test24VDC(&value, message);
    }
    if (result)
    {
        test_result |= SUPPLY_24VDC_VALID;
    }
    if (print_out) //print results to the test interface
    {
        printf("\n\r Test_24VDC              Value = %.3f  %s  Message:  %s", 
                (double)value, (result ? PASS : FAIL), message);
    }   
    //-------------------------------------------------------------------
    result = Test5VDC(&value, message);  //Run the individual test
    if (!result && retest)
    {
        result = Test5VDC(&value, message);
    }
    if (result)
    {
        test_result |= SUPPLY_5VDC_VALID; 
    }
    if (print_out) //print results to the test interface
    {
        printf("\n\r Test_5VDC               Value = %.3f   %s  Message:  %s",
                (double)value, (result ? PASS : FAIL), message);
    }   
    //-------------------------------------------------------------------
    result = Test33VDC(&value, message);  //Run the individual test
    if (!result && retest)
    {
        result = Test33VDC(&value, message);
    }
    if (result)
    {
        test_result |= 0x0004; 
    }
    if (print_out) //print results to the test interface
    {
        printf("\n\r Test_33VDC              Value = %.3f   %s  Message:  %s", 
                (double)value, (result ? PASS : FAIL), message);
    }   
    //-------------------------------------------------------------------
    int iValue;  //Temporary variable to retrieve test values that are integer type
    result = TestFan(&iValue, message);  //Run the individual test
    if (!result && retest)
    {
        result = TestFan(&iValue, message);
    }
    if (result)
    {
        test_result |= FAN_WORKS;
    }
    if (print_out)  //print results to the test interface
    {
        printf("\n\r Test_Fan                Value = %d    %s  Message:  %s",
                iValue, (result ? PASS : FAIL), message);
    }  
    //-------------------------------------------------------------------
    bool i2c_timeout = true;
    const uint16_t MAX_NUM_RETRIES = 12;
    uint16_t num_retries = 0;
    while (i2c_timeout && (num_retries++ < MAX_NUM_RETRIES))
    {
        result = TestControlBoardTemp(&value, message, &i2c_timeout);  //Run the individual test
    }
    if (!result && retest)
    {
        i2c_timeout = true;
        num_retries = 0;
        while (i2c_timeout && (num_retries++ < MAX_NUM_RETRIES))
        {
            result = TestControlBoardTemp(&value, message, &i2c_timeout);  //Run the individual test
        }
    }
    if (result)
    {
        test_result |= CTRL_BOARD_TEMP_WORKS; 
    }
    test_result |= I2C_DIDNT_TIMEOUT;
    if (i2c_timeout)
    {
        /* This time we CLEAR the bitflag. That's because multiple tests
         * use this flag, any a failure on any single one should set
         * this flag to zero. */
        test_result &= ~I2C_DIDNT_TIMEOUT; 
    }
    if (print_out) //print results to the test interface
    {
        printf("\n\r Test_Control_Board_Temp Value = %.3f  %s  Message:  %s",
                (double)value, (result ? PASS : FAIL), message);
        /* Only print out I2C status if an I2C failure occurs.  This is a 
           low-level detail that's only of interest for diagnostic purposes */
        if (!result && i2c_timeout)
        {
            printf("\n\r   I2C failure occurred in control board temperature measurement");
        }
    }  
    //-------------------------------------------------------------------
    i2c_timeout = true;
    num_retries = 0;
    while (i2c_timeout && (num_retries++ < MAX_NUM_RETRIES))
    {
        result = TestLEDBoardTemp(&value, message, &i2c_timeout);  //Run the individual test
    }
    if (!result && retest)
    {
        i2c_timeout = true;
        num_retries = 0;
        while (i2c_timeout && (num_retries++ < MAX_NUM_RETRIES))
        {
            result = TestLEDBoardTemp(&value, message, &i2c_timeout);  //Run the individual test
        }     
    }
    if (result)
    {
        test_result |= LED_BOARD_TEMP_WORKS; 
    }
    if (i2c_timeout)
    {
        /* This time we CLEAR the bitflag. That's because multiple tests
         * use this flag, any a failure on any single one should set
         * this flag to zero. */
        test_result &= ~I2C_DIDNT_TIMEOUT; 
    }
    if (print_out) //print results to the test interface
    {
        printf("\n\r Test_LED_Board_Temp     Value = %.3f  %s  Message:  %s", 
                (double)value, (result ? PASS : FAIL), message);
        /* Only print out I2C status if an I2C failure occurs.  This is a 
           low-level detail that's only of interest for diagnostic purposes */
        if (!result && i2c_timeout)
        {
            printf("\n\r   I2C failure occurred in LED board temperature measurement");
        }
    } 
    //-------------------------------------------------------------------
    result = TestPushbutton(false, message);  //Run the individual test
    if (!result && retest)
    {
        result = TestPushbutton(false, message); 
    }
    if (result)
    {
        test_result |= PUSHBUTTON_WORKS;
    }
    if (print_out) //print results to the test interface
    {
        printf("\n\r Test_Pushbuttons                        %s  Message:  %s",
                (result ? PASS : FAIL), message);
    }
    return test_result;
}


/*
 * Description:  This function runs all of the tests for the power on self test (POST).
 *               See DD400004-SDS1 Software Specification that outlines which tests are to be run
 *               as part of POST.  This function will optionally print the pass/fail status and test results
 *               to the test interface UART.
 * Preconditions:  All of the peripherals of the MCU have been initialized per the SYSTEM_Initialize() function.
 * Arguments:  Iterations = POST will be run multiple times based on Iterations 
 *             Print_Out:  0 = do not print to test interface UART, 1 = print test results to test interface UART
 * Returns:  false = Fail, true = Pass
 */
bool TestPOST(int num_iterations, uint16_t *test_results, bool print_out)
{
    bool pass = false;
    char message[256];  //Holds the message sent back from each of the individual test functions
    
    int i;     //general working variable for loops
    for (i=0; i<num_iterations; i++)
    {
        if (print_out)
        {
            printf("\n\r Power On Self Test \n\r"
                   "\n\r ------------------------------------------------------------------------------------------------- \n\r");
        }
        *test_results = PerformCommonTests(message, false, print_out);
  
        //-------------------------------------------------------------------
        bool result = TestRGBLED(message, true);  //Run the individual test
        if (result)
        {
            *test_results |= RGB_LED_WORKS;
        }
        if (print_out) //print results to the test interface
        {
            printf("\n\r Test_RGB_LED                            %s  Message:  %s", 
                    (result ? PASS : FAIL), message);
        }   
        //-------------------------------------------------------------------
        *test_results |= LCD_WORKS;  //we will not test the LCD connectivity as part of POST, PFW 10/06/2020
        //        result = TestLCD(message, true);  //Run the individual test
        //        if (result)
        //        {
        //            *test_results |= LCD_WORKS;
        //        }
        //        if (print_out) //print results to the test interface
        //        {
        //            printf("\n\r Test_LCD                                %s  Message:  %s",
        //                    (result ? PASS : FAIL), message);
        //        }   
        //-------------------------------------------------------------------
        uint16_t uiValue;  //Temporary variable to retrieve test values
        result = TestFlash(&uiValue, message);  //Run the individual test
        if (result)
        {
            *test_results |= FLASH_MEM_WORKS; 
        }
        if (print_out) //print results to the test interface
        {
            printf("\n\r Test_FLASH              Value = %04X    %s  Message:  %s",
                    uiValue, (result ? PASS : FAIL), message);
        }  
        //-------------------------------------------------------------------
        bool i2c_timeout = true;
        const uint16_t MAX_NUM_RETRIES = 12;
        uint16_t num_retries = 0;
        while (i2c_timeout && (num_retries++ < MAX_NUM_RETRIES))
        {
            result = TestLEDBoardEEPROM(message, &i2c_timeout);  //Run the individual test
        }
        if (result)
        {
            *test_results |= LED_BOARD_EEPROM_WORKS;
        }
        if (i2c_timeout)
        {
            /* The I2C_DIDNT_TIMEOUT bitflag will have been set by
             * PerformCommonTests() based on its own I2C operations. If a
             * failure occurs here, we now have to clear this bitflag. 
             * A more elegant solution would require some re-architecting
             * of the POST and BIST tests, so let's document this approach
             * here for now. */
            *test_results &= ~I2C_DIDNT_TIMEOUT; 
        }
        if (print_out) //print results to the test interface
        {
            printf("\n\r Test_LED_Board_EEPROM                   %s  Message:  %s", 
                    (result ? PASS : FAIL), message);
            if (i2c_timeout)
            {
                printf("\r\n   I2C interface timed out during EEPROM test");
            }
        }  
        //-------------------------------------------------------------------
        result = RTCCGetSRAM(message);  //Run the individual test
        if (result)
        {
            *test_results |= BATTERY_VALID; 
        }
        if (print_out) //print results to the test interface
        {
            printf("\n\r Test RTCC Battery                       %s  Message:  %s",
                    (result ? PASS : FAIL), message);
        }  
        //-------------------------------------------------------------------
        
        if (i==0)
        {
            pass = (*test_results == POST_REQUIREMENTS);
        } 
        if ((i>0) && (*test_results != POST_REQUIREMENTS))
        {
            pass = false;
        }
    }
  
    return pass;   
}


/*
 * Description:  This function runs all of the tests for the built in self test (BIST).
 *               See DD400004-SDS1 Software Specification that outlines which tests are to be run
 *               as part of BIST.  This function will optionally print the pass/fail status and test results
 *               to the test interface UART.
 * Preconditions:  All of the peripherals of the MCU have been initialized per the SYSTEM_Initialize() function.
 * Arguments:  Iterations = BIST will be run multiple times based on Iterations 
 *             Print_Out:  0 = do not print to test interface UART, 1 = print test results to test interface UART
 * Returns:  false = Fail, true = Pass
 */
bool TestBIST(int num_iterations, uint16_t *test_results, bool print_out)
{
    char message[256];  //Holds the message sent back from each of the individual test functions
    // Briefly blink BIST LED
    BIST_LED_SetHigh();
    __delay_ms(10);
    BIST_LED_SetLow();
    
    bool pass = false;
    int i;     //general working variable for loops    
    for (i=0; i<num_iterations; i++)
    {
        if (print_out)
        {
            printf("\n\r Built in Self Test \n\r"
                   "\n\r ------------------------------------------------------------------------------------------------- \n\r");
        }
        *test_results = PerformCommonTests(message, true, print_out);

        float value;
        bool result = TestLEDCurrent(((float) GetCurrentlyUsedLEDCurrent())/1000.0,
                                        &value, message);
        if (!result)
        {
            // Perform a second time
            result = TestLEDCurrent(((float) GetCurrentlyUsedLEDCurrent())/1000.0,
                                        &value, message);
        }
        if (result)
        {
            *test_results |= LED_CURRENT_ACCURATE;
        }
        
        if (print_out)  //print results to the test interface 
        {
            printf("\n\r Test_LED_Current   Value = %.3f  %s  Message:  %s", 
                    (double)value, (result ? PASS : FAIL), message);
        }
        //-------------------------------------------------------------------
        uint16_t hw_watchdog_results; 
        result = TestHWWdog(&hw_watchdog_results, message, false);  //Run the individual test
        if (!result)
        {
            result = TestHWWdog(&hw_watchdog_results, message, false); 
        }
        if (result)
        {
            *test_results |= HW_WATCHDOG_TEST_PASSED;
        }
        if (print_out) //print results to the test interface
        {
            printf("\n\r Test_HW_WDOG       Value = %d  %s  Message:  %s",
                    hw_watchdog_results, (result ? PASS : FAIL), message);
        }   
        //-------------------------------------------------------------------
        if (i==0)
        {
            /* When invoking BIST from the test interface, we should consider
             * whether the hardware watchdog signal has been asserted,
             * so we use this as one of the criteria in setting 'pass'.
             * When executing BIST during normal operation, we should
             * only examine how *test_results has been set, comparing
             * it against BIST_REQUIREMENTS.
             */
            pass = (*test_results == (BIST_REQUIREMENTS | HW_WATCHDOG_TEST_PASSED));
        } 
        if ((i>0) && (*test_results != (BIST_REQUIREMENTS | HW_WATCHDOG_TEST_PASSED)))
        {
            pass = false;
        }
    }
    return pass;
}


/*
 * Description:  This function will create a software watchdog timeout by looping forever
 * Preconditions:  The software watchdog needs to be initialized
 * Arguments:  void
 * Returns:  void
 */
static void TestSWWdog(void)
{
    while (true)
    {
        printf("\n\rInitiating software watchdog timeout.");
        __delay_ms(1000);        
    }
}


/*
 * Description:  This function turns the Alarm on or off based on user entry
 * Preconditions:  The digital output bit that drives the Alarm control must be initialized
 * Arguments:  void
 * Returns:  void    
 */
static void SetAlarm(void)
{
    printf("\n\r  Turn alarm on or off");
    printf("\n\r  0.  Off");
    printf("\n\r  1.  On");
    printf("\n\r  2.  Exit");
    printf("\n\r  > ");
    
    switch (GetCharWithinLimits('0', '2'))
    {
        case '0':
            ALARM_ON_SetLow();
            printf("\r\nAlarm turned off\r\n");
            break;
        case '1':
            ALARM_ON_SetHigh();
            printf("\r\nAlarm turned on\r\n");
            break;
        case '2':
        default:  // default included to satisfy static code analyzers
            printf("\r\nAborting test\r\n");
            break;         
    }
}


/*
 * Description:  This function sets the BAUD rate for the UART1 which services the test interface
 * Preconditions:  The UART1 needs to be initialized
 * Arguments:  void
 * Returns:  void    
 */
static void SetUART1Baud(void)
{
    printf("\n\r  Select BAUD Rate for Test Interface");
    printf("\n\r  1.  9600");
    printf("\n\r  2.  19200");
    printf("\n\r  3.  38400");
    printf("\n\r  4.  115200");
    printf("\n\r  5.  235294  (Use FTDI USB--Serial device)");
    printf("\n\r  6.  444444  (Use FTDI USB--Serial device)");
    printf("\n\r  7 to exit");
    printf("\n\r  > ");
    
    static const uint16_t BAUD_RATE_SETTING[6] =
        {0x01A0, 0x00CF, 0x0067, 0x0022, 0x0010, 0x0008};
    char c = GetCharWithinLimits('1', '7');
    if ((c >= '1') && (c <= '6'))
    {
        printf("\r\nBaud rate changed\r\n");
        U1BRG = BAUD_RATE_SETTING[c-'1'];
    }
    else
    {
        printf("\r\nBaud rate unchanged\r\n");
    }
 }


/*
 * Description:  This function tests UART1 which services the test interface
 * Preconditions:  The UART1 needs to be initialized
 * Arguments:  void
 * Returns:  void    
 */
static void TestUART1(void)
{
    printf("\n\r UART1 Test");
    printf("\n\r There are two sections to this test.  The first section will test how well characters can be");
    printf("\n\r transmitted to the console.  The second section will test how well characters can be received");
    printf("\n\r from the console.  The test can be used to verify how robust the UART is under different");
    printf("\n\r BAUD rates.  Hitting the enter key will exit each section.");
    
    char c = 0;  //holds data being read in from the console
    while (c != CR) //if a carriage return is entered, we're done with this part of the test
    {
        printf("\n\r Enter in any character and it will be printed to the screen: 80 times 40 rows.  Hit enter to exit.");
        printf("\n\r\n\r\n\r");
        c = UART1_Read();
        printf("%c", c);
 
        //Print a bunch of characters to the screen so they can be viewed as a large pattern
        int i;
        for (i=1; i<=40; i++)
        {
            printf("\n\r");
            int j;
            for (j=1; j<=80; j++)
            {
                printf("%c", c);  //print the character to the screen
            }
        }        
    }

    printf("\n\r Test how well characters can be received from the console.");
    
    bool break_received = false;
    while (!break_received) 
    {
        printf("\n\r Hold down any key on the keyboard except the enter key (which automatically exits this routine.)\n\r");
        
        c = UART1_Read();  
        if (c == CR)
        {
            break_received = true; //break from the loop
        }
        else
        {
            printf("\n\r Testing character: %c\n\r", c);
            char d = c;
            int k = 0;
            while (d == c)
            {
                k++;
                printf("\r%d",k);   //print how many times the proper character has been received
                d = UART1_Read();   //read another character from the console
            }
        }
    }    
    printf("\n\r  End of UART test.\n\r");
}

/*
 * Description:  This function implements the Test Interface which allows test functions to be run via UART1  
 *               connected to a simple terminal
 * Preconditions:  Generally, full system initialization should be done.  However if only certain tests are done,
 *                 then only the subsystems associated with those tests need to be initialized.
 * Arguments:  Test is the first menu entry and prescribes a test to be done, or some kind of action
 * Returns:  void
 */
void TestInterface(char cmd)
{
    const int MAX_MSG_LEN = 500;
    char message[MAX_MSG_LEN];   //holds message coming back from the test function that is run
    switch (cmd)
    {
        case '0':
            { 
                float fValue;
                bool passed = Test24VDC(&fValue, message);
                printf("\n\r Value = %.3f  Message = %s  %s\n\r", (double) fValue, message, (passed ? "PASSED" : "FAILED"));
            }
            break;
        case '1':
            { 
                float fValue;
                bool passed = Test5VDC(&fValue, message);
                printf("\n\r Value = %.3f  Message = %s  %s\n\r", (double) fValue, message, (passed ? "PASSED" : "FAILED"));
            }
            break;
        case '2':
            { 
                float fValue;
                bool passed = Test33VDC(&fValue, message);
                printf("\n\r Value = %.3f  Message = %s  %s\n\r", (double) fValue, message, (passed ? "PASSED" : "FAILED"));
            }
            break;
        case '3':
            { 
                bool passed = TestAlarm(message);
                printf("\n\r Message = %s  %s\n\r",  message, (passed ? "PASSED" : "FAILED"));
            }
            break;
        case '4':
            { 
                bool passed = TestRGBLED(message, false);
                printf("\n\r Message = %s  %s\n\r",  message, (passed ? "PASSED" : "FAILED"));
            }
            break;
        case '5':
            { 
                bool passed = TestLCD(message, false);
                printf("\n\r Message = %s  %s\n\r",  message, (passed ? "PASSED" : "FAILED"));
            }
            break;
        case '6':
            { 
                int iValue;
                bool passed = TestFan(&iValue, message);
                printf("\n\r Value = %d  Message = %s  %s\n\r", iValue, message, (passed ? "PASSED" : "FAILED"));
            }
            break;
        case '7':
            { 
                uint16_t data;
                bool passed = TestFlash(&data, message);
                printf("\n\r MFG ID = 0x%04X  Message = %s  %s\n\r", data, message, (passed ? "PASSED" : "FAILED"));
            }
            break;
        case '8':
            { 
                float fValue;
                bool i2c_timeout = true;
                const uint16_t MAX_NUM_RETRIES = 12;
                uint16_t num_retries = 0;
                bool passed = false;
                while (i2c_timeout && (num_retries++ < MAX_NUM_RETRIES))
                {
                    passed = TestControlBoardTemp(&fValue, message, &i2c_timeout);
                }
                printf("\n\r Value = %.3f  Message = %s  %s\n\r", (double) fValue, message, (passed ? "PASSED" : "FAILED"));
                if (i2c_timeout)
                {
                    printf("   I2C interface timed out\r\n");
                }
            }
            break;
        case '9':
            { 
                bool timeout = true;
                const uint16_t MAX_NUM_RETRIES = 12;
                uint16_t num_retries = 0;
                bool passed = false;
                while (timeout && (num_retries++ < MAX_NUM_RETRIES))
                {
                    passed = TestLEDBoardEEPROM(message, &timeout);
                }
                printf("\n\r Message = %s  %s\n\r",  message, (passed ? "PASSED" : "FAILED"));
                if (timeout)
                {
                    printf("   I2C interface timed out during EEPROM test\r\n");
                }
            }
            break;
        case 'a':
            {
                float fValue;
                bool i2c_timeout = true;
                const uint16_t MAX_NUM_RETRIES = 12;
                uint16_t num_retries = 0;
                bool passed = false;
                while (i2c_timeout && (num_retries++ < MAX_NUM_RETRIES))
                {
                     passed = TestLEDBoardTemp(&fValue, message, &i2c_timeout);
                }
                printf("\n\r LED temperature = %.3f  Message = %s  %s\n\r", (double) fValue, message, (passed ? "PASSED" : "FAILED"));
                if (i2c_timeout)
                {
                    printf("   I2C interface timed out\r\n");
                }
            }
            break;
        case 'b':
            {
                float fValue;
                bool passed = TestLEDCurrent(0.0, &fValue, message);
                printf("\n\r LED current = %.3f  Message = %s  %s\n\r", (double) fValue, message, (passed ? "PASSED" : "FAILED"));
            }
            break;
        case 'c':
            { 
                uint16_t data;
                bool passed = TestHWWdog(&data, message, true);
                printf("\n\r HW Watchdog = 0x%04X  Message = %s  %s\n\r", data, message, (passed ? "PASSED" : "FAILED"));
            }
            break;
        case 'd':
            { 
                printf("\n\r"); 
                uint16_t iterations;
                input_status_t status = INPUT_INVALID;
                while (status == INPUT_INVALID)
                {
                    printf("Enter number of POST iterations or 'x' to cancel >"); 
                    status = ReadUInt16OrCancel(&iterations);
                    if (status == INPUT_INVALID)
                    {
                        printf("\r\nInput invalid\r\n");
                    }
                }
                if ((status == INPUT_CANCELED) || (iterations == 0))
                {
                    printf("\r\nTest canceled\r\n");
                }
                else
                {
                    uint16_t results;
                    bool passed = TestPOST(iterations, &results, true);
                    printf("\n\r POST Iterations = %d  %s\n\r", iterations,
                            (passed ? "PASSED" : "FAILED"));
                }
            }
            break;
        case 'e':
            { 
                printf("\n\r"); 
                uint16_t iterations;
                input_status_t status = INPUT_INVALID;
                while (status == INPUT_INVALID)
                {
                    printf("Enter number of BIST iterations or 'x' to cancel >"); 
                    status = ReadUInt16OrCancel(&iterations);
                    if (status == INPUT_INVALID)
                    {
                        printf("\r\nInput invalid\r\n");
                    }
                }
                if ((status == INPUT_CANCELED) || (iterations == 0))
                {
                    printf("\r\nTest canceled\r\n");
                }
                else
                {
                    uint16_t results;
                    bool passed = TestBIST(iterations, &results, true);
                    printf("\n\r BIST Iterations = %d  %s\n\r", iterations,
                            (passed ? "PASSED" : "FAILED"));
                }
            }
            break;
        case 'f':
            {
                printf("\n\r"); 
                float led_current;
                if (WaitForValidFloat("\n\r Enter value for LED Current (A) or 'x' to cancel > ",
                        &led_current) == INPUT_VALID)
                {
                    WriteLEDCurrent(led_current, true);
                }
                else
                {
                    printf("\r\nOperation canceled\r\n");
                }
            }
            break;
        case 'g':
            { 
                float fValue;
                TestLEDCurrent(0.0, &fValue, message);
                printf("\n\r LED current = %.3f", (double) fValue);
            }          
            break;
        case 'h': 
        case 'H': 
        case 'm': 
        case 'M': 
            PrintMenu();
            break;
        case 'i':
            printf("\n\r LED voltage = %.3f", (double) ReadLEDVoltage());
            break;
        case 'j':
            { 
                printf("Pushbutton test. Enter:  0 = Simple, 1 = Interactive, x = cancel > ");
                int16_t iValue = GetZeroOneOrCancel();
                if (iValue == -1)
                {
                    printf("\r\nTest canceled\r\n");
                }
                else
                {
                    bool passed = TestPushbutton((bool) iValue, message);
                    printf("\n\r  %s  %s\n\r", (passed ? "PASSED" : "FAILED"),
                            message);
                }
            }
            break;
        case 'k':
            {
                printf("\r\nLights test. Enter:  0 = Simple, 1 = Interactive, x = Cancel > ");
                uint16_t iValue = GetZeroOneOrCancel();
                if (iValue == -1)
                {
                    printf("\r\nTest canceled\r\n");
                }
                else
                {
                    bool passed = TestLights((bool) iValue);
                    printf("\n\r %s\n\r", (passed ? "PASSED" : "FAILED"));
                }
            }
            break;
        case 'l':
            TestSWWdog();
            break;
        // case 'm' already handled earlier
        case 'n':
            LEDBoardEEPROMSubmenu();
            PrintMenu();
            break;
        case 'o':
            UserInterfaceSubmenu();
            PrintMenu();
            break;
        case 'p':
            ImageFlashSubmenu();
            PrintMenu();
            break;
        case 'q':
            TestUART1();
            break;
        case 'r':
            SetUART1Baud();
            break;
        case 's':
            {
                printf("\n\r"); 
                float fValue;
                if (WaitForValidFloat("\n\r Enter ValueFrom (A) or 'x' to cancel > ",
                        &fValue) == INPUT_VALID)
                {
                    float fValue2;
                    if (WaitForValidFloat("\n\r Enter ValueTo (A) or 'x' to cancel > ",
                        &fValue2) == INPUT_VALID)
                    {
                        WriteDAC(fValue, fValue2, false);
                    }
                    else
                    {
                        printf("\r\nOperation canceled\r\n");
                    }
                }
                else
                {
                    printf("\r\nOperation canceled\r\n");
                }
            }
            break;
        case 't':
            printf("\n\r Write LCD Test Image");
            DisplayTestImage();
            break;
        case 'u':
            {
                printf("\r\nSetting current offset...\r\n");
                message[0] = 0;
                bool passed = TestLEDCurrentOffset(message);
                printf("Message = %s  %s\n\r", message, (passed ? "PASSED" : "FAILED"));
            }
            break;
        case 'v':
            {
                uint16_t color;
                WaitForValidUInt16WithLimits("\n\r Specify color "
                        "(1 = RED, 2 = GREEN, 3 = BLUE, 4 = YELLOW, 5 = WHITE, 6 = BLACK, 7 = TEST >",
                        &color, 1, 7);
                WriteRGBLED((led_color_t) (color-1), 0, 0, 0, 0);
                printf("\r\n");
            }
            break;
        case 'w':
            printf("\n\r  Toggle backlight on and off.");
            BACKLIGHT_OFF_Toggle();
            break;
        case 'x':
            printf("\n\rSoftware watchdog timer disabled\r\n");
            _SWDTEN = 0;
            break;
        case 'y':
            SetAlarm();
            break;
        case 'z':
            ReadFWVersion(message);
            printf(message);
            break;
        case 'A':
            GenerateCountdownDisplay();
            break;
        case 'B':
            BlankOutDisplay(false);
            break;
        case 'C':
            Print_to_LCD();
            break;
        case 'D':
            EEPROMSubmenu();
            break;
        case 'E':
            RTCCSubmenu();
            break;
        case 'F':
            if(RTCCGetSRAM(message))
            {
                printf("\n\r Battery has kept SRAM contents:  %s", message);
            }
            else
            {
                printf("\n\r Battery has failed or RTCC SRAM not initialized properly:  %s", message);
            }
            break;
        case 'Z':
            break;
        default:
            break;
    }
}


void PrintMenu(void)
{
    printf("\n\r ");
    printf("\n\r      MuReva Control Board Main Menu");
    printf("\n\r ");
    printf("\n\r      Testing                            Diagnostics                          System Setup                 ");
    printf("\n\r---------------------------------    ---------------------------------    ---------------------------------");
    printf("\n\r  0 = Test 24.0 VDC                                                       n = LED Board EEPROM Sub-menu    ");
    printf("\n\r  1 = Test 5.0 VDC                                                        o = User Interface Sub-menu      ");
    printf("\n\r  2 = Test 3.3 VDC                                                        p = Image/FLASH Sub-menu         ");
    printf("\n\r  3 = Test Alarm Connection          y = Turn Alarm On/Off                D = MCA EEPROM Sub-menu          ");
    printf("\n\r  4 = Test RGB LED Connection        v = Write RGB LED with a color       E = RTCC Sub-menu                ");
    printf("\n\r  k = Test Lights                                                                                          ");
    printf("\n\r  5 = Test LCD Connection            w = LCD Backlight Control                                             ");
    printf("\n\r  t = Test LCD Image                 C = Print to LCD                                                                      ");
    printf("\n\r  6 = Test Fan Connection and RPM                                                                          ");
    printf("\n\r  7 = Test FLASH, print MFG ID                                                                             ");
    printf("\n\r  8 = Test Control Board Temp                                                                              ");
    printf("\n\r  9 = Test LED Board EEPROM          g = Read LED Current                                                                      ");
    printf("\n\r  a = Test LED Board Temp            f = Write LED Current                                                 ");
    printf("\n\r  b = Test LED Current               i = Read LED Voltage                                                  ");
    printf("\n\r  u = Test LED Current Offset        s = Write DAC voltage                                                 ");
    printf("\n\r  F = Test RTCC Battery                                                                                    ");
    printf("\n\r  j = Test Pushbuttons                                                        Misc                         ");
    printf("\n\r  l = Test SW Watchdog               x = Disable SW Watchdog              ---------------------------------");
    printf("\n\r  c = Test HW Watchdog                                                    A = Generate Countdown Display   ");
    printf("\n\r  q = Test UART1                     r = Set UART1 BAUD rate              B = Start User Interface         ");
    printf("\n\r  d = Test POST                                                           z = Read Software Version        ");
    printf("\n\r  e = Test BIST                                                           m = Menu                         ");
    printf("\n\r");
    printf("\n\r   Notes:                                                                                     ");
    printf("\n\r   1.  Some tests require the software watchdog to be disabled, otherwise it will time out.   ");
    printf("\n\r   2.  User interface state machine does not operate when in any submenu.                     ");
    printf("\n\r   3.  Using this Test Interface does not prevent the screensaver from turning on.            ");
    printf("\n\r   4.  Hardware Watchdog Errors that are not cleared by a power cycle will be found at POST.  ");
    printf("\n\r");
}


/* Description:  This function implements a submenu of selections to program
 *               the image flash memory 
 * Preconditions:  SPI module for the FLASH and LCD interfaces.  UART module
 *                 will need initialized for com.
 * Arguments:  void
 * Returns:  void
 */
static void ImageFlashSubmenu(void)
{
	bool done = false;

	// Disable watchdog timer
	_SWDTEN = 0;
    
    while(!done)
    {
        printf("\n\r");
        printf("\n\r");
        printf("\n\r    Image/FLASH Sub-Menu");
        printf("\n\r---------------------------------------------------------------------------------------------");
        printf("\n\r  0 = Write Image to FLASH (requires host software utility)                                  ");
        printf("\n\r  1 = Write Image to LCD                                                                     ");
        printf("\n\r  2 = Display table of stored images (also has FLASH explanation)                            ");
        printf("\n\r  3 = Erase FLASH (in pages)                                                                 ");
        printf("\n\r  4 = Erase FLASH (in blocks)                                                                ");
        printf("\n\r  5 = Read FLASH (in Longs)                                                                  ");
        printf("\n\r  6 = Write FLASH (in Longs)                                                                 ");
        printf("\n\r  7 = Read FLASH Status                                                                      ");
        printf("\n\r  8 = Exit");
        printf("\n\r");

        switch (GetCharWithinLimits('0', '8'))
        {
            case '0':
                WriteImageToFlash();
                break;
            case '1':
                {
                    uint32_t id;
                    if (WaitForValidUInt32("\n\r Enter image identifier or 'x' to cancel > ",
                            &id) == INPUT_VALID)
                    {
                        WriteImageToLCD(id, false, true);
                    }
                    else
                    {
                        printf("\r\nOperation canceled\r\n");
                    }
                }
                break;
            case '2':
                DisplayTableOfImages();
                break;
            case '3':
                EraseDataFromFlashInPages();
                break;
            case '4':
                EraseDataFromFlash();
                break;
            case '5':
                ReadDataFromFlash();
                break;
            case '6':
                WriteDataToFlash();
                break;
            case '7':
                ReadFlashStatus(true);
                break;
            case '8':
                printf("\r\n");
                done = true;
                break;
            default:  // default included to satisfy static code analyzers
                break;
        }
    }
}


/* Description:  This function displays the listing of images, this table is stored in the FLASH at location 0.
 *               (The images are stored somewhere else within FLASH.)
 *               Whenever an image is uploaded to the FLASH using the Write_Image_to_FLASH() function, an entry
 *               is written into the FLASH, somewhere between pages 0 and 255.
 * Preconditions:  SPI module for the FLASH.  
 * Arguments:  void
 * Returns:  void
 */
static void DisplayTableOfImages(void)
{
    printf("\n\r---------------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("\n\r Notes on FLASH organization:  ");
    printf("\n\r 1.  The W25Q16JV FLASH contains 16,777,216 bits = 2,097152 Bytes = 524,288 Longs = 8192 pages = 32 64kB_Blocks");
    printf("\n\r 2.  The FLASH is broken up into 32 blocks, each block is 64kByte = 256 pages, each page is 256 Bytes. ");
    printf("\n\r 3.  Block 0 = Pages 0 to 255 = Holds 256 Image Table entries, each entry gives an identifier, description and location for the Image data. ");
    printf("\n\r 4.  Each 256 Byte Entry is written when the image data is uploaded to the FLASH from the host PC. ");
    printf("\n\r 5.  Block 1 = Pages 256 to 511 = Used as a scratchpad area only do not store any data here as it will be overwritten. ");
    printf("\n\r 6.  Block 2 = Pages 512 to 767 = Reserved for Program usage, possible future upgrades. ");
    printf("\n\r 7.  Block 3 = Pages 768 to 1023 = Reserved for Program usage, possible future upgrades. ");
    printf("\n\r 8.  Block 4 to 31 = Pages 1024 to 8191 and can all be used by image data.");   
    
    printf("\n\r\t\t\t\tBlock\tFrom\tTo\tLong Address");
    int i;  //general working variable for loops   
    for (i=0; i<32; i++)
    {
        printf("\n\r\t\t\t\t%d\t%d\t%d\t%ld", i, 256*i, 256*i+255, 256*(unsigned long)i*64);
        switch (i)
        {
            case 0:
                printf("\tImage Table");
                break;
            case 1:
                printf("\tScratchpad");
                break;
            case 2:
                printf("\tReserved:  Page 512 = LED Board EEPROM Defaults");
                printf("\n\r\t\t\t\t\t\t\t\tReserved:  Page 513 to 517 = User Interface Settings");
                break;
            case 3:
                printf("\tReserved");
                break;
            default:
                printf("\tImage Data");
                break;
        }
    }
    
    printf("\n\r 9.  Image data can cross page and block boundaries, all data for one image must be contiguous.");
    printf("\n\r 10.  If a table entry for an image is erased, then the corresponding image data must also be erased and this must be done manually. ");
    printf("\n\r 11.  Data can only be erased on page boundaries.");
    printf("\n\r 13.  There is also an Erase command that will erase any number of blocks, be careful with this.");
    printf("\n\r 14.  The Read FLASH and Write FLASH commands take a Long address and operate on a Long boundary.");
    printf("\n\r 15.  Images are uploaded to the FLASH using a Python App:  Load_Bitmap.py.  Open a DOS CMD shell:");
    printf("\n\r 16.  DOS> python Load_Bitmap.py COM6 115200 Image.bmp \"RGB Color Graphic\" 1234 10 20 30  ");
    printf("\n\r 17.  Enter: COM port, use 115200 BAUD, bitfile, Description, ID#, Starting page, XPOS, YPOS ");
    printf("\n\r   ");
    // Note that the python code to update images is dependent upon how this table is formatted.  Do not change
    // the formatting of this table without consulting the python code for modifying images!
    printf("\n\r----------The current Table of Images--------------------------------------------------------------------------------------------------------------------");
    printf("\n\r\n\rEntry \tIdentifier \tDescription \t\t\tDateStamp \t\t\tStarting_Page \tTotal_Pages \tXDIM\tYDIM \tXPOS \tYPOS");
    printf("\n\r---------------------------------------------------------------------------------------------------------------------------------------------------------");
    
    for (i=0; i<256; i++)
    {
        uint32_t identifier;
        ReadFlash(i*64, &identifier, 1, 0);
        if (identifier == ULONG_MAX)
        {
            continue;  //  Goto next unprogrammed page
        }
        //The array dimensions below format the table, and Python depends upon this formatting, do not change without 
        //consulting the Python app
        uint32_t Description[30];  //makes these arrays a little longer than necessary because I will pad with spaces
        uint32_t DateStamp[20];    //makes these arrays a little longer than necessary because I will pad with spaces
        uint32_t Starting_Page;
        uint32_t Total_Pages;
        uint32_t XDIM;
        uint32_t YDIM;
        uint32_t XPOS;
        uint32_t YPOS;
    
        ReadFlash(i*64+1, Description, 20, 0);
        ReadFlash(i*64+21, DateStamp, 10, 0);
        ReadFlash(i*64+31, &Starting_Page, 1, 0);
        ReadFlash(i*64+32, &Total_Pages, 1, 0);
        ReadFlash(i*64+33, &XDIM, 1, 0);
        ReadFlash(i*64+34, &YDIM, 1, 0);
        ReadFlash(i*64+35, &XPOS, 1, 0);
        ReadFlash(i*64+36, &YPOS, 1, 0);
    
        //Change length of Description and DateStamp to 30 printable characters - Python code depends upon this formatting!
        strcat((char *)Description, "                                        ");  //concatenate 40 spaces to the end of string
        Description[7] = Description[7] && 0x0000FFFF;  //null terminate the string
         
        strcat((char *)DateStamp, "                                        ");  //concatenate 40 spaces to the end of string
        DateStamp[7] = DateStamp[7] && 0x0000FFFF;  //null terminate the string
   
        //The Python code depends upon this formatting, do not change without consulting the python code!
        printf("\n\r%d \t%ld \t\t%s \t%s \t%ld \t\t%ld \t\t%ld \t%ld \t%ld \t%ld", 
                i, identifier, (char *)Description, (char *)DateStamp, 
                Starting_Page, Total_Pages, XDIM, YDIM, XPOS, YPOS);
    }   
    printf("\n\r End of table");  //Python code depends on this 
}

/*
 * Description:  This function returns the software revision along with other useful information such as
 *               the software name and part number, compile date, compiler revision.
 * Preconditions:  none
 * Arguments:  The Data field should be able to accommodate at least 256 characters
 * Returns:  void
 */   
static void ReadFWVersion(char *output_str)
{
    strcpy(output_str, "\n\r\n\r ");
    strcat(output_str, FW_NAME);
    strcat(output_str, "\n\r ");
    strcat(output_str, FW_PART_NUMBER);
    strcat(output_str, "  ");
    strcat(output_str, FW_VERSION);
    strcat(output_str, "\n\r");
    strcat(output_str, " Compiled on ");
    strcat(output_str, __DATE__);
    strcat(output_str, "  ");
    strcat(output_str, __TIME__);  
    strcat(output_str, "\n\r");
    strcat(output_str, " Microchip Technology Compiler XC16 version: ");
    char XC16_Ver[256];   //temporary holding for the compiler version
    sprintf(XC16_Ver, "%d", __XC16_VERSION__ );  //convert from integer to a character string
    strcat(output_str, XC16_Ver); 
    strcat(output_str, "\n\r");
    strcat(output_str, " Processor:  PIC24FJ1024GA606: 16bit 16MIPS\n\r");
    strcat(output_str, " Memory:  1MB program memory, 32kB SRAM\n\r");
    strcat(output_str, "          2MB FLASH for images/data    \n\r");
    strcat(output_str, " LCD:  320x240, 24bit color, SSD2119 controller \n\r");
}

/*
 * Description:  This function prints text to the LCD
 * Preconditions:  The SPI going to the LCD needs to be initialized
 * Arguments:  void
 * Returns:  void
 */   
static void Print_to_LCD(void)
{   
    const int BUFFER_LEN = 32;   //Max length of characters to be printed.
    char data[BUFFER_LEN];       //This holds the data to be printed to the screen
    
    uint16_t XPOS;
    uint16_t YPOS;              //Starting position of text on the screen

    printf("\n\r  Print to LCD");
    printf("\n\r  Enter text to print>");
    ReadString(data, BUFFER_LEN);   //Get data from console
    WaitForValidUInt16WithLimits("\n\r  Enter X position>", &XPOS, 0, 319);
    WaitForValidUInt16WithLimits("\n\r  Enter Y position>", &YPOS, 0, 239);

 
    DisplayText(data, XPOS, YPOS);
   
    
}

/*
 * Description:  This function implements a submenu for reading and writing text/data to
 *               the 1-wire EEPROM on the MCA
 * Preconditions:  The pin manager should have the IO pin for the 1-wire interface 
 * Arguments:  void
 * Returns:  void
 */   
static void EEPROMSubmenu(void)
{
	bool done = false;
    rtcc_data_t r;  //data from real time clock
    
    // Get today's date and time
    RTCCGetDateTimeData(&r);
    
	// Disable watchdog timer
	_SWDTEN = 0;
	if (!EEPROMIdentify())
	{
		// If the SWI EEPROM cannot be detected, then immediately exit this menu
		done = true;
	}
	while (!done)
	{
        printf("\n\r");
        printf("\n\r");
		printf("\r\n    MCA EEPROM Sub-Menu\r\n");
		printf("---------------------------------------------------------------------------------------------\r\n");
		printf("  0 = Display contents of MCA EEPROM\r\n");
		printf("  1 = Set serial number\r\n");
		printf("  2 = Set MCA calibration constant\r\n");
		printf("  3 = Set active use time in seconds\r\n");
		printf("  4 = Set maximum use time in seconds\r\n");
		printf("  5 = Set time stamp (number entry)\r\n");
		printf("  6 = Set time stamp (auto = 20%d%d/%d%d/%d%d %d%d:%d%d:%d%d)\r\n", r.rtcyear.yrten, r.rtcyear.yrone, r.rtcmth.mthten, r.rtcmth.mthone, r.rtcdate.dateten, r.rtcdate.dateone, r.rtchour.hrten, r.rtchour.hrone, r.rtcmin.minten, r.rtcmin.minone, r.rtcsec.secten, r.rtcsec.secone);
		printf("  7 = Dump memory, device notes\r\n");
		printf("  8 = Read memory\r\n");
		printf("  9 = Write memory\r\n");
		printf("  a = Erase memory\r\n");
		printf("  b = Read MFG ID\r\n");
		printf("  c = Lock serial number\r\n");
		printf("  d = Perform data integrity check\r\n");
		printf("  e = Exit\r\n");
        printf("\n\r");
		switch (GetCharWithinLimits('0', 'e'))
		{
		case '0':
			MCADisplayContents();
			break;
		case '1':
			MCASetSerialNumber();
			break;
		case '2':
			MCASetCalibration();
			break;
		case '3':
			MCASetActiveUseTime();
			break;
		case '4':
			MCASetMaxUseTime();
			break;
		case '5':
			MCASetTimeStamp(false);
			break;
		case '6':
			MCASetTimeStamp(true);
			break;
        case '7':
			printf("\r\n");
			EEPROMDump(0);
			break;
		case '8':
			MCAMemoryRead();
			break;
		case '9':
			MCAMemoryWrite();
			break;
		case 'a':
			MCAMemoryErase();
			break;
		case 'b':
			printf("\r\n");
			EEPROMReadManufID(0);
			break;
		case 'c':
			MCALockSerial();
			break;
		case 'd':
			printf("\r\n");
			MCADataIntegrityCheck();
			break;
		case 'e':
			done = true;
			break;
		default:
			break;
		}
	}
	PrintMenu();
}

/*
 * Description:  This function implements a submenu for the real time clock/calendar
 *               device on the control board
 * Preconditions:  The pin manager should have the IO pin for the 1-wire interface 
 * Arguments:  void
 * Returns:  void
 */   
static void RTCCSubmenu(void)
{
	bool done = false;

	// Disable watchdog timer
	_SWDTEN = 0;
	if (!RTCCIdentify())
	{
		// If RTCC cannot be detected, then immediately exit this menu
        printf("\n\r RTCC not detected\n\r");
		done = true;
	}
	while (!done)
	{
        printf("\n\r");
        printf("\n\r");
		printf("\r\n    RTCC Sub-Menu\r\n");
		printf("---------------------------------------------------------------------------------------------\r\n");
		printf("  0 = Display date and time\r\n");
		printf("  1 = Set date and init SRAM\r\n");
		printf("  2 = Set time\r\n");
		printf("  3 = Get PWRFAIL status\r\n");
		printf("  4 = Dump registers\r\n");
		printf("  5 = Read register\r\n");
		printf("  6 = Write register\r\n");
		printf("  7 = Exit\r\n");
        printf("\n\r");
		switch (GetCharWithinLimits('0', '7'))
		{
		case '0':
			RTCCDisplayDateTime();
			break;
		case '1':
			RTCCSetDate();
			break;
		case '2':
			RTCCSetTime();
			break;
		case '3':
			RTCCDisplayPowerFailStatus();
			break;
		case '4':
			RTCCDumpRegisters();
			break;
		case '5':
			RTCCReadRegister();
			break;
		case '6':
			RTCCWriteRegister();
			break;
		case '7':
			done = true;
			break;
		default:
			break;
		}
	}
	PrintMenu();
}
