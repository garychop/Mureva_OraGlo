/*
 * SWI EEPROM
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "eeprom.h"
#include "swi.h"
#include "mcc_generated_files/mcc.h"
#define FCY	(16000000UL) // required by libpic30.h
#define FCY_MHZ	(16UL)
#include <libpic30.h>

#define AT21CS_WRITE_CYCLE_TIME			5000   //time is in uS

bool EEPROMIdentify(void)
{
	uint8_t slave_address;

	printf("\r\nResetting and discovering EEPROM: ");
	if (!SWIResetAndDiscoveryResponse())
	{
		printf("FAILED: Could not detect EEPROM\r\n");
		return false;
	}
	printf("Detected EEPROM.\r\n");
	printf("Scanning addresses: ");
	for (slave_address = 0; slave_address < 8; slave_address++)
	{
		if (EEPROMReadManufID(slave_address))
		{
			printf("Found EEPROM at address %d\r\n", slave_address);
			break;
		}
	}
	if (slave_address >= 8)
	{
		// Although the SWI EEPROM was detected, a valid address could not be found for it.
		printf("FAILED: Address not found.\r\n");
		return false;
	}
	return true;
}

bool EEPROMReadManufID(uint8_t slave_address)
{
	uint8_t address_word;
	uint8_t data[3];
	bool success = false;
	
	address_word = (AT21CS_OPCODE_MANUFACTURER_ID_READ << 4) | ((slave_address & 0x7) << 1) | EEPROM_READ;
	INTCON2bits.GIE = 0;
	success = SWISendAddress(address_word);
	if (!success)
		goto out;
    
    success = success && SWIReadData(&data[0]);
    SWISendAck();
    success = success && SWIReadData(&data[1]);
    SWISendAck();
    success = success && SWIReadData(&data[2]);
    SWISendNack();
    
	if (success)
	{
		printf("Manufacturing ID: 0x%02X 0x%02X 0x%02X = ", data[0], data[1], data[2]);
		if ((data[0] == 0x00) && (data[1] == 0xd2) && (data[2] == 0x00))
			printf("AT21CS01");
		else if ((data[0] == 0x00) && (data[1] == 0xd3) && (data[2] == 0x80))
			printf("AT21CS11");
		else
			printf("unknown");
		printf("\r\n");
		success = true;
	}
out:
	INTCON2bits.GIE = 1;
	return success;
}

bool EEPROMRead(uint8_t slave_address, uint8_t memory_address, uint8_t *data)
{
	bool success = false;
	uint8_t attempts = 0;

    //Implement a "Random Read" per the protocol of the AT21CS11 datasheet
	INTCON2bits.GIE = 0;
	while ((!success) && (attempts++ < 20))
	{
		success = SWISendAddress((AT21CS_OPCODE_EEPROM_ACCESS << 4) | ((slave_address & 0x7) << 1) | EEPROM_WRITE);
		if (!success)
			goto out;
		SWISendData(memory_address);
        SWIGetAck();  //This completes the "dummy write" operation per the protocol
        
		SWIStartAndStopCondition();
		success = SWISendAddress((AT21CS_OPCODE_EEPROM_ACCESS << 4) | ((slave_address & 0x7) << 1) | EEPROM_READ);
		if (!success)
			goto out;
		success = SWIReadData(data);
        SWISendNack();   //This completes the read operation
		if (!success)
			goto out;
	out:
		SWIStartAndStopCondition();
	}
	INTCON2bits.GIE = 1;
	return success;
}

bool EEPROMWrite(uint8_t slave_address, uint8_t memory_address, uint8_t data)
{
	bool success = false;
	uint8_t attempts = 0;

	INTCON2bits.GIE = 0;
	while ((!success) && (attempts++ < 20))
	{
        
		success = SWISendAddress((AT21CS_OPCODE_EEPROM_ACCESS << 4) | ((slave_address & 0x7) << 1) | EEPROM_WRITE);
		if (!success)
			goto out;
		SWISendData(memory_address);
		success = SWIGetAck();
		if (!success)
			goto out;
		SWISendData(data);
		success = SWIGetAck();
        
        __delay_us(AT21CS_WRITE_CYCLE_TIME);
        
		if (!success)
			goto out;
out:
		SWIStartAndStopCondition();
	}
	INTCON2bits.GIE = 1;
	return success;
}

bool EEPROMWritePage(uint8_t slave_address, uint8_t memory_address, uint8_t *data, uint8_t length)
{
	bool success = false;
	uint8_t attempts = 0;

	if (length > 8)
		return false;
	INTCON2bits.GIE = 0;
	while ((!success) && (attempts++ < 20))
	{
		success = SWISendAddress((AT21CS_OPCODE_EEPROM_ACCESS << 4) | ((slave_address & 0x7) << 1) | EEPROM_WRITE);
		if (!success)
			goto out;
		SWISendData(memory_address);
		success = SWIGetAck();
		if (!success)
			goto out;
		/* From the AT21CS11 data sheet:
		 * A page write operation allows up to eight bytes to be written in the same write cycle, provided all bytes are in the
		 * same row (address bits A6 through A3 are the same) of the memory array. Partial page writes of less than eight bytes
		 * are allowed.
		 * A page write is initiated the same way as a byte write, but the bus master does not send a Stop condition after the
		 * first data byte is clocked in. Instead, after the EEPROM Acknowledges receipt of the first data byte, the bus master
		 * can transmit up to an additional seven data bytes.
		 */
		while (length-- > 0)
		{
			SWISendData(*data++);
			success = SWIGetAck();
			if (!success)
				goto out;
		}
