/*	Filename:  rtc.c
 *	Date Created:  11/16/2018
 *	Company:  Valtronic, USA
 *	Project:   Lumitex OraGlo Control Board
 *	Project number:  DD400004
 *	Description:   Real-time clock (RTC) routines
 */

#include "mcc_generated_files/mcc.h"
#include "rtc.h"
#include "screensaver.h"
#include "tachometer.h"

#include <limits.h>
#include <stdio.h>

static const uint16_t COUNTDOWN_START_VALUE = 3;

volatile static uint16_t g_generic_countdown_timer = 0;
volatile static bool g_generic_countdown_timer_elapsed = false;

// Variables used by operation state
volatile bool g_time_for_display_update = false;
volatile static uint16_t g_operation_state_countdown_timer = 0;
volatile static uint16_t g_initial_operation_state_countdown_timer;
/*  Number of seconds to be displayed in the initial clock dial display.
 *  Let N = the amount of therapy time in seconds.  If N is an even multiple
 *  of 60, then g_num_seconds_first_interval should be set to 60, i.e.
 *  a complete clock dial should be displayed.  Otherwise, we should set
 *  it to N div 60, indicating that a partial dial should be shown. */
volatile static uint16_t g_num_seconds_first_interval; 

//    Used in testing LED current. We want to ignore LED current errors
//    when the countdown has been newly started/restarted, i.e.
//    when the LED power has just been turned on.
volatile static uint16_t g_restart_countdown = 0;

// Indicates when we should perform BIST, except when in
// the operation state. See comments in ISR below.
volatile bool g_time_to_perform_bist = false;

/*  The following is used in two situations: when the operation state timer
 *  is paused partway through its countdown and when it has not been started
 */
volatile static bool g_operation_state_timer_paused = true; 

// Indicates whether screensaver could come on or is in use
//   -- not necessarily whether it is currently active
volatile static bool g_screensaver_timer_enabled = false;
// timer used in implementing screen saver
volatile static uint16_t g_screensaver_timer = 0;
// The number of timer counts (0.5 periods) before we turn screensaver on
volatile static uint16_t g_timer_counts_before_screensaver = 0;
// The number of timer counts (0.5 periods) before we turn the display dark
volatile static uint16_t g_timer_counts_before_blank_screen = 0;

/* --- Generic countdown timer routines.  Used for implementing a
 *     general-purpose timer with one-second resolution.  --- */

void StartGenericCountdownTimer(uint16_t num_seconds)
{
    g_generic_countdown_timer = num_seconds * 2; // Convert seconds to 500ms periods
}

void ClearGenericCountdownTimer(void)
{
    g_generic_countdown_timer = 0;
}

bool GenericCountdownTimerElapsed(void)
{
    bool finished = g_generic_countdown_timer_elapsed;
    if (g_generic_countdown_timer_elapsed)
    {
        // Clear variable so that we only detect the countdown finish once
        g_generic_countdown_timer_elapsed = false;
    }
    return finished;
}

void WaitForGenericCountdownTimer(uint16_t restart_value)
{
    while (!g_generic_countdown_timer_elapsed) ;
    g_generic_countdown_timer_elapsed = false;  // Reset
    if (restart_value > 0)
    {
        g_generic_countdown_timer = restart_value;
    }
}

/* --- End of generic countdown timer routines --- */

void StartOperationStateCountdown(uint16_t num_seconds)
{
    g_time_for_display_update = false;
    // Multiply number of seconds by two since the RTC
    // interrupt is triggered twice per second.  Add 1 so that
    // we start at (num_seconds * 2) on the next interrupt.
    g_operation_state_countdown_timer = num_seconds * 2 + 1;
    // The following is used in generating the proper tick mark images
    g_initial_operation_state_countdown_timer = g_operation_state_countdown_timer;
    g_num_seconds_first_interval = ((num_seconds % 60) ? (num_seconds % 60) : 60);
    g_operation_state_timer_paused = false; 
    g_restart_countdown = COUNTDOWN_START_VALUE;
}


