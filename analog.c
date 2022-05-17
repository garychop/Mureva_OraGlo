/*	Filename:  analog.c
 *	Date Created:  11/13/2018
 *	Company:  Valtronic, USA
 *	Project:   Lumitex OraGlo Control Board
 *	Project number:  DD400004
 *	Description:  ADC and DAC routines
 */

#include "mcc_generated_files/mcc.h"
#include "Support_Functions.h"
#include "analog.h"

#define    FCY    16000000UL  // required by libpic30.h    
#include <libpic30.h>

#include <stdio.h>

#define ADC_AVERAGES 100

static volatile int g_adc1_total_accum = 0;  

static volatile uint32_t g_analog_ch8_accum = 0;
static volatile uint32_t g_analog_ch8_average = 0;
static volatile float g_analog_ch8 = 0;

static volatile uint32_t g_analog_ch9_accum = 0;
static volatile uint32_t g_analog_ch9_average = 0;
static volatile float analog_ch9 = 0;

static volatile uint32_t g_analog_ch10_accum = 0;
static volatile uint32_t g_analog_ch10_average = 0;
static volatile float g_analog_ch10 = 0;

static volatile uint32_t g_analog_ch11_accum = 0;
static volatile uint32_t g_analog_ch11_average = 0;
static volatile float g_analog_ch11 = 0;
static volatile float g_analog_ch11_offset = 0;

static volatile uint32_t g_analog_ch12_accum = 0;
static volatile uint32_t g_analog_ch12_average = 0;
static volatile float g_analog_ch12 = 0;


float GetADCReadingCh8(void)
{
    return g_analog_ch8;
}

float GetADCReadingCh9(void)
{
    return analog_ch9;
}

float GetADCReadingCh10(void)
{
    return g_analog_ch10;
}

float GetADCReadingCh11(void)
{
    return g_analog_ch11;
}

void ClearADCCh11Offset(void)
{
    g_analog_ch11_offset=0;
}

void SetADCCh11Offset(void)
{
    g_analog_ch11_offset = g_analog_ch11;
}

float GetADCReadingCh12(void)
{
    return g_analog_ch12;
}


/* Description:  This function writes a value to the DAC that controls the current source.
 *               Simulation results have showed that the voltage should change at a rate that is
 *               no faster than 36mV/mS.  So for this function to operate most efficiently, we need
 *               to know the current value of the DAC so that we can ramp the new value.
 * Preconditions:  The SPI module that interfaces to the DAC needs to be initialized
 *                 The ADC acquisition system has been setup and running, providing the averaged, analog input values
 *                 The digital IO bits associated with the hardware watchdog have been initialized.
 *                 POST and BIST should have been run and passed, to verify over all hardware health
 * Arguments:  ValueFrom is the current value of the DAC.  This can be found by measuring the LED current and then
 *             calculating the value of the DAC needed to get that current
 *             ValueTo is the new value for the DAC.
 *             print_out:  'true' to print to test interface UART, 'false' otherwise
 * Returns:  void
 */

void WriteDAC(float value_from, float value_to, bool print_out)
{
    if (print_out)
    {
        printf("\n\rDAC_Write - make sure to check timing wrt new Delay function");
    }
    
    float a = value_from;  
    float b = value_to;
    
    //place bounds on the inputs
    if (a < 0)
    {
        a = 0;
    }
    if (a > ADC_REF)
    {
        a = ADC_REF;
    }
    a = (a/ADC_REF)*1024;   //convert to DAC code value
    long ka = a;          
    long k = a;
    
    //place bounds on the inputs
    if (b<0)
    {
        b=0;
    }
    if (b>ADC_REF)
    {
        b=ADC_REF;
    }
    b = (b/ADC_REF)*1024;   //convert to DAC code value
    long kb = b; 
    
    if (print_out) 
    {
        printf("\n\r a=%.3f b=%.3f", (double)a, (double)b);
        printf("\n\r k=%ld ka=%ld kb=%ld", k, ka, kb);
    }
    
    IO_RD3_SetLow(); //  IO_RF5_SetHigh();   //Enable DAC, disable RFID
    SPI1_Exchange32bit( 0x00700000 );  //Write to DAC, this writes the command code and selects the external reference
    IO_RD3_SetHigh();
    __delay_us(12);
    
    if (kb > ka)   //increment k to go from a to b
    {
        while (k <= kb)
        {
            long i=64*k + 0x00300000;
            IO_RD3_SetLow(); //  IO_RF5_SetHigh();   //Enable DAC, disable RFID
            SPI1_Exchange32bit( i );  //Write to DAC
            IO_RD3_SetHigh();
            k++;
            __delay_us(100);   //Delay so we get an appropriate voltage ramp
            if (print_out)
            {
                printf("\n\r k=%ld ka=%ld kb=%ld", k, ka, kb);
            }
        }
    }
    
    if (kb < ka)   //decrement k to go from a to b
    {
        while (k >= kb)
        {
            long i=64*k + 0x00300000;
            IO_RD3_SetLow(); //  IO_RF5_SetHigh();   //Enable DAC, disable RFID
            SPI1_Exchange32bit( i );  //Write to DAC
            IO_RD3_SetHigh();
            k--;
            __delay_us(100);   //Delay so we get an appropriate voltage ramp
            if (print_out)
            {
                printf("\n\r k=%ld ka=%ld kb=%ld", k, ka, kb);
            }
        }
    }
}


