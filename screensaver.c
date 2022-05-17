/*	Filename:  screensaver.c
 *	Date Created:  12/19/2018
 *	Company:  Valtronic, USA
 *	Project:   Lumitex OraGlo Control Board
 *	Project number:  DD400004
 *	Description:  Routines that implement screensaver mode
 */


#include <stdio.h>

#define    FCY    16000000UL  // required by libpic30.h    
#include <libpic30.h>

#include "mcc_generated_files/mcc.h"
#include "screensaver.h"
#include "rtc.h"
#include "lcd.h"


static uint32_t g_screensaver_image_id;

// Indicates whether screensaver is currently active. This includes
//   situations where the screen has been blanked due to inactivity
volatile static bool g_screensaver_active = false;
// Indicates when screen has been blanked due to inactivity
volatile static bool g_time_to_blank_screen = false;

// This is used to determine when it is time to update the screen
volatile static uint16_t g_screensaver_counter = 0;

static uint16_t g_time_before_screensaver_starts;
static uint16_t g_max_screensaver_time;
static uint16_t g_screensaver_refresh_interval;
static bool g_screen_blanked = false;

void ScreenSaverInit(uint32_t image_id, uint16_t time_before_screensaver_starts,
                     uint16_t max_screensaver_time, uint16_t screensaver_refresh_interval)
{
    g_screensaver_image_id = image_id;
    g_time_before_screensaver_starts = time_before_screensaver_starts;
    g_max_screensaver_time = max_screensaver_time;
    //The multiplier of 20 was determined by empirical testing
    g_screensaver_refresh_interval = 20*screensaver_refresh_interval;
}

/*  
 *  This function should be called from the user interface routine.
 *  It starts the screen saver timer and informs this module that we
 *  are in a state where the screen saver could become active once 
 *  triggered by the timer.
 */
void EnableScreenSaver(void)
{
    g_screensaver_active = false;  // Will become active when triggerd by timer
    g_time_to_blank_screen = false; // Also to be triggered by timer
    ResetScreenSaverTimer(g_time_before_screensaver_starts,
                          g_max_screensaver_time);
}

void DisableScreenSaver(void)
{
    g_screensaver_active = false;
    g_time_to_blank_screen = false;
    g_screen_blanked = false;
    StopScreenSaverTimer();
}


/*
 *  This function should be called by the real-time clock module.
 *  It informs this module that the timer has triggered and
 *  turned the screensaver on.   
 */
void ActivateScreenSaver(bool enable)
{
    g_screensaver_active = true;
}

/*
 *  This function should likewise be called by the real-time clock module.
 *  It informs this module that the timer has triggered, indicating that
 *  it's time to turn the screen blank.
 */
void ActivateBlankScreen(void)
{
    g_time_to_blank_screen = true;
}


bool ScreenSaverIsActive(void)
{
    return g_screensaver_active;
}

bool ScreenShouldBeBlank(void)
{
    return g_time_to_blank_screen;
}


/*
 *   This function is to be called periodically when the screen saver
 *   is active
 */
void ExecuteScreenSaverMode(void)
{
    if (!g_screen_blanked && g_time_to_blank_screen)
    {
        BlankOutDisplay(true);
        g_screen_blanked = true;
    }
    else
    {
         
        if (++g_screensaver_counter >= g_screensaver_refresh_interval)
        {
            // Update display here
            BlankOutDisplay(false);
            DisplayImageAtRandomLocation(g_screensaver_image_id);
            g_screensaver_counter = 0;
        }
    }   
    /*
     *  Add some delay so that we can use the counter to update the
     *  display at a reasonably consistent rate.  We should keep this
     *  delay reasonably small though, so that we can respond quickly
     *  to events that turn the screensaver off.
     */
    __delay_ms(50);
}

