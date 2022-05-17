/*	Filename:  rgbled.c
 *	Date Created:  11/13/2018
 *	Company:  Valtronic, USA
 *	Project:   Lumitex OraGlo Control Board
 *	Project number:  DD400004
 *	Description:  Routines for controlling the LED ring (the RGB LED(s))
 */


#include "mcc_generated_files/mcc.h"
#include "Support_Functions.h"
#include "lcd.h"
#include "timers.h"
#include "rgbled.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define    FCY    16000000UL  // required by libpic30.h    
#include <libpic30.h>

/* Description:  This function writes the specified colors to the RGB LED.
 *               The function will allow for blinking between the two colors. 
 *               Here, color1 is on for time1 (milliSeconds) and color2 is on
 *               for time2.  If no blinking is desired, then set time2 to zero.
 *               This function also allows for a predefined color to be written
 *               to the RGB LED for testing purposes.
 * Preconditions:  The OCC (Output Compare) modules need to be initialized
 *                 for all 3 (red, green, blue) digital outputs
 *                 The timer module needs to be initialized to allow blinking
 * Arguments:  color1 and color2 define an RGB color according to these bit
 *               assignments:
 *             Color(7:0)   = Red    (0 to 255)
 *             Color(15:8)  = Green  (0 to 255)
 *             Color(23:16) = Blue   (0 to 255)
 * 
 *             color is a character string with the definitions shown below,
 *               generally used for testing purposes only
 *             If color = "CUSTOM" then the arguments (color1, time1, color2,
 *               time2) define the color/blinking
 * 
 *             Time is in milliseconds and can be anywhere from 0 to 16000
 * 
 * Returns:  void
 */  
void WriteRGBLED(led_color_t color, uint32_t color1, uint32_t time1, uint32_t color2, uint32_t time2)
{
    const uint16_t FULL = 32768;  //This is the value for the OC Timer to turn the pulse on full (100% duty cycle)
    const uint16_t HALF = 16384;  //This is the value for the OC timer to turn the pulse on half (50% duty cycle)
    const uint16_t SCALE = 128;   //This is the scaler amount (multiplier) for color intensity from 0 to 255, SCALE*256 = FULL
   
    uint16_t red1=0, green1=0, blue1=0;  //Default color is black (off)
    uint16_t red2=0, green2=0, blue2=0;  //Default color is black (off)
    
    switch (color)
    {
        case LED_RED:
            red1 = HALF;
            green1 = 0;
            blue1 = 0;
            red2 = HALF;
            green2 = 0;
            blue2 = 0;
            break;
        case LED_GREEN:
            red1 = 0;
            green1 = HALF;
            blue1 = 0;
            red2 = 0;
            green2 = HALF;
            blue2 = 0;
            break;
        case LED_BLUE:
            red1 = 0;
            green1 = 0;
            blue1 = HALF;
            red2 = 0;
            green2 = 0;
            blue2 = HALF;
            break;
        case LED_YELLOW:
            red1 = HALF;
            green1 = HALF;
            blue1 = 0;
            red2 = HALF;
            green2 = HALF;
            blue2 = 0;
            break;
        case LED_WHITE:
            red1 = HALF;
            green1 = HALF;
            blue1 = HALF;
            red2 = HALF;
            green2 = HALF;
            blue2 = HALF;
            break;
        case LED_BLACK:
            red1 = 0;
            green1 = 0;
            blue1 = 0;
            red2 = 0;
            green2 = 0;
            blue2 = 0;
            break;
        case LED_TEST:
            red1 = FULL;
            green1 = FULL;
            blue1 = FULL;
            red2 = FULL;
            green2 = FULL;
            blue2 = FULL;
            break;
        case LED_CUSTOM:
           red1   = SCALE*((color1 & 0x000000FF) >> 0);
           green1 = SCALE*((color1 & 0x0000FF00) >> 8);
           blue1  = SCALE*((color1 & 0x00FF0000) >> 16);

           red2   = SCALE*((color2 & 0x000000FF) >> 0);
           green2 = SCALE*((color2 & 0x0000FF00) >> 8);
           blue2  = SCALE*((color2 & 0x00FF0000) >> 16);
           break;
        default: // Should never happen
            break;
    }

    OC1_SecondaryValueSet(FULL);
    OC2_SecondaryValueSet(FULL);
    OC3_SecondaryValueSet(FULL);

    if (time2 == 0)
    {
        DisableLedBlinking();
        SetLedRGB(red1, green1, blue1);
    }
    else
    {
        SetLedBlinkPattern(red1, green1, blue1, time1,
                           red2, green2, blue2, time2);
        // LED blink pattern is established by RTC ISR
    }
}

