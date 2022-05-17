/*	Filename:  ledboard.c
 *	Date Created:  11/13/2018
 *	Company:  Valtronic, USA
 *	Project:   Lumitex OraGlo Control Board
 *	Project number:  DD400004
 *	Description:  LED board routines
 */

#include "mcc_generated_files/mcc.h"
#include "Support_Functions.h"
#include "ledboard.h"
#include "analog.h"
#include "timers.h"
#include "flashmem.h"
#include "lcd.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#define    FCY    16000000UL  // required by libpic30.h    
#include <libpic30.h>

#define LED_BOARD_MCP9808_ADDRESS 0x01
#define LED_BOARD_24LC02_ADDRESS 0x00

#define LED_CURRENT_GAIN_AMPLIFIER 1.12f
#define LED_CURRENT_SHUNT 0.056f
#define LED_CURRENT_LIMIT 2.0f

#define RES_LED_VOLTS_PULLUP 25500.0f
#define RES_LED_VOLTS_PULLDOWN 1470.0f

//Test Limits from DD400004-SDS1
#define SW_SPEC_REVA_7_6_11 75.0f
#define SW_SPEC_REVA_7_6_12 0.016f

static const size_t EEPROM_FIELD_LENGTH = 16;
static const uint16_t EEPROM_SIZE = 256;


static const uint16_t EEPROM_ADDR_SERIAL_NUM = 0x50;
static const uint16_t EEPROM_ADDR_DATE_OF_TEST = 0x60;
static const uint16_t EEPROM_ADDR_TIME_OF_TEST = 0x70;
static const uint16_t EEPROM_ADDR_CAL_CONST1 = 0x80;

/* Indicates whether a non-zero LED current has been specified
 * through the command menu system.  This is used to ensure that
 * these commanded currents don't generate a "therapy on" error. */
static bool g_led_current_on = false;


static void DisplayLEDBoardEEPROMContents(void);
static void InitializeLEDBoardEEPROM(void);
static void WriteLEDBoardTestInfo(void);
static void WriteLEDBoardDefaultCurrent(void);


/* Description:  This function tests the LED board EEPROM by reading the
 *               contents, writing a test pattern, then restoring the original
 *               contents.  This determines the pass/fail.
 * Preconditions:  The I2C1 module that communicates to the LED board EEPROM
 *                 needs to be initialized
 * Arguments:  message should allow for up to 255 characters.  Test failure
 *             details will be placed here
 * Returns:  false = Fail, true = Pass
 */
bool TestLEDBoardEEPROM(char *message, bool *timeout)
{
    const char *I2C_ERROR_MSG = "I2C timeout error in EEPROM test\r\n";
    char data[EEPROM_SIZE];  //will hold entire contents of the LED board EEPROM
    char test_data[EEPROM_SIZE];  //Will hold the test pattern when it is read back from the LED board EEPROM
    
    strcpy(message, "Test LED Board EEPROM ");  //initialize the message string
    
    const uint8_t TEST_PATTERN_ADDRESS = 0x90;
    const char* TEST_STRING = "____Test1234";
    ReadLEDBoardEEPROM(data, timeout);  //Read the entire contents of the LED board EEPROM
    // Use early returns and multiple exit points to prevent deep nesting
    if (*timeout)
    {
        sprintf(message, I2C_ERROR_MSG);
        return false;
    }

    //Write a test pattern to the LED board EEPROM at given address
    WriteLEDBoardEEPROM(TEST_PATTERN_ADDRESS, TEST_STRING, strlen(TEST_STRING), timeout); 
    if (*timeout)
    {
        sprintf(message, I2C_ERROR_MSG);
        return false;
    }
    
    ReadLEDBoardEEPROM(test_data, timeout);  //Read the entire contents of the LED board EEPROM
    if (*timeout)
    {
        sprintf(message, I2C_ERROR_MSG);
        return false;
    }
    bool pass = (strncmp(TEST_STRING, &test_data[TEST_PATTERN_ADDRESS],
                               strlen(TEST_STRING)) == 0);
    if (!pass)
    {
        sprintf(message, "Expected test pattern not found at expected location");
    }

    //Restore the original contents. 
    WriteLEDBoardEEPROM(TEST_PATTERN_ADDRESS, &data[TEST_PATTERN_ADDRESS],
            EEPROM_FIELD_LENGTH, timeout); 
    if (*timeout)
    {
        sprintf(message, I2C_ERROR_MSG);
        pass = false;
    }
    return pass;
}


