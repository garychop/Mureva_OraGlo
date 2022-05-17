/*
 * MCA parameter management
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "mca.h"
#include "eeprom.h"
#include "Support_Functions.h"
#include "rtcc.h"
#include "UI_Functions.h"
#include "mcc_generated_files/uart1.h"

// Multi-byte data elements are stored in the EEPROM in big-endian format
// These values are stored in the general-purpose memory area
#define EEPROM_ADDR_MCA_TOTAL_USAGE_TIME	0x00   //requires 16 bits
#define EEPROM_ADDR_MCA_MAX_USAGE_TIME  	0x02   //requires 16 bits
#define EEPROM_ADDR_MCA_CALIBRATION         0x04   //requires 16 bits
#define EEPROM_ADDR_MCA_TIME_STAMP			0x06   //requires 32 bits
// The serial number is stored in the security register
#define EEPROM_ADDR_MCA_SERIAL_NUM			0x10


bool MCADisplayContents(void)
{
	bool success = true;
	bool error = false;
	uint8_t data[MCA_SERIAL_NUM_LEN + 1];
	uint16_t MCATotalUsageTime;
	uint16_t MCAMaxUsageTime;
	uint16_t MCACalibration;
	uint32_t MCATimeStamp;
    

	printf("\r\nMCA EEPROM contents:\r\n");
	success = MCAReadSerialNumber(data);
	if (success)
		printf(" Serial number: %s\r\n", data);
	else
		error = true;
	success = MCAReadCalibration(&MCACalibration);
	if (success)
		printf(" MCA calibration: %u  =>  %.2f\r\n", MCACalibration, (double)MCACalibration/32768 );
	else
		error = true;
	success = MCAReadActiveUseTime(&MCATotalUsageTime);
	if (success)
		printf(" Total active use time (S): %u\r\n", MCATotalUsageTime);
	else
		error = true;
	success = MCAReadMaxUseTime(&MCAMaxUsageTime);    
	if (success)
		printf(" Maximum use time (S): %u\r\n", MCAMaxUsageTime);
	else
		error = true;
	success = MCAReadTimeStamp(&MCATimeStamp);
	if (success)
		printf(" Time stamp (S): %ld\r\n", MCATimeStamp);
	else
		error = true;
	return !error;
}

/*
 * Description:  This function reads the Serial Number from the EEPROM in 
 *               the Mouthpiece
 * Preconditions:  The hardware must be initialized
 * Arguments:  pointer to Serial Number array
 * Returns:  false = Fail, true = Pass
 */
bool MCAReadSerialNumber(uint8_t *MCASerialNumber)
{
	bool success = false;
	int i, retries;
	uint8_t crc;

    retries = 0;
    while ((success == false) && (retries < 4))
    {
        // sit here for brief moment to let the MCA get settled
        for (i = 0; i < 10000; ++i)
            ;
        
        // Want to read the entire serial number entry, including any unused characters, but not the additional CRC stored at the end of the serial number.
        for (i = 0; i < MCA_SERIAL_NUM_LEN; i++)
        {
            success = EEPROMReadSecurity(0, EEPROM_ADDR_MCA_SERIAL_NUM + i, &MCASerialNumber[i]);
            if (!success)
            {
                printf("ERROR: Could not read serial number in MCA EEPROM security area\r\n");
                break;
            }
        }
        if (success)
        {
            // Null terminate string in case serial number uses all 9 digits
            MCASerialNumber[MCA_SERIAL_NUM_LEN] = 0;
            success = EEPROMReadSecurity(0, EEPROM_ADDR_MCA_SERIAL_NUM + MCA_SERIAL_NUM_LEN, &crc);
            if (!success)
            {
                printf("ERROR: Could not read CRC for serial number in MCA EEPROM security area\r\n");
            }
            else
            {
                if (crc8(MCASerialNumber, strnlen((char *)MCASerialNumber, MCA_SERIAL_NUM_LEN)) != crc)
                {
                    printf("CRC is incorrect for stored serial number\r\n");
                    success = false;
                }
            }
        }
        ++retries;
    }
	return success;
}

