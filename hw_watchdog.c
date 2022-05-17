/*	Filename:  hw_watchdog.c
 *	Date Created:  11/19/2018
 *	Company:  Valtronic, USA
 *	Project:   Lumitex OraGlo Control Board
 *	Project number:  DD400004
 *	Description:  Hardware watchdog routines
 */

#include "mcc_generated_files/mcc.h"
#include "hw_watchdog.h"
#include "ledboard.h"

#include <stdio.h>
#include <string.h>

#define DEBOUNCE_COUNT (10) // This is used as the debounce counter to stabilize
                            // .. Mouthpiece Switch processing

/*
 * ----------------------- function declarations -------------------------------
 */
uint16_t UpdateMouthpieceStatus(void);

/*
 * ----------------------- local data ------------------------------------------
 */
uint16_t    g_MouthpieceDebouncCounter;
bool        g_PreviousMouthpieceSwitchStatus;

/* InitializeHW_WatchdogData
 * Description:     This function initializes any data associated with the 
 *                  hardware status routine which is periodically called by
 *                  the main loop.
 * Preconditions:   Global vars must be declared
 * Arguments:       None
 * Returns:         Void
 */
void InitializeHW_WatchdogData()
{
    g_MouthpieceDebouncCounter = 0;
}


/* Description:  This function reads all of the status bits from the hardware
 *               watchdog circuit and determines if any of them are indicating
 *               a failure.  If any of the bits are indicating a failure, then
 *               this function returns a fail.
 * Preconditions:  The digital I/O bits associated with the hardware watchdog
 *                 have been initialized.
 * Arguments:  The current status of the hardware watchdog is returned in the
 *                'status' field.
 *             'message' should allow for up to 255 characters. Test failure 
 *                details will be placed here
 *              'print_out' = true will print a more verbose output to the
 *                console, false = no output to console
 * Returns:  false = Fail, true = Pass
 */
