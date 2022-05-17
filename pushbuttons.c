/*	Filename:  pushbuttons.c
 *	Date Created:  11/09/2018
 *	Company:  Valtronic, USA
 *	Project:   Lumitex OraGlo Control Board
 *	Project number:  DD400004
 *	Description:  Routines for pushbutton sensing
 */

#include <stdio.h>
#include <string.h>

#define    FCY    16000000UL  // required by libpic30.h    
#include <libpic30.h>

#include "mcc_generated_files/mcc.h"
#include "pushbuttons.h"
#include "Support_Functions.h"

static volatile bool pushbutton[2] = { false, false };


/*  Returns button status, i.e. whether it was pressed or not.
 *  Automatically clears the button status if it has been pressed.
 */
bool PushbuttonPressed(PushButton_t button)
{
    bool status = pushbutton[button];
    if (pushbutton[button])
    {
        // Only set it to false if previously true.
        // We want to avoid a race condition.
        pushbutton[button] = false;
    }
    return status;
}


/*
 * Description:  This function tests the pushbutton by one of two methods.  The simple method just verifies
 *               that the pushbutton board is attached by verifying PUSHBUTTON_DETECTED is low.  The other method
 *               would only be run during manufacturing since it requires technician interaction to verify the 
 *               pushbuttons operate.
 * Preconditions:  The digital input for the pushbutton needs to be set up
 *                 for interrupts, whereas the other digital input simply
 *                 needs to be configured for reading.
 * Arguments:  TestType:  false = simple test, true = requires technician interaction
 *             Message should hold at at least 256 characters, it returns test results and status
 * Returns:  false = Fail, true = Pass
 */
bool TestPushbutton(bool interactive_test, char *message)
{
    bool pass = false;
    
    if (message != NULL)
    {
        strcpy(message, "Test Pushbuttons ");  //initialize the message string
    }
    if (interactive_test)
    {
        printf("\n\r Pushbutton Test \n\r");
        printf("Briefly push button 1 > ");
        while (!pushbutton[PUSHBUTTON_1]) 
        {
            __delay_ms(10);
        }
        if (pushbutton[PUSHBUTTON_1])
        {
            pass = true; 
            printf("Pass \n\r"); 
            pushbutton[PUSHBUTTON_1] = false; 
            pushbutton[PUSHBUTTON_2] = false;
        }
        // Pushbutton 2 is not mechanically installed in product, so skip
        // it in the interactive test.  The electronics are installed
        // though, so we can test those in the non-interactive test.
    }
    else
    {
        if (PUSHBUTTON_DETECTED_GetValue()==0)
        {
            if (message != NULL)
            {
                strcat(message, "Connectivity  " );
            }
            pass = true;
        }
    }
    return pass;
}



/* Interrupt service routine for the IOCI interrupt. */
void __attribute__ (( interrupt, no_auto_psv )) _IOCInterrupt ( void )
{
    //Description:  This ISR responds to the interrupt request from the two pushbutton inputs, when they have a 
    //              rising edge.  The ISR just sets a global variable indicating the rising event occurred.
    //Preconditions:  The two digital inputs (pushbuttons) need configured for interrupt on rising edge
    //Arguments:  void
    //Returns:  void

    if (IFS1bits.IOCIF == 1)
    {
        // Clear the flag
        IFS1bits.IOCIF = 0;
        // interrupt on change for group IOCFF
        if (IOCFFbits.IOCFF2 == 1)
        {
            IOCFFbits.IOCFF2 = 0;
            // Add handler code here for Pin - RF2
            pushbutton[PUSHBUTTON_1] = true;
        }
        if (IOCFFbits.IOCFF3 == 1)
        {
            IOCFFbits.IOCFF3 = 0;
            // Add handler code here for Pin - RF3
            pushbutton[PUSHBUTTON_2] = true;
        }
    }
}