void PauseOperationStateCountdown(void)
{
    g_operation_state_timer_paused = true; 
}


void ResumeOperationStateCountdown(void)
{
    g_operation_state_timer_paused = false; 
    g_restart_countdown = COUNTDOWN_START_VALUE;
}

/*
 *   *first_minute should be set to 'true' if we are within the first
 *   (not necessarily whole) minute of the operation time.  For example,
 *   if the operation time is set to five minutes, then *first_minute
 *   should be set to 'true' for the first sixty seconds.  If it is set to
 *   n minutes and m seconds, where n and m are both positive integers
 *   with m < 60, then *first_minute should be set to 'true' for the
 *   first m seconds.
 */
bool TimeToUpdateDisplay(uint16_t *timer_val, bool *first_minute)
{
    // Grab the current value of g_time_for_display_update, lest it
    // change partway through the execution of this function
    bool status = g_time_for_display_update;
    if (status)
    {
        *timer_val = g_operation_state_countdown_timer/2;
        g_time_for_display_update = false;
        *first_minute = ((g_initial_operation_state_countdown_timer/2 - *timer_val) < g_num_seconds_first_interval);
    }
    return status; 
}

bool TimeToPerformBIST(void)
{
    bool status =  g_time_to_perform_bist;
    if (status)
    {
        g_time_to_perform_bist = false;
    }
    return status; 
}

uint16_t GetOperationStateTimer(void)
{
    return g_operation_state_countdown_timer/2;
}

bool OperationCountDownNewlyRestarted(void)
{
    return (g_restart_countdown > 0);
}

void ResetScreenSaverTimer(uint16_t time_before_screensaver_starts,
                           uint16_t max_screensaver_time)
{
    g_screensaver_timer_enabled = true;
    g_screensaver_timer = 0;
    g_timer_counts_before_screensaver = time_before_screensaver_starts*2;
    g_timer_counts_before_blank_screen = g_timer_counts_before_screensaver +
                                         max_screensaver_time * 2;
}

void StopScreenSaverTimer(void)
{
    g_screensaver_timer_enabled = false;
}

/* Description:  This ISR responds to the interrupt from the RTCC (real time clock and calendar) 
 *              The interrupt should occur every 500mS
 * Preconditions:  The RTCC module should be set up for interrupting every 500mS
 * Arguments:  void
 * Returns:  void
 */
void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _RTCCInterrupt( void )
{
    IFS3bits.RTCIF = false;
    
    if (g_generic_countdown_timer != 0)
    {
        if (--g_generic_countdown_timer == 0)
        {
            g_generic_countdown_timer_elapsed = true;
        }
    }
    if (!g_operation_state_timer_paused && (g_operation_state_countdown_timer > 0))
    {
        if ((--g_operation_state_countdown_timer & 0x0001) == 0)
        {
            g_time_for_display_update = true;
        }
        if (g_restart_countdown > 0)
        {
            g_restart_countdown--;
        }
    }
    
    static uint16_t counter = 0;
    if ((counter++ & 0x0001) == 0)
    {
        /* Once per second, indicate that it's time to perform
         * built-in self-test (BIST).  Note that this is the same
         * frequency at which we indicate that it's time to update
         * the display when we're in the operation state.  While
         * in the operation state then, we use the display timer
         * instead of this one. */
        g_time_to_perform_bist = true;
    }
    
    if (g_screensaver_timer_enabled && (g_screensaver_timer < UINT_MAX))
    {
        if (++g_screensaver_timer == g_timer_counts_before_screensaver)
        {
            ActivateScreenSaver(true);
        }
        else if (g_screensaver_timer == g_timer_counts_before_blank_screen)
        {
            ActivateBlankScreen();
        }
    }
    
    // We use the RTC to determine the tach ISR isn't being triggered
    DetectedStalledTachometer();
}