bool TestHWWdog(uint16_t *status, char *message, bool print_out)
{
    static uint16_t num_consec_hw_detected_errors = 0;
     // Hold the bitwise pass/fail results for all (1=pass, 0=fail for each bit)
    uint16_t test_results = 0; 

    if (message != NULL)
    {
        sprintf(message, "  ");  //Initialize the message
    }
    
    //Build the status word from all of the various digital inputs
    *status = UpdateMouthpieceStatus();
    *status <<= 1;   //shift left
    *status |= TEMP_TOO_HIGH_GetValue();
    *status <<= 1;   //shift left
    *status |= HW_WDOG_TIMEOUT_GetValue();
    *status <<= 1;   //shift left
    *status |= ENABLE_HW_WDOG_GetValue();
    *status <<= 1;   //shift left
    *status |= THERAPY_ON_GetValue();
    *status <<= 1;   //shift left
    *status |= START_WDOG_GetValue();
    *status <<= 1;   //shift left
    *status |= !CURRENT_SOURCE_FAULT_GetValue();
    *status <<= 1;   //shift left
    *status |= HW_DETECTED_ERROR_GetValue();
    *status <<= 1;   //shift left
    *status |= CURRENT_TOO_HIGH_GetValue();
    *status <<= 1;   //shift left
    *status |= CURRENT_TOO_LOW_GetValue();
    *status <<= 1;   //shift left
    *status |= SW_WDOG_TIMEOUT_GetValue();
    
    // Build the overall test result and add in any applicable error messages. 
    // Note that most of the test result bitflags are inverted! That is,
    // they are set high when the corresponding bit within *status is low. 
    // This indicates the absence of the corresponding error condition.
    if (*status & MOUTHPIECE_ATTACHED)
    {
        test_results |= MOUTHPIECE_ATTACHED;  //pass if MP_ATTACHED
    } 
    else if (message != NULL)
    {
        strcat(message, "MP not attached,");
    }
    if (!(*status & THERMISTOR_TEMP_TOO_HIGH)) 
    {
        test_results |= THERMISTOR_TEMP_TOO_HIGH;  //pass if not TEMP_TOO_HIGH
    }
    else if (message != NULL)
    {
        strcat(message, "TEMP_TOO_HIGH,");
    }
    if (!(*status & HW_WDOG_TIMEOUT))
    {
        test_results |= HW_WDOG_TIMEOUT;  //pass if not HW_WDOG_TIMEOUT
    }
    else if (message != NULL)
    {
        strcat(message, "HW_WDOG_TIMEOUT,");
    }
    if (*status & ENABLE_HW_WDOG)
    {
        test_results |= ENABLE_HW_WDOG; //pass if ENABLE_HW_WDOG
    } 
    else if (message != NULL)
    {
        strcat(message, "HW_WDOG disabled,");
    }
    /* Only clear the THERAPY_ON bit in the test results if the
     * THERAPY_ON watchdog status bit is high and the LED current isn't
     * supposed to be on. */
    if (((*status & THERAPY_ON)==0) || LEDCurrentOn()) 
    {
        test_results |= THERAPY_ON; //pass if ENABLE_HW_WDOG
    } 
    else if (message != NULL)
    {
        strcat(message, "THERAPY_ON,");
    }
    test_results |= START_WDOG;  //pass whether START_WDOG is on or off
    
    if (!(*status & CURRENT_SOURCE_FAULT))
    {
        test_results |= CURRENT_SOURCE_FAULT;  //pass if not CURRENT_SOURCE_FAULT
    } 
    else if (message != NULL)
    {
        strcat(message, "CURRENT_SOURCE_FAULT,");
    }
    if (!(*status & HW_DETECTED_ERROR))
    {
        test_results |= HW_DETECTED_ERROR; //pass if not HW_DETECTED_ERROR
    } 
    else if (message != NULL)
    {
        strcat(message, "HW_DETECTED_ERROR,");
    }
    if (!(*status & CURRENT_TOO_HIGH))
    {
        test_results |= CURRENT_TOO_HIGH; //pass if not CURRENT_TOO_HIGH
    } 
    else if (message != NULL)
    {
        strcat(message, "CURRENT_TOO_HIGH,");
    }
    if (!(*status & CURRENT_TOO_LOW))
    {
        test_results |= CURRENT_TOO_LOW;//pass if not CURRENT_TOO_LOW
    }  
    else if (message != NULL)
    {
        strcat(message, "CURRENT_TOO_LOW,");
    }
    if (!(*status & SW_WDOG_TIMEOUT)) 
    {
        test_results |= SW_WDOG_TIMEOUT; //pass if not SW_WDOG_TIMEOUT
    } 
    else if (message != NULL)
    {
        strcat(message, "SW_WDOG_TIMEOUT,");
    }

    if (print_out)
    {
        printf("\n\rMOUTHPIECE_ATTACHED   \t%d", ((*status & MOUTHPIECE_ATTACHED) >> 10));
        printf("\n\rTEMP_TOO_HIGH         \t%d", ((*status & THERMISTOR_TEMP_TOO_HIGH) >> 9));
        printf("\n\rHW_WDOG_TIMEOUT       \t%d", ((*status & HW_WDOG_TIMEOUT) >> 8));
        printf("\n\rENABLE_HW_WDOG        \t%d", ((*status & ENABLE_HW_WDOG) >> 7));
        printf("\n\rTHERAPY_ON            \t%d", ((*status & THERAPY_ON) >> 6));
        printf("\n\rSTART_WDOG            \t%d", ((*status & START_WDOG) >> 5));
        printf("\n\rCURRENT_SOURCE_FAULT  \t%d", ((*status & CURRENT_SOURCE_FAULT) >> 4));
        printf("\n\rHW_DETECTED_ERROR     \t%d", ((*status & HW_DETECTED_ERROR) >> 3));
        printf("\n\rCURRENT_TOO_HIGH      \t%d", ((*status & CURRENT_TOO_HIGH) >> 2));
        printf("\n\rCURRENT_TOO_LOW       \t%d", ((*status & CURRENT_TOO_LOW) >> 1));
        printf("\n\rSW_WDOG_TIMEOUT       \t%d", ((*status & SW_WDOG_TIMEOUT) >> 0));
    }
    
    if (test_results & HW_DETECTED_ERROR)
    {
        /* Require enough consecutive occurrences of hardware detected
         * errors before we act on it.  This amounts to an 'unknown' error
         * and can be detected before the actual cause of the error is
         * determined.  Preferably, we want to know the latter. */
        const uint16_t MIN_CONSEC_HW_DETECTED_ERRORS = 2;
        if (++num_consec_hw_detected_errors < MIN_CONSEC_HW_DETECTED_ERRORS)
        {
            test_results &= ~HW_DETECTED_ERROR;
        }
    }
    else
    {
        num_consec_hw_detected_errors = 0;
    }

    const uint16_t THERAPY_OFF_PASS_CONDITIONS = (MOUTHPIECE_ATTACHED |
                        THERMISTOR_TEMP_TOO_HIGH | HW_WDOG_TIMEOUT |
                        ENABLE_HW_WDOG | START_WDOG | CURRENT_SOURCE_FAULT |
                        HW_DETECTED_ERROR | CURRENT_TOO_HIGH |
                        CURRENT_TOO_LOW | SW_WDOG_TIMEOUT);
    const uint16_t THERAPY_ON_PASS_CONDITIONS =
                        (THERAPY_OFF_PASS_CONDITIONS | THERAPY_ON);
    uint16_t pass_conditions = (LEDCurrentOn() ? THERAPY_ON_PASS_CONDITIONS :
                                    THERAPY_OFF_PASS_CONDITIONS);
    return ((test_results & pass_conditions) == pass_conditions);
}


/* UpdateMouthpieceStatus
 * Description:     This function debounces the Mouthpiece Switch.
 * Preconditions:   Global vars must be declared and initialized
 * Arguments:       None
 * Returns:         The debounced switch status (essentially a bool).
 */
uint16_t UpdateMouthpieceStatus(void)
{
    uint16_t switchStatus;
    
    switchStatus = MP_ATTACHED_GetValue();

    // If first time through, then simply get the switch status and exit
    // with a detached status.
    if (g_MouthpieceDebouncCounter == 0)        // first time after power up
    {
        g_PreviousMouthpieceSwitchStatus = switchStatus;
        ++g_MouthpieceDebouncCounter;
    }
    else    // We must be stabilizing the input
    {
        // If the switch has changed status, then we are debouncing
        if (g_PreviousMouthpieceSwitchStatus != switchStatus)
        {
            // If we have waited long enough, use the new switch status.
            if (++g_MouthpieceDebouncCounter > DEBOUNCE_COUNT)
            {
                g_PreviousMouthpieceSwitchStatus = switchStatus;
                g_MouthpieceDebouncCounter = 1;
            }
        }
        else    // The switch has not changed status.
        {
            g_MouthpieceDebouncCounter = 1;
        }
    }
    
    return (g_PreviousMouthpieceSwitchStatus);
    
//    if (g_PreviousMouthpieceSwitchStatus)   // Active LOW
//    {
//        *status &= ~MOUTHPIECE_ATTACHED;
//    }
//    else
//    {
//        *status |= MOUTHPIECE_ATTACHED;
//    }
}