/* Description:  This function writes the Data to the LED board EEPROM starting
 *               at the given Address.  Only NumBytes of data will be written.
 * Preconditions:  The I2C module (I2C1) that communicates to the LED board
 *                 EEPROM needs to be initialized
 * Arguments:  Address is the pointer into the 24LC02 EEPROM and is expected
 *               to be between 0 and 255 inclusive
 *             Data is a string pointer that contains at least NumBytes worth
 *               of characters to be written
 *             NumBytes is the number of bytes to be written to the EEPROM
 * Returns:  void
 */
void WriteLEDBoardEEPROM(int address, const char *data, int num_bytes, bool *timeout)
{
    unsigned char one_transfer[2] = {0x00, 0x00};   //small packet of just one address byte and one data byte
    int slave_device_address = 0xA0;  //I2C address of EEPROM
    slave_device_address |= (LED_BOARD_24LC02_ADDRESS*2);  //OR in the address
    slave_device_address >>= 1;  //shift it right by one since this is what the I2C1_MasterWrite function requires
    I2C1_MESSAGE_STATUS status; //status of I2C interface
    
    if (num_bytes > 256)
    {   //only allow 256 bytes to be written since that is the maximum for the 24LC02 EEPROM
        num_bytes = 256;
    }  
    if (num_bytes < 0)
    {
        num_bytes = 0; // num_bytes must be non-negative
    }  
 
    int int_address;  //internal Address variable
    for (int_address = address; int_address < address+num_bytes; int_address++)
    {
        one_transfer[0] = int_address;
        one_transfer[1] = data[int_address-address];  //each control word is 0x00 appended with data to be written to control register
        
        I2C1_MasterWrite(one_transfer, 2, slave_device_address, &status);  //put out on the I2C bus
        while (status == I2C1_MESSAGE_PENDING)
        {
            __delay_ms(1); //Hang up here until message has arrived
        } 
        __delay_ms(5);   //need a 5ms delay here, that is the maximum write cycle time for the 24LC02 EEPROM
        
        I2C1_MasterWrite(0x00, 0, slave_device_address, &status);  //see if the EEPROM is done writing the byte
        while (status == I2C1_MESSAGE_PENDING)
        {
            __delay_ms(1); //Hang up here until message has arrived
        } 
        
        if (status != I2C1_MESSAGE_COMPLETE)
        {
            printf("LED_EEPROM_Write problem");
        }
    }
}


/* Description:  This function reads the entire contents of the LED board
 *               EEPROM (256 bytes) and places it in the data field. 
 * Preconditions:  The I2C module (I2C1) that communicates to the LED board
 *                 EEPROM needs to be initialized
 * Arguments:  data is a string pointer that allows for at least 257 characters
 *                to be written (null terminated)
 *             data must be initialized prior to calling this function
 * Returns:  void
 */
