/*
 * RTCC
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#define FCY	(16000000UL) // required by libpic30.h
#include <libpic30.h>
#include "rtcc.h"
#include "Support_Functions.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/i2c1.h"

#define RTCC_ADDRESS	(0xde >> 1)


bool RTCCIdentify(void)
{
	return true;
}

bool RTCCInit(void)
{
	if (!RTCCGetStartStatus())
	{
		// RTCC was not running. Time/Date will be incorrect. Starting.
		RTCCSetStartStatus(true);
	}
	if (!RTCCGetVBATEN())
	{
		// VBATEN is not enabled, so enable it.
		RTCCSetVBATEN(true);
	}
	return true;
}

bool RTCCGetStartStatus(void)
{
	uint8_t data;
	int8_t retries;
	I2C1_MESSAGE_STATUS status;
	// Send address of 0
	data = 0x00;
	retries = 10;
	I2C1_MasterWrite(&data, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	// Read the Start Oscillator bit (which is the MSB in the seconds place)
	retries = 10;
	I2C1_MasterRead(&data, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (2)\r\n");
		return false;
	}
	return (data & 0x80);
}

bool RTCCSetStartStatus(bool start)
{
	uint8_t data[2];
	int8_t retries;
	I2C1_MESSAGE_STATUS status;
	bool st_bit;
	st_bit = RTCCGetStartStatus();
	if (start && st_bit)
		goto out;
	if (!start && !st_bit)
		goto out;
	// We need to read the current seconds value so we can restore it after setting or clearing the ST bit.
	// Send address of 0
	data[0] = 0x00;
	retries = 10;
	I2C1_MasterWrite(data, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	// Read the Start Oscillator bit (which is the MSB in the seconds place)
	retries = 10;
	I2C1_MasterRead(&data[1], 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (2)\r\n");
		return false;
	}
	// Set or Clear the Start Oscillator bit
	data[1] &= ~0x80;
	if (start)
		data[1] |= 0x80;
	retries = 10;
	I2C1_MasterWrite(data, 2, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (3)\r\n");
		return false;
	}
out:
	return true;
}

bool RTCCDisplayDateTime(void)
{
	rtcc_data_t rtcc_data;
    uint32_t LCUTimeStamp;
    char String[64];
    
    printf("\n\r");
	RTCCGetDateTimeData(&rtcc_data);
	RTCCPrint(&rtcc_data);
    
    RTCCCalcLCUTimestamp( &rtcc_data, &LCUTimeStamp);
    printf("LCUTimeStamp = %ld Seconds since 1/1/00 12:00:00 EST\n\r", LCUTimeStamp );
    
    RTCCGetSRAM(String);
    printf("RTCC SRAM init string: %s", String);
    printf("\n\r");
    
	return true;
}

/*
 * RTCC
 * 
 * MCP7940N Registers for storing date and time stamp:
 * 
 * Address    Description
 *   00h        Seconds in binary coded decimal
 *   01h        Minutes      "
 *   02h        Hours        "
 *   03h        Weekday
 *   04h        Date
 *   05h        Month
 *   06h        Year
 * 
 * 
 */
