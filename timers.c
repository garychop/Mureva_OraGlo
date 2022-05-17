/*	Filename:  timers.c
 *	Date Created:  08/27/2018
 *	Company:  Valtronic, USA
 *	Project:   Lumitex OraGlo Control Board
 *	Project number:  DD400004
 *	Description:  Contains routines that make use of the internal timers
 * 
 *  This module uses timer2 to implement a general-purpose software timer
 *  In addition, timer2 is also used for blinking the RGB LED
 */

#include "mcc_generated_files/mcc.h"
#include "UI_Functions.h"
#include "ledboard.h"
#include "rgbled.h"

// Conversion factor from milliseconds to timer counts
static const uint16_t MS_TO_TIMER_CNTS = 40;

static volatile bool timer_done = false;

/* Set to 'true' when using TimerDelay(), StartTimer() and
   related routines. Set to 'false' if using the routines for
   blinking LEDs. */
static bool multipurpose_timer = false;

static uint16_t g_red1;
static uint16_t g_green1;
static uint16_t g_blue1;
static uint16_t g_time1;
static uint16_t g_red2;
static uint16_t g_green2;
static uint16_t g_blue2;
static uint16_t g_time2;
static uint16_t g_blink_timer_cnt = 0;

static bool g_led_blinking_enabled = false;

// delay time is expressed in units of 0.025 milliseconds
void TimerDelay(uint16_t time)
{
    multipurpose_timer = true;
    IPC1bits.T2IP = 1;  // Interrupt priority TBD
    TMR2 = 0x0000;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    PR2 = time;
    T2CONbits.TON = 1;
    while (!timer_done) ;
    timer_done = false;
    IEC0bits.T2IE = 0;
}

void StartTimer(uint16_t time)
{
    multipurpose_timer = true;
    IPC1bits.T2IP = 1; // Interrupt priority TBD
    TMR2 = 0x0000;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    PR2 = time;
    T2CONbits.TON = 1;
}

void WaitForTimer(void)
{
    while (!timer_done) ;
    timer_done = false;
    IEC0bits.T2IE = 0;
}

bool TimerDone(void)
{
    bool done = false;
    if (timer_done)
    {
        done = true;
        timer_done = false;
        IEC0bits.T2IE = 0;
    }
    return done;
}

void ClearTimer(void)
{
    timer_done = true;
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 0;
}

void SetLedBlinkPattern(uint16_t red1, uint16_t green1, uint16_t blue1,
                        uint16_t time1, uint16_t red2, uint16_t green2,
                        uint16_t blue2, uint16_t time2)
{
    multipurpose_timer = false;
    g_red1 = red1;
    g_green1 = green1;
    g_blue1 = blue1;
    g_time1 = time1;
    g_red2 = red2;
    g_green2 = green2;
    g_blue2 = blue2;
    g_time2 = time2;
    g_blink_timer_cnt = 0;
    SetLedRGB(g_red1, g_green1, g_blue1);
    g_led_blinking_enabled = true;
    IPC1bits.T2IP = 1; 
    TMR2 = 0x0000;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    PR2 = time1*MS_TO_TIMER_CNTS;
    T2CONbits.TON = 1;
}

void DisableLedBlinking(void)
{
    g_led_blinking_enabled = false;
}

// Invoked by the Timer2 ISR's callback function
void Timer2Handler(void)
{
    if (multipurpose_timer)
    {
        ClearTimer();
    }
    else if (g_led_blinking_enabled)
    {
        T2CONbits.TON = 0;
        if (++g_blink_timer_cnt & 0x0001)
        {
            SetLedRGB(g_red2, g_green2, g_blue2);
            PR2 = g_time2*MS_TO_TIMER_CNTS;
        }
        else
        {
            SetLedRGB(g_red1, g_green1, g_blue1);
            PR2 = g_time1*MS_TO_TIMER_CNTS;
        }
        T2CONbits.TON = 1;
    }
}