bool MCAWriteSerialNumber(uint8_t *MCASerialNumber)
{
	bool success = false;
	int i;

	for (i = 0; i < MCA_SERIAL_NUM_LEN; i++)
	{
		success = EEPROMWriteSecurity(0, EEPROM_ADDR_MCA_SERIAL_NUM + i, (i <= strnlen((char *)MCASerialNumber, MCA_SERIAL_NUM_LEN) ? MCASerialNumber[i] : 0));
		if (!success)
		{
			printf("ERROR: Could not write serial number to MCA EEPROM security area\r\n");
			goto out;
		}
	}
	success = EEPROMWriteSecurity(0, EEPROM_ADDR_MCA_SERIAL_NUM + MCA_SERIAL_NUM_LEN, crc8(MCASerialNumber, strnlen((char *)MCASerialNumber, MCA_SERIAL_NUM_LEN)));
	if (!success)
	{
		printf("ERROR: Could not write serial number CRC to MCA EEPROM security area\r\n");
		goto out;
	}
out:
	return success;
}

bool MCASetSerialNumber(void)
{
	// Serial number of mouthpiece: 9 digits, 0 to 1M, of the form "MCA01NNNN"
	bool success = false;
	uint8_t data[MCA_SERIAL_NUM_LEN + 1];

	printf("\r\n");
	success = MCAReadSerialNumber(data);
	if (success)
		printf("Current serial number: %s\r\n", data);
	printf("Enter new serial number: ");
	if (ReadString((char *)data, MCA_SERIAL_NUM_LEN + 1) > 0)
	{
		printf("\r\nSetting serial number to: %s\r\n", data);
		success = MCAWriteSerialNumber(data);
	}
	else
		printf("\r\nSerial number not changed.\r\n");
	return success;
}

bool MCAReadActiveUseTime(uint16_t *MCATotalUsageTime)
{
	uint8_t data_byte;
	uint16_t data_word;
	bool success;
	
	success = EEPROMRead(0, EEPROM_ADDR_MCA_TOTAL_USAGE_TIME, &data_byte);
	if (!success)
		goto out;
	data_word = data_byte * 256;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_TOTAL_USAGE_TIME + 1, &data_byte);
	if (!success)
		goto out;
	data_word += data_byte;
	*MCATotalUsageTime = data_word;
out:
	return success;
}

bool MCAWriteActiveUseTime(uint16_t MCATotalUsageTime)
{
	uint8_t data_byte;
	bool success;
	
	data_byte = (MCATotalUsageTime >> 8) & 0xff;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_TOTAL_USAGE_TIME, data_byte);
	if (!success)
		goto out;
	data_byte = MCATotalUsageTime & 0xff;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_TOTAL_USAGE_TIME + 1, data_byte);
	if (!success)
		goto out;
out:
	return success;
}

bool MCAIncActiveUseTime(void)
{
    bool success;
    uint8_t Iteration;
    uint16_t MCATotalUsageTime;
    uint16_t MCATotalUsageTimePlusOne;
    uint8_t EEPROM_ReadErrorCounterMax = 5; // [SDS#7.5.16]
    
    //Read the total usage time, try several times if there are failures
    Iteration=1;
    while(1)
    {
        success = MCAReadActiveUseTime(&MCATotalUsageTime);
        if(success){break;}
        if(Iteration>=EEPROM_ReadErrorCounterMax){return false;}  //we can't read
        Iteration++;
    }
        
    //Increment the total usage time and write it back, try several times if there are failures   
    Iteration=1;
    while(1)
    {
        success = MCAWriteActiveUseTime(MCATotalUsageTime+1);
        if(success){break;}
        if(Iteration>=EEPROM_ReadErrorCounterMax){return false;}  //we can't write
        Iteration++;
    }
    
     //Read back the new value and verify it is expected   
    Iteration=1;
    while(1)
    {
        success = MCAReadActiveUseTime(&MCATotalUsageTimePlusOne);
        if(success){break;}
        if(Iteration>=EEPROM_ReadErrorCounterMax){return false;}  //we can't read it back
        Iteration++;
    }

    if (MCATotalUsageTimePlusOne == (MCATotalUsageTime+1))
    {
        return true;
    }
    else
    {
        return false;
    }
}




bool MCASetActiveUseTime(void)
{
	bool success = false;
	uint16_t data_word;
	uint16_t MCATotalUsageTime;

	printf("\r\n");
	success = MCAReadActiveUseTime(&MCATotalUsageTime);
	if (!success)
		goto out;
	printf("Total active use time (s): %u\r\n", MCATotalUsageTime);
	if (WaitForValidUInt16WithLimits("Enter new use time: ", &data_word, 0, 60000) == INPUT_VALID)
	{
		success = MCAWriteActiveUseTime(data_word);
		success = MCAReadActiveUseTime(&MCATotalUsageTime);
		if (success)
			printf("\r\nTotal active use time (s): %u\r\n", MCATotalUsageTime);
	}
	else
		printf("\r\n");
out:
	return success;
}