out:
		SWIStartAndStopCondition();
	}
	INTCON2bits.GIE = 1;
	return success;
}

bool EEPROMReadSecurity(uint8_t slave_address, uint8_t memory_address, uint8_t *data)
{
	bool success = false;
	uint8_t attempts = 0;
    uint16_t looper;

	INTCON2bits.GIE = 0;        // Disable global interrupts
	while ((!success) && (attempts++ < 20))
	{
		success = SWISendAddress((AT21CS_OPCODE_SECURITY_REGISTER_ACCESS << 4) | ((slave_address & 0x7) << 1) | EEPROM_WRITE);
		if (success)
        {
            SWISendData(memory_address);
            SWIStartAndStopCondition();
    		success = SWISendAddress((AT21CS_OPCODE_SECURITY_REGISTER_ACCESS << 4) | ((slave_address & 0x7) << 1) | EEPROM_READ);
        	if (success)
            {
        		success = SWIReadData(data);
            }
        }
        if (!success)
        {
            // delay for a little bit
            for (looper = 0; looper < 1000; ++looper)
                ;
        }
		SWIStartAndStopCondition();
	}
	INTCON2bits.GIE = 1;        // enable global interrupts
	return success;
}

bool EEPROMWriteSecurity(uint8_t slave_address, uint8_t memory_address, uint8_t data)
{
	bool success = false;
	uint8_t attempts = 0;

	INTCON2bits.GIE = 0;
	while ((!success) && (attempts++ < 20))
	{
		success = SWISendAddress((AT21CS_OPCODE_SECURITY_REGISTER_ACCESS << 4) | ((slave_address & 0x7) << 1) | EEPROM_WRITE);
		if (!success)
			goto out;
		SWISendData(memory_address);
		success = SWIGetAck();
		if (!success)
			goto out;
		SWISendData(data);
		success = SWIGetAck();
		if (!success)
			goto out;
out:
		SWIStartAndStopCondition();
	}
	INTCON2bits.GIE = 1;
	return success;
}

bool EEPROMDump(uint8_t slave_address)
{
	uint8_t addr;
	uint8_t data;
	bool success = false;
	
    printf("   AT21CS01 / AT21CS11 Notes\r\n");
    printf("   Single wire (plus ground) 128 Byte EEPROM\r\n");
    printf("   128B region has 4 zones of 32B each that can be individually locked permanently.  \r\n");
    printf("   Additional 32B Security Region holds unique serial number, set at the device mfg.  \r\n");
    printf("   16B of the Security Region are user programmable and can be locked permanently.  \r\n");
    printf("\r\n");
    printf("   128 Byte EEPROM region\r\n");
	printf("    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\r\n");
	printf("   -----------------------------------------------");
	for (addr = 0; addr < 128; addr++)
	{
		success = EEPROMRead(slave_address, addr, &data);
		if (success)
		{
			if (addr % 16 == 0)
				printf("\r\n%X_ ", addr >> 4);
			printf("%02X ", data);
		}
		else
		{
			printf("EEPROM read error\r\n");
			goto out;
		}
	}
    printf("\r\n");
    printf("\r\n   32 Byte Security region");
	printf("\r\n   -----------------------------------------------");
	for (addr = 0; addr < 32; addr++)
	{
		success = EEPROMReadSecurity(slave_address, addr, &data);
		if (success)
		{
			if (addr % 16 == 0)
				printf("\r\n%X_ ", addr >> 4);
			printf("%02X ", data);
		}
		else
		{
			printf("EEPROM read error\r\n");
			goto out;
		}
	}
	printf("\r\n");
out:
	return success;
}

bool EEPROMLockSecurity(uint8_t slave_address)
{
	bool success = false;
	uint8_t attempts = 0;

	INTCON2bits.GIE = 0;
	while ((!success) && (attempts++ < 20))
	{
		success = SWISendAddress((AT21CS_OPCODE_LOCK_SECURITY_REGISTER << 4) | ((slave_address & 0x7) << 1) | EEPROM_WRITE);
		if (!success)
			goto out;
		SWISendData(0x60);
		success = SWIGetAck();
		if (!success)
			goto out;
		SWISendData(0);
		success = SWIGetAck();
		if (!success)
			goto out;
out:
		SWIStartAndStopCondition();
	}
	INTCON2bits.GIE = 1;
	return success;
}