void ReadLEDBoardEEPROM(char *data, bool *timeout)
{
    int slave_device_address = 0xA0;  //I2C address of EEPROM
    slave_device_address |= (LED_BOARD_24LC02_ADDRESS*2);  //OR in the address
    slave_device_address >>= 1;  //shift it right by one since this is what the I2C1_MasterWrite function requires
    
     //assign the character pointer to the unsigned integer pointer, since the I2C1_MasterRead() function expects uint8_t
    I2C1_MESSAGE_STATUS status; //status of I2C interface
    uint8_t *pData = (uint8_t *) data;   
    
    I2C1_MasterWrite(0x00, 1, slave_device_address, &status);  //set the address pointer back to zero
    const uint16_t I2C_TIMEOUT_PERIOD = 1000; // in milliseconds
    const uint16_t DELAY = 1;
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
    if (*timeout)
    {
        printf("LED board EEPROM reading problem\r\n");
    }
    else
    { 
        I2C1_MasterRead(pData, EEPROM_SIZE-1, slave_device_address, &status);  //read data starting at location zero
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
            printf("LED board EEPROM reading problem\r\n");
        }
        data[256] = 0;  //null terminate the character string       
    }
}

/*
 *   Returns the LED current to be used, as stored on the LED board
 *   EEPROM.  Unit of milliamps.
 */
uint16_t ReadLEDBoardSpecifiedCurrent(void)
{
    int slave_device_address = 0xA0;  //I2C address of EEPROM
    slave_device_address |= (LED_BOARD_24LC02_ADDRESS*2);  //OR in the address
    slave_device_address >>= 1;  //shift it right by one since this is what the I2C1_MasterWrite function requires
    I2C1_MESSAGE_STATUS status; //status of I2C interface
    
    uint8_t LED_CURRENT_ADDRESS = 0x80;
    I2C1_MasterWrite(&LED_CURRENT_ADDRESS, 1, slave_device_address, &status);
    while (status == I2C1_MESSAGE_PENDING)
    {
        __delay_ms(1); //Hang up here until message has arrived
    } 
    
    uint8_t p_data[EEPROM_FIELD_LENGTH];  
    I2C1_MasterRead(p_data, EEPROM_FIELD_LENGTH, slave_device_address, &status); 
    while (status == I2C1_MESSAGE_PENDING) 
    {
        __delay_ms(1); //Hang up here until message has arrived
    } 
    
    if (status != I2C1_MESSAGE_COMPLETE)
    {
        printf("LED_EEPROM_Read problem");
    }
    p_data[15] = 0;  //null terminate the character string
    return (uint16_t) strtol((const char *)p_data, NULL, 0);
}


/* Description:  This function tests the LED board temp sensor and applies limits to the measured temperature
 *               to determine the pass/fail.
 * Preconditions:  The I2C module that communicates to the MCP9808 sensor needs to be initialized
 * Arguments:  The measured temperature is returned in the Value field
 *             Message should allow for up to 255 characters, test failure details will be placed here
 * Returns:  false = Fail, true = Pass
 */
bool TestLEDBoardTemp(float *temperature, char *message, bool *timeout)
{
    uint8_t one_transfer[2] = {0x00, 0x00};   //small packet of just two bytes, used to access the registers of the MCP9808
    
    if (message != NULL)
    {
        strcpy(message, "Test LED Board Temp ");  //initialize the message string
    }
    int slave_device_address = 0x30;  //I2C address of the MCP9808 temperature sensor
    slave_device_address |= (LED_BOARD_MCP9808_ADDRESS*2);  //OR in the address
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
        printf("LED board temperature reading problem\r\n");
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
            printf("LED board temperature reading problem\r\n");
            pass = false;
        }
        else
        {   
            *temperature = (one_transfer[0] & 0x0F) * 256 + one_transfer[1];
            *temperature /= 16;
            if ((one_transfer[0] & 0x10) == 0x10)
            { 
                *temperature -= 256;
            }   //Determine if temperature value is positive or negative 

            pass = (*temperature < SW_SPEC_REVA_7_6_11 );
            if (!pass && (message != NULL))
            {
                sprintf(message, "Expected temperature < %.1fC", (double)SW_SPEC_REVA_7_6_11);
            }
        }
    }
    return pass;
}