/*
 * Description:  This ISR responds to the interrupt request from the ADC1, after it has made one complete scan.
 *               The raw ADC values are read out of the buffer and added to an accumulator
 *               When the preset number of values has been accumulated, the average is taken and put into a
 *               global variable so that it can be read out by other functions.
 *               The ADC1 was setup with the following parameters:
 *               TAD = 8uS, TCY = 32 (8MHz), Ch8 through Ch12 were both scanned along with 3 other inputs, total of 5 channels in scan list
 *               Enable Auto Sampling was on, AD1CON1 = 0x8474,  AD1CON2 = 0x0C10, AD1CON3 = 0x1F1F
 *   With these parameters, I noticed that the ISR was being executed every 1800uS.  When the averaging had to be done for all channels
 *   the ISR took about 320uS to complete, without the math it took about 3uS to complete (using LSA to look at DIO bit)
 *   I also observed that the 320uS pulse was occuring every 190mS when averaging was set to 100

 * Preconditions:  The ADC initialization function has been called.  The ADC system has been initialized with the proper
 *                 register settings for:  interrupts on, scanning of analog inputs, TAD, TCY, etc.
 * Arguments:  N/A
 * Returns:  N/A
 */
void __attribute__ ( ( __interrupt__ , __auto_psv__ ) ) _ADC1Interrupt ( void )    //Analog Acquisition ISR
{
    // clear the ADC interrupt flag
    IFS0bits.AD1IF = false;
    
    if (g_adc1_total_accum >= ADC_AVERAGES)
    {
        g_adc1_total_accum=0;
        //---------------------------------------
        g_analog_ch8_average = g_analog_ch8_accum/ADC_AVERAGES;      //+3.3V
        g_analog_ch8 = (ADC_REF*g_analog_ch8_average)/4096;
        g_analog_ch8_accum = 0;
        //---------------------------------------
        g_analog_ch9_average = g_analog_ch9_accum/ADC_AVERAGES;      //+5V
        analog_ch9 = (ADC_REF*g_analog_ch9_average)/4096;
        g_analog_ch9_accum = 0;
        //---------------------------------------
        g_analog_ch10_average = g_analog_ch10_accum/ADC_AVERAGES;    //+24V
        g_analog_ch10 = (ADC_REF*g_analog_ch10_average)/4096;
        g_analog_ch10_accum = 0;
        //---------------------------------------
        g_analog_ch11_average = g_analog_ch11_accum/ADC_AVERAGES;    //LED current
        g_analog_ch11 = (ADC_REF*g_analog_ch11_average)/4096 - g_analog_ch11_offset;
        g_analog_ch11_accum = 0;
        //---------------------------------------
        g_analog_ch12_average = g_analog_ch12_accum/ADC_AVERAGES;    //LED voltage
        g_analog_ch12 = (ADC_REF*g_analog_ch12_average)/4096;
        g_analog_ch12_accum = 0;
    }
    else 
    {
        g_adc1_total_accum++;
        g_analog_ch8_accum += ADC1BUF8;
        g_analog_ch9_accum += ADC1BUF9;
        g_analog_ch10_accum += ADC1BUF10;
        g_analog_ch11_accum += ADC1BUF11;
        g_analog_ch12_accum += ADC1BUF12;
    }
}