bool MCAReadMaxUseTime(uint16_t *MCAMaxUsageTime)
{
	uint8_t data_byte;
    uint16_t data_word;
	bool success;
	
	success = EEPROMRead(0, EEPROM_ADDR_MCA_MAX_USAGE_TIME, &data_byte);
	if (!success)
		goto out;
    data_word = (uint16_t)data_byte*256;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_MAX_USAGE_TIME + 1, &data_byte);
	if (!success)
		goto out;
    data_word = data_word | (uint16_t)data_byte;
    
	*MCAMaxUsageTime = data_word;
out:
	return success;
}

bool MCAWriteMaxUseTime(uint16_t MCAMaxUsageTime)
{
    uint8_t data_byte;
	bool success;

    data_byte = (MCAMaxUsageTime >> 8);
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_MAX_USAGE_TIME, data_byte);
    if (!success)
        goto out;
    data_byte = MCAMaxUsageTime;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_MAX_USAGE_TIME + 1, data_byte);
    if (!success)
        goto out;
    
out:
	return success;
}

bool MCASetMaxUseTime(void)
{
	bool success = false;
	uint16_t data_word;
	uint16_t MCAMaxUsageTime;

	printf("\r\n");
	success = MCAReadMaxUseTime(&MCAMaxUsageTime);
	if (!success)
		goto out;
	printf("Max usage time: %u\r\n", MCAMaxUsageTime);
	if (WaitForValidUInt16WithLimits("Enter new max usage time: ", &data_word, 0, 65535) == INPUT_VALID)
	{
		success = MCAWriteMaxUseTime(data_word);
		success = MCAReadMaxUseTime(&MCAMaxUsageTime);
		if (success)
			printf("\r\nMax usage time: %u\r\n", MCAMaxUsageTime);
	}
	else
		printf("\r\n");
out:
	return success;
}

bool MCAReadCalibration(uint16_t *MCACalibration)
{
	uint8_t data_byte;
    uint16_t data_word;
	bool success;
	
	success = EEPROMRead(0, EEPROM_ADDR_MCA_CALIBRATION, &data_byte);
	if (!success)
		goto out;
    data_word = (uint16_t)data_byte*256;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_CALIBRATION + 1, &data_byte);
	if (!success)
		goto out;
    data_word = data_word | (uint16_t)data_byte;
    
	*MCACalibration = data_word;
out:
	return success;
}

bool MCAWriteCalibration(uint16_t MCACalibration)
{
    uint8_t data_byte;
	bool success;

    data_byte = (MCACalibration >> 8);
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_CALIBRATION, data_byte);
    if (!success)
        goto out;
    data_byte = MCACalibration;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_CALIBRATION + 1, data_byte);
    if (!success)
        goto out;
 
out:
	return success;
}

bool MCASetCalibration(void)
{
	bool success = false;
	uint16_t data_word;
	uint16_t MCACalibration;

	printf("\r\n");
	success = MCAReadCalibration(&MCACalibration);
	if (!success)
		goto out;
	printf("MCA calibration: %u\r\n", MCACalibration);
	if (WaitForValidUInt16WithLimits("Enter new MCA calibration: ", &data_word, 0, 65535) == INPUT_VALID)
	{
		success = MCAWriteCalibration(data_word);
		success = MCAReadCalibration(&MCACalibration);
		if (success)
			printf("\r\nMCA calibration: %u\r\n", MCACalibration);
	}
	else
		printf("\r\n");
out:
	return success;
}

bool MCAReadTimeStamp(uint32_t *MCATimeStamp)
{
	uint8_t data_byte;
	uint32_t data_lword;
	bool success;
	
	success = EEPROMRead(0, EEPROM_ADDR_MCA_TIME_STAMP, &data_byte);
	if (!success)
		goto out;
	data_lword = (uint32_t)data_byte*256*256*256;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_TIME_STAMP + 1, &data_byte);
	if (!success)
		goto out;
    data_lword = data_lword | (uint32_t)data_byte*256*256;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_TIME_STAMP + 2, &data_byte);
	if (!success)
		goto out;
    data_lword = data_lword | (uint32_t)data_byte*256;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_TIME_STAMP + 3, &data_byte);
	if (!success)
		goto out;
    data_lword = data_lword | (uint32_t)data_byte;

	*MCATimeStamp = data_lword;
out:
	return success;
}