/*  Description:  This function tests the LED current and applies limits to the measured current
 *                to determine the pass/fail.
 *  Preconditions:  The ADC acquisition system has been setup and running, providing the averaged, analog input values
 *  Arguments:  The Expected value is normally the last value of current programmed into the current source
 *              The measured current is returned in the Measured field
 *              Message should allow for up to 255 characters, test failure details will be placed here
 *  Returns:  false = Fail, true = Pass    
 */  
bool TestLEDCurrent(float expected, float *measured, char *message)
{
    float measured_current = GetADCReadingCh11() / LED_CURRENT_GAIN_AMPLIFIER;  //calculate the LED current
    float limit_high = expected + SW_SPEC_REVA_7_6_12;
    float limit_low = expected - SW_SPEC_REVA_7_6_12;
    
    *measured = measured_current;
    if (message != NULL)
    {
        sprintf(message, "Limit_Low = %.3f  Limit_High = %.3f",
                (double)limit_low, (double)limit_high);
    }
    return (measured_current >= limit_low) && (measured_current <= limit_high);
}


/* Description:  This function calibrates the LED current measurement by zeroing out the offset.
 *               It does this by turning the current source off, verifying it is off by reading the voltage.
 *               The current will then be measured with the global variable Analog_Ch11_Offset set to zero.
 *               The current that is measured is the offset, and must be zeroed out.  
 *               The global variable Analog_Ch11_Offset is then set to the measured current.
 *               Then another current measurement is done to verify it is much closer.
 * Preconditions:  The ADC acquisition system has been setup and running, providing the averaged, analog input values
 *                 The SPI that services the programmable current source (via DAC) has been setup and initialized
 *                 The digital IO bits associated with the hardware watchdog have been initialized.
 *                 POST and BIST should have been run and passed, to verify over all hardware health
 * Arguments:  Message should allow for up to 255 characters, test failure details will be placed here
 * Returns:  pass/fail indication
 */
bool TestLEDCurrentOffset(char *message)
{
    WriteLEDCurrent(0.0f, false);
    //Set offset back to zero prior to measuring the offset
    ClearADCCh11Offset();
    
    //Turn current source off and verify it is off
    WriteDAC(0, 0, 0);        //Turn current source off by writing zero volts to the DAC
    __delay_ms(640);             //Wait for acquisition system to take enough samples and average
    float voltage = ReadLEDVoltage();  //Read the voltage across the LEDs, we want to make sure the current source is off
    bool pass1 = (voltage <= 0.100);
    if (!pass1 && (message != NULL))  //We fail if we can't turn off the current source
    {
        strcat(message, "LED Voltage > 0.100V when current source is off,");
    }
    
    //Now measure the LED current offset and verify it is within reason
    float measured_current_offset;  //measured current of LED stack    
    TestLEDCurrent(0.0, &measured_current_offset, NULL);
    bool pass2 = (measured_current_offset <= 0.100);
    if (!pass2 && (message != NULL))   //We fail if we can't turn off the current source
    {
        strcat(message, "Current Offset > 0.100A when current source is off,");
    }
    
    SetADCCh11Offset();  //set the offset equal the averaged value
    __delay_ms(640);             //Wait for acquisition system to take enough samples and average
    
    //Now measure the LED current offset and verify it is within reason
    TestLEDCurrent(0.0, &measured_current_offset, NULL);
    bool pass3 = (measured_current_offset <= 0.100);
    if (!pass3 && (message != NULL))    //We fail if we can't turn off the current source
    {
        strcat(message, "Current Offset > 0.100A when current source is off,");
    }
   
    return (pass1 && pass2 && pass3);
}


/*
 * Description:  Reads the voltage across the LED stack
 * Preconditions:  The ADC acquisition system has been setup and running, providing the averaged, analog input values
 * Arguments:  None
 * Returns:  Voltage measurement in volts
 */ 
