/*	Filename:  tachometer.c
 *	Date Created:  11/17/2018
 *	Company:  Valtronic, USA
 *	Project:   Lumitex OraGlo Control Board
 *	Project number:  DD400004
 *	Description:  Routines for implementing and using the fan tachometer
 */


#include <float.h>
#include <stdio.h>

#include "mcc_generated_files/mcc.h"
#include "tachometer.h"

#define FAN_RPM_NOMINAL_LOW  2000.0f
#define FAN_RPM_NOMINAL_HIGH 4000.0f

//Test Limits from DD400004-SDS1
#define SW_SPEC_REVA_7_6_7 0.25f

static volatile uint16_t fan_tach_rpm = 0;

/*  Used in conjunction with the RTC to detect situations where
 *  the tachometer is stalled, i.e. the fan has stopped moving.
 */
static bool g_tachometer_isr_triggered = false;


/* Description:  This function tests that the fan is connected and determines
 *               pass/fail.  This is done by measuring the RPM of the fan, 
 *               since the fan has a tachometer output that is input to the
 *               microcontroller.  The RPM of the Fan Tach is calculated in the
 *               ISR for the Input Capture module
 * Preconditions:  The Input Capture module needs to be initialized for
 *                 measuring the tachometer frequency.  This includes
 *                 specifying the clock source for the input capture counter
 *                 (timer).  This clock source most likely has to be one of
 *                 the other timers, such as timer1.
 * Arguments:  The RPM of the fan is returned in the rpm field
 *             message should allow for up to 255 characters.   Test failure
 *                details are to be placed here.
 * Returns:  false = Fail, true = Pass
 */
bool TestFan(int *rpm, char *message)
{
    // Calculate the lower and upper limits for the fan.  There is a spec limit
    // but also a window for nominal operation which is an attempt to
    // accommodate a wide array of fans,  not just one.
    const uint16_t Lower_Limit = (1.0f - SW_SPEC_REVA_7_6_7)*FAN_RPM_NOMINAL_LOW;
    const uint16_t Upper_Limit = (1.0f + SW_SPEC_REVA_7_6_7)*FAN_RPM_NOMINAL_HIGH;
    
    //Apply the limits to the measured value
    bool pass = ((fan_tach_rpm >= Lower_Limit) && (fan_tach_rpm <= Upper_Limit));
  
    *rpm = fan_tach_rpm;   //return the measured RPM
    sprintf(message, "Fan RPM Lower_Limit = %.3f  Upper_Limit = %.3f  ",
            (double)Lower_Limit, (double)Upper_Limit);
    
    return pass;
}

// To be called only by the RTC ISR
void DetectedStalledTachometer(void)
{
    /* Determine if tach has been triggered since the last
     * time the RTC ISR went off.  If so, set RPM to zero */
    if (!g_tachometer_isr_triggered)
    {
        fan_tach_rpm = 0;
    }
    g_tachometer_isr_triggered = false;
}


/* Description:  This ISR responds to the interrupt request from the IC1 module,
 *               after it has made 4 capture events.  Each capture event logs
 *               the number of ticks from a 16-bit timer value with a frequency
 *               of 16MHz prescaled by 64 -- equivalently, 250kHz.   The IC1
 *               timer/counter is driven with a 10kHz (hence 100uS periods)
 *               clock from Timer1.  The IC1 input capture module is set up as: 
 *                   Prescaler ICM = "every rising edge" and
 *                   ICI = "Every 4th event"
 *               With these settings, the logic analyzer confirms that the ISR
 *               is firing every 4th rising edge of the tachometer input.
 *               One capture event is equal to 1 period of the fan's tachometer
 *                   output signal

 * Preconditions:  The IC1 module and TMR1 module should be set up as described
 *                 above. These modules also need to be initialized and running.
 * Arguments:  void
 * Returns:  void
 */
void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC1Interrupt( void )    //Fan Tachometer ISR
{
    // clear the IC1 interrupt flag
    IFS0bits.IC1IF = 0; 
    g_tachometer_isr_triggered = true;   
    uint16_t fan_tach_value1 = IC1_CaptureDataRead();  //First in, First out, so Value1 occurred prior to Value2
    uint16_t fan_tach_value2 = IC1_CaptureDataRead();
    
    //When the input capture module is configured for:  ICM = every rising, ICI = every 4th event
    //then the difference calculated below is equal to 
    uint16_t fan_tach_difference = fan_tach_value2 - fan_tach_value1;
    
    // We are using fan from Sanyo Denki America, part number 109P0824M701.  It
    // specifies on page 530 of the catalog that the "Pulse sensor (Tach output
    // type) outputs two pulse waves per revolution of the fan.  The period of
    // the tach signal is equal to the difference times 0.000004 (4uS per count,
    // the period of the aforementioned input timer).  The reciprocal of this
    // gives us the frequency of the tach signal in cycles/second.  Multiplying
    // this value by 60 gives us the cycles/minute and then divide it by two
    // gives the fan speed in RPM.
    fan_tach_rpm = (25.0/(( (float)fan_tach_difference)*(0.0001)) ) * (60.0 / 2.0);
    
    //Clean out the buffer if there is any data left in there
    while (!IC1_IsCaptureBufferEmpty())
    {
        IC1_CaptureDataRead();
    }
}