bool MCAWriteTimeStamp(uint32_t MCATimeStamp)
{
	uint8_t data_byte;
	bool success;
	
	data_byte = (MCATimeStamp >> 24);
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_TIME_STAMP, data_byte);
	if (!success)
		goto out;
	data_byte = MCATimeStamp >> 16;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_TIME_STAMP + 1, data_byte);
	if (!success)
		goto out;
	data_byte = MCATimeStamp >> 8;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_TIME_STAMP + 2, data_byte);
	if (!success)
		goto out;
	data_byte = MCATimeStamp;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_TIME_STAMP + 3, data_byte);
	if (!success)
		goto out;
    
out:
	return success;
}

bool MCASetTimeStamp(bool automatic)
{
    const uint32_t TIMESTAMP_LOWER_LIMIT = 0x271326E2;  //  10/09/2020
    const uint32_t TIMESTAMP_UPPER_LIMIT = 0xAAA737D0;  //  10/09/2090
            
	bool success = false;
	uint32_t data_lword;
	uint32_t MCATimeStamp;
    uint32_t LCUTimeStamp;
            
	printf("\r\n");
	success = MCAReadTimeStamp(&MCATimeStamp);
	if (!success)
		goto out;
	printf("Time stamp: %ld\r\n", MCATimeStamp);
    
    if (!automatic)
    {
        if (WaitForValidUInt32WithLimits("Enter new time stamp: ", &data_lword, TIMESTAMP_LOWER_LIMIT, TIMESTAMP_UPPER_LIMIT ) == INPUT_VALID)
        {
            success = MCAWriteTimeStamp(data_lword);
            success = MCAReadTimeStamp(&MCATimeStamp);
            if (success)
                printf("\r\nTime stamp: %ld\r\n", MCATimeStamp);
        }
        else
            printf("\r\n");
    }
    else 
    {
        //Get timestamp based on real time clock and calendar chip, do a sanity check on the value
        success = RTCCGetTimestamp(&LCUTimeStamp);
        if ((LCUTimeStamp < TIMESTAMP_LOWER_LIMIT) || (LCUTimeStamp > TIMESTAMP_UPPER_LIMIT))
        {
            printf("\r\nTime stamp of %ld from LCU is out of range of %ld to %ld\r\n", LCUTimeStamp, TIMESTAMP_LOWER_LIMIT, TIMESTAMP_UPPER_LIMIT);
            printf("\r\n Check that LCU date has been set correctly.");
        }
        
        success = MCAWriteTimeStamp(LCUTimeStamp);
        success = MCAReadTimeStamp(&MCATimeStamp);
        if (success)
        {
            printf("\r\nMCA Time stamp: %ld\r\n", MCATimeStamp);
            printf("\r\nLCU Time stamp: %ld\r\n", LCUTimeStamp);
        }
    }
out:
	return success;
}

bool MCADataIntegrityCheck(void)
{
	bool success = false;
	int i;
	uint8_t data[8];
	uint8_t address;
	uint8_t test_data[] = {0xde, 0xad, 0xbe, 0xef, 0xfa, 0xce, 0x12, 0x34};
	uint8_t xor_checksum;
	uint8_t xor_checksum_reference = 0;
	bool got_checksum = false;

	printf("Reading unique 64-bit ID: ");
	for (i = 0; i < 8; i++)
	{
		success = EEPROMReadSecurity(0, i, &data[i]);
		if (!success)
		{
			printf("ERROR: Could not read MCA EEPROM\r\n");
			goto out;
		}
		printf("%02X ", data[i]);
	}
	success = crc8(data, 8) == 0;
	printf("(CRC is %s)\r\n", success ? "OK" : "INCORRECT");
	if (!success)
		goto out;
	printf("Performing write test: ");
	address = 0x78;
	// Write out a string of 8 test data bytes
	success = EEPROMWritePage(0, address, test_data, 8);
	if (!success)
		goto out;
	// Verify each of the 8 test bytes one by one
	for (i = 0; i < 8; i++)
	{
		success = EEPROMRead(0, 0x78 + i, data);
		if (!success)
			goto out;
		if (data[0] != test_data[i])
		{
			printf("FAILED: Expected 0x%02X but read 0x%02X\r\n", test_data[i], data[0]);
			goto out;
		}
	}
	printf("PASSED\r\n");
	printf("Performing read test (press a key to stop): ");
	got_checksum = false;
	while(!U1STAbits.URXDA)
	{
		xor_checksum = 0;
		for (address = 0; address < 128; address++)
		{
			success = EEPROMRead(0, address, data);
			if (!success)
				goto out;
			xor_checksum ^= data[0];
		}
		if (!got_checksum)
		{
			xor_checksum_reference = xor_checksum;
			got_checksum = true;
		}
		else
		{
			if (xor_checksum != xor_checksum_reference)
			{
				printf("FAILED\r\n");
				goto out;
			}
			else
				printf(".");
		}
	}
	printf("PASSED\r\n");
out:
	return success;
}