float ReadLEDVoltage(void)
{
    return ((RES_LED_VOLTS_PULLDOWN+RES_LED_VOLTS_PULLUP)/RES_LED_VOLTS_PULLDOWN) * GetADCReadingCh12();  //calculate the LED_Voltage
}


/*
 * Description:  This function programs the LED current to the prescribed value
 *               in the argument list.  The current will be read back and
 *               verified to be within the required tolerance.  If it is not,
 *               an attempt will be made to adjust the value.
 * Preconditions:  The ADC acquisition system has been setup and running,
 *                 providing the averaged, analog input values.  The SPI
 *                 channel that services the programmable current source
 *                 (via DAC) has been setup and initialized.  The digital I/O
 *                 bits associated with the hardware watchdog have been
 *                 initialized.  POST and BIST should have been run and passed
 *                 to verify overall hardware health.
 * Arguments:  current_level = the LED current (in amps) to be programmed.
 *                             Will be bounded
 *             print_out:  true to print information to test interface UART;
 *                         false otherwise
 * Returns:  void
 */
void WriteLEDCurrent(float current_level, bool print_out)
{
    // Limits that will be applied to insure the programmed current is within 
    // specification.  These limits are tighter than the spec
    float limit_high = current_level + (SW_SPEC_REVA_7_6_12/2);  //high limit for current, in Amps
    float limit_low = current_level - (SW_SPEC_REVA_7_6_12/2);   //low limit for current, in Amps
    
    //Get the value of current that is being sourced at this moment
    __delay_ms(250);  //Delay a minimum amount to account for any previous step change to the current, time for Acq system to average
    float measured_current;  //measured current of LED stack
    TestLEDCurrent(0.0, &measured_current, NULL);
    
    //Apply bounds to the next value for current
    float value_to = current_level;
    if (current_level > LED_CURRENT_LIMIT)
    {
        value_to = LED_CURRENT_LIMIT;  //Upper bound for LED current
    } 
    if (current_level < 0.0)
    {
        value_to = 0.0;  //Lower bound for LED current
    }  
    
    const float MIN_CURRENT_THRESHOLD = 0.001;
    if (value_to < MIN_CURRENT_THRESHOLD)
    {
        /* Just treat the current as zero if it's low enough. 
         * Also just turn the DAC off. */
        WriteDAC(10*LED_CURRENT_SHUNT*measured_current + 0.25, 0.0, 0);
        g_led_current_on = false;
    }
    else
    {
        //Set the DAC value to program the new current
        WriteDAC(10*LED_CURRENT_SHUNT*measured_current + 0.25, 10*LED_CURRENT_SHUNT*value_to + 0.25, 0);  //Write the DAC value, this will program the new current
        __delay_ms(640);  // time for Acq system to average

        //Now measure the new current and determine if it is too low or too high, or within spec
        TestLEDCurrent(0.0, &measured_current, NULL);

        if ((measured_current < limit_low) || (measured_current > limit_high))    //If were outside the limits, try to adjust 
        {
            if (print_out)
            {
                printf("\n\r Adjusting the LED current ");
            }
            float value_adjustment = value_to - measured_current;
            if (print_out)
            {
                printf("  ValueAdjustment = %.3f  ValueTo = %.3f  Measured_Current = %.3f",
                        (double)value_adjustment, (double)value_to, (double)measured_current);
            }
            if (fabs(value_adjustment) < 0.100)  //Do not perform adjustments unless they're small
            {
                WriteDAC(10*LED_CURRENT_SHUNT*value_to + 0.25,
                        10*LED_CURRENT_SHUNT*(value_to + value_adjustment) + 0.25, 0);
            }
        }
        g_led_current_on = true;
    }
}


/* Description:  This function implements a submenu of selections to program
 *               the LED Board EEPROM 
 * Preconditions:  I2C module for the EEPROM.  UART module will need to be
 *                 initialized for comm.
 * Arguments:  void
 * Returns:  void
 */ 
