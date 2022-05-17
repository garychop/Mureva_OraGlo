/*
 * SWI protocol implementation
 */

#include <stdio.h>
#include <stdbool.h>
#define FCY	(16000000UL) // required by libpic30.h
#define FCY_MHZ	(16UL)
#include <libpic30.h>
#include "mcc_generated_files/pin_manager.h"
#include "swi.h"

/*
 * Most 1-Wire devices support two data rates: standard speed of 15 kbps and overdrive speed of 111 kbps.
 */

// Table of SWI bit timings (in microseconds)
const uint16_t period[NUM_SIO_BUS_SPEEDS][NUM_SIO_PERIOD_TYPES] = {
//	BIT,		DACK,	DRR,	DSCHG,	  HLD0,	   HTSS,	  LOW0,	  LOW1,	   MRS,		RD,		RESET,	   RRT
	{ 70,		20,		7,		150,		55,		500,		60,		6,		50,		9,		480,		20,},	// SIO_BUS_SPEED_STANDARD
	{ 15,		8,		1,		150,		6,		150,		8,		1,		2,		1,		96,		    8,}	// SIO_BUS_SPEED_HIGH
};

uint8_t bus_speed = SIO_BUS_SPEED_HIGH;

void SWIInit(void)
{
	SIO_SetDigitalInput();
	SIO_SetLow();
}

bool SWIResetAndDiscoveryResponse(void)
{
	bool success;

	SIO_SetDigitalOutput();
	__delay_us(period[bus_speed][SIO_PERIOD_TYPE_RESET]);
	SIO_SetDigitalInput();
	__delay_us(period[bus_speed][SIO_PERIOD_TYPE_RRT]);
	success = SWIGetAck();
	return success;
}

void SWIStartAndStopCondition(void)
{
	SIO_SetDigitalInput();
	__delay_us(period[bus_speed][SIO_PERIOD_TYPE_HTSS]);
}

bool SWISendAddress(uint8_t addr)
{
	bool success;
	
	SWIStartAndStopCondition();
	SWISendData(addr);
	success = SWIGetAck();
	return success;
}

void SWISendData(uint8_t data)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
		SWISendBit((data >> (7 - i)) & 1);
}

bool SWIReadData(uint8_t *data)
{
	bool b;
	uint8_t i;

	// Read in data bit by bit
	*data = 0;
	for (i = 0; i < 8; i++)
	{
		if (SWIRecvBit(&b))
		{
			*data |= b << (7 - i);
		}
		else
		{
			// Missed a bit, so the data transfer fails.
			return false;
		}
	}
	return true;
}

bool SWIGetAck(void)
{
	bool ack = false;
	SIO_SetDigitalOutput();
	__delay_us(period[bus_speed][SIO_PERIOD_TYPE_DRR]);
	// At this point, the slave should be holding the SIO line low for an ACK. We check for that here:
	SIO_SetDigitalInput();
	// Give the SIO line time to go high if the slave is not sending an ACK
	__delay_us(SIO_RETURN_TIME);
	// Check for ACK from slave
	if (!SIO_GetValue())
		ack = true;
    //Continue to wait to complete the bit time (Ack = reading a 0 from the EEPROM)
    __delay_us(period[bus_speed][SIO_PERIOD_TYPE_BIT] - period[bus_speed][SIO_PERIOD_TYPE_DRR] - SIO_RETURN_TIME);
	
    return ack;
}

void SWISendBit(bool b)
{
	SIO_SetDigitalOutput();
	__delay_us(period[bus_speed][b ? SIO_PERIOD_TYPE_LOW1 : SIO_PERIOD_TYPE_LOW0]);
	SIO_SetDigitalInput();
	if (b)
	{
		__delay_us(period[bus_speed][SIO_PERIOD_TYPE_BIT] - period[bus_speed][SIO_PERIOD_TYPE_LOW1]);
	}
	else
	{
		__delay_us(period[bus_speed][SIO_PERIOD_TYPE_BIT] - period[bus_speed][SIO_PERIOD_TYPE_LOW0]);
	}
}

bool SWIRecvBit(bool *bit)
{
	bool received = false;
	bool b;
	int timeout = 100;

	SIO_SetDigitalOutput();
	__delay_us(period[bus_speed][SIO_PERIOD_TYPE_RD]);
	SIO_SetDigitalInput();
	// Give the SIO line time to go high if the slave is sending a '1'
	__delay_us(SIO_RETURN_TIME);
	b = SIO_GetValue();
	// Wait for SIO to return high
	while ((SIO_GetValue() == 0) && (timeout-- > 0))
	{
	}
	if (timeout == 0)
	{
		// SIO is stuck low.
		printf("SIO is stuck low!\r\n");
	}
	else
	{
		*bit = b;
		received = true;
	}
	__delay_us(period[bus_speed][SIO_PERIOD_TYPE_RRT]);
	return received;
}

void SWISendAck(void)
{
	SWISendBit(0);
}

void SWISendNack(void)
{
	SWISendBit(1);
}