bool RTCCGetDateTimeData(rtcc_data_t *r)
{
	// Write address 0
	uint8_t address = 0x00;
	int8_t retries;
	I2C1_MESSAGE_STATUS status;
	
	retries = 10;
	I2C1_MasterWrite(&address, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	// Read in 7 bytes of RTCC data
	retries = 20;
	I2C1_MasterRead((uint8_t *)r, 7, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (2)\r\n");
		return false;
	}
	return true;
}

bool RTCCSetDateTimeData(rtcc_data_t *r)
{
	uint8_t data[8];
	int8_t retries;
	I2C1_MESSAGE_STATUS status;
	// Write address 0
	data[0] = 0x00;
	memcpy(data + 1, r, 7);
	retries = 10;
	I2C1_MasterWrite(data, 8, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	return true;
}

void RTCCPrint(rtcc_data_t *r)
{
	printf("20%d%d/%d%d/%d%d %d%d:%d%d:%d%d\r\n", r->rtcyear.yrten, r->rtcyear.yrone, r->rtcmth.mthten, r->rtcmth.mthone, r->rtcdate.dateten, r->rtcdate.dateone, r->rtchour.hrten, r->rtchour.hrone, r->rtcmin.minten, r->rtcmin.minone, r->rtcsec.secten, r->rtcsec.secone);
}

bool RTCCCalcLCUTimestamp(rtcc_data_t *r, uint32_t *LCUTimeStamp)
{
    uint16_t Month_to_Days[13]={0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    uint16_t Month_to_Days_in_LPYR[13]={0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
    
    uint8_t year = 10*(r->rtcyear.yrten) + r->rtcyear.yrone;
    uint8_t month = 10*(r->rtcmth.mthten) + r->rtcmth.mthone;
    bool leapyear = r->rtcmth.lpyr;
    uint8_t day = 10*(r->rtcdate.dateten) + r->rtcdate.dateone;
    uint8_t hour = 10*(r->rtchour.hrten) + r->rtchour.hrone;
    uint8_t min = 10*(r->rtcmin.minten) + r->rtcmin.minone;
    uint8_t sec = 10*(r->rtcsec.secten) + r->rtcsec.secone;
    
    uint16_t NumberOfFullDays;

    //Some sanity checking
    *LCUTimeStamp = 0;
    if((year < 19) | (year > 100)){return false;}
    if((month > 12) | (month == 0)){return false;}
    if((day > 31) | (day == 0))    {return false;}
    if(hour > 24){return false;}
    if(min > 60){return false;}
    if(sec > 60){return false;}
    
    //Convert months to days, add in the number of days in the partial month, add in days for previous leap years
    //leap years are 2000, 2004, 2008, 2012, 2016, 2020, etc
    //Note that if the current day is 9, there are only 8 full days to be counted here
    if(leapyear == 0)
    {
        NumberOfFullDays = Month_to_Days[month-1] + year/4 + day - 1;
    }
    else
    {
        NumberOfFullDays = Month_to_Days_in_LPYR[month-1] + year/4 + day - 1;
    }
   
    //Calculate the number of seconds since Saturday Jan 1, 2000 at 12:00:00 midnight Eastern Standard Time
    //Disregard daylight savings time (therefore LCUTimeStamp will be an hour too long during the summer)
    *LCUTimeStamp = (uint32_t)year*365*24*60*60 + (uint32_t)NumberOfFullDays*24*60*60 + (uint32_t)hour*60*60 + (uint32_t)min*60 +sec;
    
    return true;
}

bool RTCCGetTimestamp(uint32_t *LCUTimeStamp)
{
    rtcc_data_t rtcc_data;
        
	RTCCGetDateTimeData(&rtcc_data);
    RTCCCalcLCUTimestamp(&rtcc_data, LCUTimeStamp);
    
    return true;
}

bool RTCCGetVBATEN(void)
{
	uint8_t data;
	int8_t retries;
	I2C1_MESSAGE_STATUS status;
	// Send address of 3
	data = 0x03;
	retries = 10;
	I2C1_MasterWrite(&data, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	// Read the VBATEN bit
	retries = 10;
	I2C1_MasterRead(&data, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (2)\r\n");
		return false;
	}
	return (data & 0x08);
}

bool RTCCSetVBATEN(bool enable)
{
	uint8_t data[2];
	int8_t retries;
	I2C1_MESSAGE_STATUS status;
	bool vbaten_bit;
	vbaten_bit = RTCCGetVBATEN();
	if (enable && vbaten_bit)
		goto out;
	if (!enable && !vbaten_bit)
		goto out;
	// We need to read the current rtcwkday value so we can restore it after setting or clearing the VBATEN bit.
	// Send address of 3
	data[0] = 0x03;
	retries = 10;
	I2C1_MasterWrite(data, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	// Read the VBATEN bit
	retries = 10;
	I2C1_MasterRead(&data[1], 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (2)\r\n");
		return false;
	}
	// Set or Clear the VBATEN bit
	data[1] &= ~0x08;
	if (enable)
		data[1] |= 0x08;
	retries = 10;
	I2C1_MasterWrite(data, 2, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (3)\r\n");
		return false;
	}
out:
	return true;
}

bool RTCCGetPWRFAIL(void)
{
	uint8_t data;
	int8_t retries;
	I2C1_MESSAGE_STATUS status;
	// Send address of 3
	data = 0x03;
	retries = 10;
	I2C1_MasterWrite(&data, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	// Read the PWRFAIL bit
	retries = 10;
	I2C1_MasterRead(&data, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (2)\r\n");
		return false;
	}
	return (data & 0x10);
}

bool RTCCClearPWRFAIL(void)
{
	uint8_t data[2];
	int8_t retries;
	I2C1_MESSAGE_STATUS status;
	bool pwrfail_bit;
	pwrfail_bit = RTCCGetPWRFAIL();
	if (!pwrfail_bit)
		goto out;
	// We need to read the current rtcwkday value so we can restore it after setting or clearing the PWRFAIL bit.
	// Send address of 3
	data[0] = 0x03;
	retries = 10;
	I2C1_MasterWrite(data, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	// Read the PWRFAIL bit
	retries = 10;
	I2C1_MasterRead(&data[1], 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (2)\r\n");
		return false;
	}
	// Clear the PWRFAIL bit
	data[1] &= ~0x10;
	retries = 10;
	I2C1_MasterWrite(data, 2, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (3)\r\n");
		return false;
	}
out:
	return true;
}

bool RTCCDisplayPowerFailStatus(void)
{
	bool pwrfail_bit = RTCCGetPWRFAIL();
	printf("\r\nCurrent PWRFAIL status: %s\r\n", pwrfail_bit ? "FAIL" : "PASS");
	return true;
}

bool RTCCSetDate(void)
{
	rtcc_data_t r;
	uint16_t input;
	bool success;
	printf("\r\nSet date:");
	// Read the current date and time from the RTCC
	RTCCGetDateTimeData(&r);
	// Set the year
	if (WaitForValidUInt16WithLimits(" Enter last two digits of year: ", &input, 0, 99) != INPUT_VALID)
		return false;
	r.rtcyear.yrten = input / 10;
	r.rtcyear.yrone = input % 10;
	// Set the month
	if (WaitForValidUInt16WithLimits("\r\n Enter month number (January = 1): ", &input, 1, 12) != INPUT_VALID)
		return false;
	r.rtcmth.mthten = input / 10;
	r.rtcmth.mthone = input % 10;
	// Set the day
	if (WaitForValidUInt16WithLimits("\r\n Enter day number: ", &input, 1, 31) != INPUT_VALID)
		return false;
	r.rtcdate.dateten = input / 10;
	r.rtcdate.dateone = input % 10;
	success = RTCCSetDateTimeData(&r);
	printf("\r\n %s\r\n", success ? "SUCCEEDED" : "FAILED");
    
    if (RTCCInitSRAM())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool RTCCSetTime(void)
{
	rtcc_data_t r;
	uint16_t input;
	bool success;
	printf("\r\nSet time:");
	// Read the current date and time from the RTCC
	RTCCGetDateTimeData(&r);
	// Set the hour
	if (WaitForValidUInt16WithLimits(" Enter the hour in 24 hour format: ", &input, 0, 23) != INPUT_VALID)
		return false;
	r.rtchour.hrten = input / 10;
	r.rtchour.hrone = input % 10;
	// Specify 24 hour format
	r.rtchour.hrfmt = 0;
	// Set the minute
	if (WaitForValidUInt16WithLimits("\r\n Enter the minute: ", &input, 0, 59) != INPUT_VALID)
		return false;
	r.rtcmin.minten = input / 10;
	r.rtcmin.minone = input % 10;
	// Set the second
	if (WaitForValidUInt16WithLimits("\r\n Enter the second: ", &input, 0, 59) != INPUT_VALID)
		return false;
	r.rtcsec.secten = input / 10;
	r.rtcsec.secone = input % 10;
	success = RTCCSetDateTimeData(&r);
	printf("\r\n %s\r\n", success ? "SUCCEEDED" : "FAILED");
	return true;
}

bool RTCCDumpRegisters(void)
{
	uint8_t addr = 0;
	uint8_t data[64];
	bool success = false;
    uint8_t address;
	int8_t retries;
	I2C1_MESSAGE_STATUS status;
    
    printf("\r\n   MCP7940N RTCC Register set");
    printf("\r\n   00 - 06: Time and Date registers");
    printf("\r\n   07 - 1F: Config, Trim, Alarms, Power fail timestamp");
    printf("\r\n   20 - 5F: Battery backed SRAM");
    printf("\r\n");
	printf("\r\n    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\r\n");
	printf("   -----------------------------------------------");
	
    // Write address 0
    address = 0x00;
	retries = 10;
	I2C1_MasterWrite(&address, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	// Read in all of the register data
	retries = 20;
	I2C1_MasterRead(data, 32, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (2)\r\n");
		return false;
	}
	for (addr = 0; addr < 32; addr++)
	{
		if (addr % 16 == 0)
			printf("\r\n%X_ ", addr >> 4);
		printf("%02X ", data[addr]);
	}
    
    // The RTCC's register space and the RTCC's SRAM space are actually two different blocks within
    // the RTCC chip.  Per the datasheet, the address pointer rolls over within each block so that it
    // always stays within the current block (register space or SRAM)
	// Write address 
	address = 0x20;  //address for the start of SRAM
	retries = 10;
	I2C1_MasterWrite(&address, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	// Read in all of the register data
	retries = 20;
	I2C1_MasterRead(data, 64, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (2)\r\n");
		return false;
	}
	for (addr = 32; addr < 96; addr++)
	{
		if (addr % 16 == 0)
			printf("\r\n%X_ ", addr >> 4);
		printf("%02X ", data[addr-32]);
	}    
 
	printf("\r\n");
	return success;
}

bool RTCCReadRegister(void)
{
	uint8_t addr = 0;
	uint8_t data;
	int8_t retries;
    char HexAddress[3];
    char *EndPtr;    
    
    printf("\n\rEnter address (in hex): ");
    ReadString(HexAddress, 3);
    addr = strtol(HexAddress, &EndPtr, 16);
  
    if (!((addr >= 0x00) && (addr <= 0x5f)))
    {
        return false;
    }

	I2C1_MESSAGE_STATUS status;
	retries = 10;
	I2C1_MasterWrite(&addr, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	// Read in the register data
	retries = 20;
	I2C1_MasterRead(&data, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (2)\r\n");
		return false;
	}
	printf("\r\nData = 0x%02X\r\n", data);
	return true;
}

bool RTCCWriteRegister(void)
{
	uint8_t data[2];
	int8_t retries;
    char HexAddress[3], HexData[3];
    char *EndPtr;        
 
    printf("\n\rEnter address (in hex): ");
    ReadString(HexAddress, 3);
    data[0] = strtol(HexAddress, &EndPtr, 16);
  
    if (!((data[0] >= 0x00) && (data[0] <= 0x5f)))
    {
        return false;
    }
 
    printf("\n\rEnter data (in hex): ");
    ReadString(HexData, 3);
    data[1] = strtol(HexData, &EndPtr, 16);
 
	I2C1_MESSAGE_STATUS status;
	retries = 10;
	I2C1_MasterWrite(data, 2, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	printf("\r\n");
	return true;
}


/*
 * Description:  This function initializes the RTCC's SRAM with a string constant.
 *               The SRAM is maintained by the back-up battery, along with the real time clock.
 *               This string constant is used as an indicator that the battery is still good.
 *               As long as the RTCC SRAM has been initialized with this string constant, then
 *               it should always be held in the SRAM unless the battery fails.
 *             
 * Preconditions:  I2C1 is initialized 
 * Arguments:  none
 * Returns:  pass/fail indication
 */
bool RTCCInitSRAM(void)
{
    const char cString[64] = RTCC_INIT_STRING;
    uint8_t pdata[65]; 
    uint8_t i;  //general working variable
	int8_t retries;
	I2C1_MESSAGE_STATUS status;
    
    //initialize the array of data to be sent, keeping in mind that the first byte 
    //is dedicated to the address of the RTCC's SRAM region
    for(i=1; i<65; i++)
    {
        pdata[i] = (uint8_t)cString[i-1]; 
    }
    
    pdata[0] = 0x20;  //Set address to the beginning of the SRAM region

	// Initialize SRAM to all zeroes
	retries = 10;
	I2C1_MasterWrite(pdata, 65, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}   
     
	return true;
}



/*
 * Description:  This function reads the RTCC's SRAM and returns a string constant.
 *               The SRAM is maintained by the back-up battery, along with the real time clock.
 *               This string constant is used as an indicator that the battery is still good.
 *               As long as the RTCC SRAM has been initialized with this string constant, then
 *               it should always be held in the SRAM unless the battery fails.
 *             
 * Preconditions:  I2C1 is initialized 
 * Arguments:  none
 * Returns:  pass/fail indication.  Also verifies the MuReva initialization string was correct
 */
bool RTCCGetSRAM(char *String)
{
	uint8_t data[64];
    uint8_t address;
	int8_t retries;
	I2C1_MESSAGE_STATUS status;
    uint8_t length = strlen(RTCC_INIT_STRING);
    
	// Write address of RTCC's SRAM region
	address = 0x20;  //address for the start of SRAM
	retries = 10;
	I2C1_MasterWrite(&address, 1, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (1)\r\n");
		return false;
	}
	// Read in all of the SRAM data
	retries = 20;
	I2C1_MasterRead(data, 64, RTCC_ADDRESS, &status);
	while ((status == I2C1_MESSAGE_PENDING) && (retries-- > 0))
		__delay_ms(1);
	if (retries == 0)
	{
		printf("RTCC I2C1 timed out (2)\r\n");
		return false;
	}

    //copy 
    memcpy(String , data, length+1);
    
    if(strcmp(String, RTCC_INIT_STRING) ==0)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}