void LEDBoardEEPROMSubmenu(void)
{
	bool done = false;

	// Disable watchdog timer
	_SWDTEN = 0;
    
    while(!done)
    {
        printf("\n\r");
        printf("\n\r");
        printf("\n\r    LED Board Sub-Menu");
        printf("\n\r--------------------------------------------------------------------------------------------------");
        printf("\n\r  0 = Read LED board EEPROM defaults from FLASH                                                   ");
        printf("\n\r  1 = Write LED board EEPROM defaults into FLASH (overwrites previous defaults)                   ");
        printf("\n\r  2 = Read LED board EEPROM                                                                       ");
        printf("\n\r  3 = Write LED board EEPROM with defaults from FLASH (Initialize EEPROM)                         ");
        printf("\n\r  4 = Write LED board EEPROM with serial number, date and time of test, calibration constant #1   ");
        printf("\n\r  5 = Write default LED current (calibration constant #1) to flash memory                         ");
        printf("\n\r  6 = Write default LED current (calibration constant #1) to EEPROM                               ");
        printf("\n\r  7 = Notes on LED Board EEPROM");
        printf("\n\r  8 = Exit");
        printf("\n\r");

        /* Use UART1_Read() instead of GetChar().  GetChar() prints the
         * entered character out, and this will interfere with batch
         * scripts used for updating the images. */
        switch (GetCharWithinLimits('0', '8'))
        {
            case '0':
                PrintLEDBoardEEPROMDefaultsFromFlash();
                break;
            case '1':
                StoreLEDBoardEEPROMDefaultsToFlash();
                break;
            case '2':
                DisplayLEDBoardEEPROMContents();
                break;
            case '3':
                InitializeLEDBoardEEPROM();
                break;
            case '4':
                WriteLEDBoardTestInfo();
                break;
            case '5':
                StoreLEDBoardDefaultCurrentToFlash();
                break;
            case '6':
                WriteLEDBoardDefaultCurrent();
                break;
            case '7':
                LEDBoardEEPROMPrintNotes();
                break;
            case '8':
                done = true;
                break;
            default:
                break;
        }
        printf("\n\r");
    }
}

