//	Filename:  main.c
//	Author:  Paul Wittibschlager
//	Date Created:  08/27/2018
//	Company:  Valtronic, USA
//	Project:   Lumitex OraGlo Control Board
//	Project number:  DD400004
//	Description:  This file contains the main function for the OraGlo control board MCU


#include "mcc_generated_files/mcc.h"
#include "Test_Functions.h"
#include "Support_Functions.h"
#include "UI_Functions.h"
#include "rtc.h"
#include "rtcc.h"
#include "lcd.h"
#include "swi.h"
#include "hw_watchdog.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <float.h>


//----------------------------------------------

/*
                         Main application
 */
int main(void)
{
    _SWDTEN = 1;  //turn on watchdog
    // initialize the device
    SYSTEM_Initialize();
    RTCCInit();
    SWIInit();
    InitializeUserInterface();
    InitializeHW_WatchdogData();
    
    /* Enable DMA. For some reason, we can't just initialize the other
       DMA register flags once at startup.  They have to be set just
       prior to use, even when their values shouldn't be changing. */
    DMACONbits.DMAEN = 1;
    
    PrintMenu();  //Print main menu to console
    
    //Main while loop that implements the user interface state machine, error checking, BIST
    while (true)
    {
        TEST_LED_Toggle();
        //Determine if a phase change is required and update the LCD and RGB LED accordingly
        UpdateUIStateMachine();
        
        /* Quickly check the status bit before attempting to perform any
           user interface operations. This will ensure that the software
           watchdog timer doesn't go off. */
        if (UART1_DataReady)
        {
            /* Use UART1_Read() instead of GetChar().  GetChar() prints the
             * entered character out, and this will interfere with batch
             * scripts used for updating the images. */
            TestInterface(UART1_Read());
        }
        
        ClrWdt();  //Clear the watchdog timer, do this only here, nowhere else
    }

    return -1;
}


/**
 End of File
*/