/*  Sets PWM waveforms to generate specified color combination
 */
void SetLedRGB(uint16_t red, uint16_t green, uint16_t blue)
{
    OC1_PrimaryValueSet(red);   
    OC2_PrimaryValueSet(green);
    OC3_PrimaryValueSet(blue);   
}

/*
 * Description:  This function tests that the RGB LED is connected and
 *                 determines pass/fail, RED LED will blink
 *               Technically, only the RED LED is tested.  
 * Preconditions:  The digital IO must be configured for the RGB LED.  There
 *                 can be no "hardware detected error"   If the hardware
 *                 watchdog is currently driving HW_DETECTED_ERROR = 1 then
 *                 this test will automatically fail.
 * Arguments:  'message' should allow for up to 255 characters.  Test failure
 *                 details will be placed here.
 *             'post' is 'true' if this test is being performed as part of POST.
 *                 During POST, we want to synchronize when the RGB LED and
 *                 the LCD are turned on and off.  (This results in some
 *                 coupling between this and the LCD test routine.  This
 *                 is an acceptable trade-off for the sake of performance and
 *                 visual aesthetics, though.)
 * Returns:  false = Fail, true = Pass
 */
bool TestRGBLED(char *message, bool post)
{
    strcpy(message, "Test RGB LED ");  //initialize the message string
    
    //Tests to make sure that there is no hardware detected error
    bool no_hw_detected_error = (HW_DETECTED_ERROR_GetValue() != 1);
    if (!no_hw_detected_error)
    {
        strcat(message, "Hardware Detected Error, can't run test.  ");
    }
            
    WriteRGBLED(LED_TEST, 0, 0, 0, 0);   //Turn on the Red LED in test mode, which means there is no modulation of the RED LED
    __delay_ms(12); 
    
    //If the RED_LED_DETECTED# is low when the RED LED is on, this is a pass
    bool red_led_detected = (RED_LED_DETECTED_GetValue() == 0);
    if (!red_led_detected)
    {
        strcat(message, "RED LED not detected");
    }
            
    if (!post)
    {
        /* During POST, we want the LCD and the red LED to turn on and off
         * simultaneously, just for aesthetics.  So if POST is being execugted,
         * don't turn the RGB LED off just yet. */
        WriteRGBLED(LED_BLACK, 0, 0, 0, 0);  
        __delay_ms(12); 
    }
    return no_hw_detected_error && red_led_detected;   
}

/*
 * Description:  This function tests all of the individual lights in the
 *               product:  the RGB LED and LCD backlight.  It does this by
 *               turning them on for two seconds.  The test can be run in one 
 *               of two modes.  The simple mode just flashes all of the lights
 *               for two seconds.  The interactive mode requires a technician
 *               to confirm that the light works.
 * Preconditions:  The output compare modules that drive the digital outputs
 *                 to the RGB LED need to be initialized.
 *                 The SPI interface for the LCD needs to be initialized.  
 *                 Finally, the LCD needs to be initialized.
 * Arguments:  interactive_test:  true if it requires technician interaction;
 *                 false otherwise
 * Returns:  void
 */
bool TestLights(bool interactive_test)
{
    WriteRGBLED(LED_RED, 0, 0, 0, 0);
    bool pass_r = false;
    if (interactive_test)
    {
        printf("\n\r Is the LED glowing Red?  Enter Y to accept or N to reject >");
        pass_r = GetYesOrNo();
    }
    __delay_ms(2500);
    //-------------------------------------------
    WriteRGBLED(LED_GREEN, 0, 0, 0, 0);
    bool pass_g = false;
    if (interactive_test)
    {
        printf("\n\r Is the LED glowing Green?  Enter Y to accept or N to reject >");
        pass_g = GetYesOrNo();
    }
    __delay_ms(2500);
    //-------------------------------------------
    WriteRGBLED(LED_BLUE, 0, 0, 0, 0);
    bool pass_b = false;
    if (interactive_test)
    {
        printf("\n\r Is the LED glowing Blue?  Enter Y to accept or N to reject >");
        pass_b = GetYesOrNo();
    }
    __delay_ms(2500);
    WriteRGBLED(LED_BLACK, 0, 0, 0, 0);
    //-------------------------------------------
    TurnBacklightOff(); 
    DisplayTestImage();
    TurnBacklightOn();
    bool pass_l = false;
    if (interactive_test)
    {
        printf("\n\r Is the LCD test image visible?  Enter Y to accept or N to reject >");
        pass_l = GetYesOrNo();
    }
    
    return (interactive_test ? (pass_r && pass_g && pass_b && pass_l) : true);
}