bool MCAMemoryRead(void)
{
	bool success = false;
	uint8_t address, data;
    char HexAddress[3];
    char *EndPtr;

    printf("\n\rEnter address (in hex): ");
    ReadString(HexAddress, 3);
    address = strtol(HexAddress, &EndPtr, 16);
  
    if ((address >= 0) && (address <= 127))
    {
        success = EEPROMRead(0, address, &data);
		if (success)
			printf("\n\rData = 0x%02X\r\n", data);
        else
            printf("\n\rread failure! \r\n");
    }
    else
    {
        printf("\n\rAddress out of range of 0 to 7F");
    }
 
	return success;
}

bool MCAMemoryWrite(void)
{
	bool success = false;
	uint8_t address, data;
    char HexAddress[3], HexData[3];
    char *EndPtr;

    printf("\n\rEnter address (in hex): ");
    ReadString(HexAddress, 3);
    address = strtol(HexAddress, &EndPtr, 16);
  
    if ((address >= 0) && (address <= 127))
    {
        printf("\n\rEnter data (in hex): ");
        ReadString(HexData, 3);
        data = strtol(HexData, &EndPtr, 16);
    }
    else
    {
        printf("\n\rAddress out of range of 0 to 7F");
        return success;
    }
    
    success = EEPROMWrite(0, address, data);
    if (success)
        printf("\r\nSuccessfully written.\r\n");    
 
	printf("\r\n");
	return success;
}

bool MCAMemoryErase(void)
{
	bool success = true;
	bool rc;
	uint8_t address;
	uint8_t data[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

	printf("\r\nErasing: ");
	for (address = 0; address < 128; address += 8)
	{
		rc = EEPROMWritePage(0, address, data, 8);
		if (success)
		{
			printf(".");
		}
		else
		{
			printf("x");
			success = false;
		}
	}
	printf("Erasure %s.\r\n", success ? "succeeded" : "failed");
	return success;
}

bool MCALockSerial(void)
{
	printf("\r\nLocking EEPROM\r\n");
	EEPROMLockSecurity(0);
	return false;
}




/* Description:  This function reads the MCA 1-wire EEPROM and the RTCC to determine
 *               the status of the MCA, whether it can be used for therapy or not.
 *               Bit   MCA Status
 *                0    MCA reading error
 *                1    MCA expired
 *                2    MCA period
 * 
 * Preconditions:  1-wire EEPROM communications and I2C communications to RTCC
 * Arguments:  pointer to status byte
 * Returns:  pass/fail indication if function worked
 */
bool MCAGetStatus(uint8_t *MCAStatus)
{
    bool MCAReadingSuccess;    
	uint16_t MCATotalUsageTime;
    uint16_t MCAMaxUsageTime;
	uint32_t MCATimeStamp, LCUTimeStamp;   
    
    

    //Read the LCU's timestamp, MCA's timestamp, the MCA's total usage time (for therapy),
    //and the MCA's maximum usage time (for therapy)
    RTCCGetTimestamp(&LCUTimeStamp);
    MCAReadingSuccess = EEPROMIdentify();
    MCAReadingSuccess = MCAReadingSuccess && MCAReadActiveUseTime(&MCATotalUsageTime);
    MCAReadingSuccess = MCAReadingSuccess && MCAReadMaxUseTime(&MCAMaxUsageTime);
    MCAReadingSuccess = MCAReadingSuccess && MCAReadTimeStamp(&MCATimeStamp);

//    printf("\n\r MCATotalUsageTime = %i", MCATotalUsageTime);
//    printf("\n\r MCATimeStamp = %ld", MCATimeStamp);
//    printf("\n\r LCUTimeStamp = %ld", LCUTimeStamp);
//    printf("\n\r");

    *MCAStatus = 0;
    //Check for MCA EEPROM reading error
    if (!MCAReadingSuccess)   
    {
        *MCAStatus = MCA_READING_ERROR;        
    }
    //Check to make sure this particular MCA has not exceeded the maximum amount of therapy time allowed
    if (MCATotalUsageTime >= MCAMaxUsageTime)
    {
        *MCAStatus |= MCA_EXPIRED;
    }
    // [SDS#7.5.19] Check to make sure this particular MCA was not used within the last minimum period
    if ( (LCUTimeStamp - MCATimeStamp) < 60*60*ReadUISetting(READING_MCA_STATE, SPARE1))
    {
        *MCAStatus |= MCA_PERIOD;
    }
 

    return MCAReadingSuccess;
}