void LEDBoardEEPROMPrintNotes()
{
    printf("\n\r");
    printf("\n\r---------------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("\n\r Notes on LED Board EEPROM organization:  ");
    printf("\n\r 1.  The 24LC02 EEPROM contains 256 Bytes, interpreted as 256 characters of text.");
    printf("\n\r 2.  The template that defines the fields within the EEPROM is shown below.      ");
    printf("\n\r          Address(hex)   Description of Field                                    ");
    printf("\n\r             00          Company Name                                            ");
    printf("\n\r             10          Product Name                                            ");
    printf("\n\r             20          SubAssy Name                                            ");
    printf("\n\r             30          SubAssy Part Number                                     ");
    printf("\n\r             40          SubAssy Revision                                        ");
    printf("\n\r             50          SubAssy Serial Number                                   ");
    printf("\n\r             60          SubAssy Date of Test                                    ");
    printf("\n\r             70          SubAssy Time of Test                                    ");
    printf("\n\r             80          Calibration Constant #1                                 ");
    printf("\n\r             90-FF       Reserved/Future updates                                 ");
    printf("\n\r                                                                                 ");
    printf("\n\r 3.  Defaults for all fields are stored in FLASH and those defaults can be updated at any time.");
    printf("\n\r 4.  When entering text, use backspace to delete characters, Enter to finish.    ");
    printf("\n\r");
}


/*
 *   Indicates whether LED current is currently active
 */
bool LEDCurrentOn(void)
{
    return g_led_current_on;
}


// ---------------- PRIVATE FUNCTIONS ----------------

/* Description:  The LED board EEPROM contains 256 bytes of data. This
 *               function displays its contents.
 * Preconditions:  I2C interface must be initialized
 */
static void DisplayLEDBoardEEPROMContents(void)
{
    // Holds the data for the EEPROM and/or FLASH defaults.
    // +1 is added to allow for final terminating null character below
    char data[EEPROM_SIZE+1]; 
    
    bool timeout = true; // Will discard timeout value in absence of way to recover
    const uint16_t MAX_NUM_RETRIES = 12;
    uint16_t num_retries = 0;
    while (timeout && (num_retries++ < MAX_NUM_RETRIES))
    {
        ReadLEDBoardEEPROM(data, &timeout); 
    }                                // absence of any way to recover
    char c_to_restore = data[128]; // Copy the character about to be overwritten
    data[128]=0;  //Null terminate the character array to stop the printf at the appropriate character
    printf("\n\r\n\r      Company Name    Product Name    SubAssy Name    SubAssy P/N     SubAssy Rev     SubAssy S/N     Date of Test    Time of Test");
    printf(    "\n\r      00..............10..............20..............30..............40..............50..............60..............70..............");
    printf(    "\n\r      %s", data);

    data[128] = c_to_restore; // Restore the previously overwritten character
    data[256]=0;  //Null terminate the character array to stop the printf at the appropriate character
    printf("\n\r\n\r      Cal Const #1    Reserved        Reserved        Reserved        Reserved        Reserved        Reserved        Reserved        ");
    printf(    "\n\r      80..............90..............A0..............B0..............C0..............D0..............E0..............F0..............");
    printf(    "\n\r      %s", &data[128]);
    printf("\n\r");
}


/*
 *   Reads default EEPROM parameters from flash memory and then
 *   stores them on LED board EEPROM
 */
static void InitializeLEDBoardEEPROM(void)
{
    char data[257];
    uint32_t data_from_flash[64]={0};   //This holds the data for the FLASH
    
    printf("\r\nInitializing LED Board EEPROM ...");
    ReadLEDBoardEEPROMDefaultsFromFlash(data_from_flash);
    int i;
    for (i=0; i<64; i++)   //copy from an unsigned long array to character array
    {
        data[i*4] = (char)((data_from_flash[i] & 0xFF000000)>>24);
        data[i*4+1] = (char)((data_from_flash[i] & 0x00FF0000)>>16);
        data[i*4+2] = (char)((data_from_flash[i] & 0x0000FF00)>>8);
        data[i*4+3] = (char)((data_from_flash[i] & 0x000000FF));
    }
    bool timeout = true;
    const uint16_t MAX_NUM_RETRIES = 12;
    uint16_t num_retries = 0;
    while (timeout && (num_retries++ < MAX_NUM_RETRIES))
    {
        WriteLEDBoardEEPROM(0, data, 256, &timeout);
    }
    printf("Done\r\n");
}


/*
 *   Prompts user for LED board parameters and stores them on LED board EEPROM
 */
static void WriteLEDBoardTestInfo(void)
{
    char data[EEPROM_SIZE];
    bool timeout = true; // Will discard timeout value in absence of way to recover
    const uint16_t MAX_NUM_RETRIES = 12;
    uint16_t num_retries = 0;
    while (timeout && (num_retries++ < MAX_NUM_RETRIES))
    {
        ReadLEDBoardEEPROM(data, &timeout);  //Get the string from the EEPROM, this will be modified with user entries
    }
    //-----------------------------------
    printf("\n\r\n\r");
    printf("Enter serial number >");
    char serial_number[EEPROM_FIELD_LENGTH];  //This holds the LED board serial number when the user enters it, init with spaces
    ReadString(serial_number, EEPROM_FIELD_LENGTH);
    //-----------------------------------
    printf("\n\r");
    printf("Enter date of test >");
    char date_of_test[EEPROM_FIELD_LENGTH];  //This holds the date the LED board was tested, when the user enters it, init with spaces
    ReadString(date_of_test, EEPROM_FIELD_LENGTH);
    //-----------------------------------
    printf("\n\r");
    printf("Enter time of test >");
    char time_of_test[EEPROM_FIELD_LENGTH];  //This holds the time the LED board was tested, when the user enters it, init with spaces
    ReadString(time_of_test, EEPROM_FIELD_LENGTH);
    //-----------------------------------
    printf("\n\r");
    printf("Enter current in milliamps >");
    char cal_const_1[EEPROM_FIELD_LENGTH];  //This holds the calibration constant from the test results, user enters, init with spaces
    ReadString(cal_const_1, EEPROM_FIELD_LENGTH); 
    //-----------------------------------

    uint16_t length = min(strlen(serial_number), EEPROM_FIELD_LENGTH);
    memcpy(&data[EEPROM_ADDR_SERIAL_NUM], "                ", EEPROM_FIELD_LENGTH);  //First you have to replace the field with spaces
    memcpy(&data[EEPROM_ADDR_SERIAL_NUM], serial_number, length);   //Then write in the new value

    length = min(strlen(date_of_test), EEPROM_FIELD_LENGTH);
    memcpy(&data[EEPROM_ADDR_DATE_OF_TEST], "                ", EEPROM_FIELD_LENGTH);  //First you have to replace the field with spaces
    memcpy(&data[EEPROM_ADDR_DATE_OF_TEST], date_of_test, length);    //Then write in the new value
    
    length = min(strlen(time_of_test), EEPROM_FIELD_LENGTH);
    memcpy(&data[EEPROM_ADDR_TIME_OF_TEST], "                ", EEPROM_FIELD_LENGTH);  //First you have to replace the field with spaces
    memcpy(&data[EEPROM_ADDR_TIME_OF_TEST], time_of_test, length);    //Then write in the new value
    
    length = min(strlen(cal_const_1), EEPROM_FIELD_LENGTH);
    memcpy(&data[EEPROM_ADDR_CAL_CONST1], "                ", EEPROM_FIELD_LENGTH);  //First you have to replace the field with spaces
    memcpy(&data[EEPROM_ADDR_CAL_CONST1], cal_const_1, length);     //Then write in the new value

    timeout = true;
    num_retries = 0;
    while (timeout && (num_retries++ < MAX_NUM_RETRIES))
    {
        WriteLEDBoardEEPROM(0, data, EEPROM_SIZE, &timeout);
    }
    printf("\r\nDone\r\n");
}


/*
 *  Prompts user for default LED current in milliamps and stores this in flash
 */
static void WriteLEDBoardDefaultCurrent(void)
{
    char data[EEPROM_SIZE];
    bool timeout = true; // Will discard timeout value in absence of way to recover
    const uint16_t MAX_NUM_RETRIES = 12;
    uint16_t num_retries = 0;
    while (timeout && (num_retries++ < MAX_NUM_RETRIES))
    {
        ReadLEDBoardEEPROM(data, &timeout);  //Get the string from the EEPROM, this will be modified with user entries
    }

    printf("\r\nEnter current in milliamps >");
    char cal_const_1[EEPROM_FIELD_LENGTH];  //This holds the calibration constant from the test results, user enters, init with spaces
    ReadString(cal_const_1, EEPROM_FIELD_LENGTH);  

    uint16_t length = min(strlen(cal_const_1), EEPROM_FIELD_LENGTH); 
    memcpy(&data[EEPROM_ADDR_CAL_CONST1], "                ", EEPROM_FIELD_LENGTH);  //First you have to replace the field with spaces
    memcpy(&data[EEPROM_ADDR_CAL_CONST1], cal_const_1, length);     //Then write in the new value

    timeout = true;
    num_retries = 0;
    while (timeout && (num_retries++ < MAX_NUM_RETRIES))
    {
        WriteLEDBoardEEPROM(0, data, EEPROM_SIZE, &timeout);
    }
    printf("\r\nDone\r\n");
}

