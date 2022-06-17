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
#define EEPROM_ADDR_MCA_CURRENT_THERAPY_ELAPSED_TIME	0x00    //requires 16 bits
#define EEPROM_ADDR_MCA_MAX_NUM_OF_THERAPIES          	0x02    //requires 16 bits
#define EEPROM_ADDR_MCA_CALIBRATION                     0x04    //requires 16 bits
#define EEPROM_ADDR_MCA_TIME_STAMP                      0x06    //requires 32 bits
#define EEPROM_ADDR_MCA_NUM_OF_COMPLETED_THERAPIES      0x0a    //uses 16 bits
// The serial number is stored in the security register
#define EEPROM_ADDR_MCA_SERIAL_NUM                      0x10

bool MCADisplayContents(void)
{
	bool success = true;
	bool error = false;
	uint8_t data[MCA_SERIAL_NUM_LEN + 1];
	uint16_t elapsedTherapyTime;
	uint16_t maxNumOfTherapies;
    uint16_t completedTherapies;
	uint16_t MCACalibration;
	uint32_t MCATimeStamp;
    

	printf("\r\nMCA EEPROM contents:\r\n");
	success = MCAReadSerialNumber(data);
	if (success)
		printf("    Serial number: %s\r\n", data);
	else
		error = true;
	success = MCAReadCalibration(&MCACalibration);
	if (success)
		printf("    MCA calibration: %u  =>  %.2f\r\n", MCACalibration, (double)MCACalibration/32768 );
	else
		error = true;
	success = MCAReadElapsedTherapyTime(&elapsedTherapyTime);
	if (success)
		printf("    Elapsed Therapy Time (S): %u\r\n", elapsedTherapyTime);
	else
		error = true;
	success = MCAReadMaxNumOfTherapies(&maxNumOfTherapies);    
	if (success)
		printf("    Maximum Number of Therapies: %u\r\n", maxNumOfTherapies);
	else
		error = true;
	success = MCAReadCompletedTherapies(&completedTherapies);    
	if (success)
		printf("    Completed Therapies: %u\r\n", completedTherapies);
	else
		error = true;
	success = MCAReadTimeStamp(&MCATimeStamp);
	if (success)
		printf("    Time stamp (S): %ld\r\n", MCATimeStamp);
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
    		return false;
		}
	}
	success = EEPROMWriteSecurity(0, EEPROM_ADDR_MCA_SERIAL_NUM + MCA_SERIAL_NUM_LEN, crc8(MCASerialNumber, strnlen((char *)MCASerialNumber, MCA_SERIAL_NUM_LEN)));
	if (!success)
	{
		printf("ERROR: Could not write serial number CRC to MCA EEPROM security area\r\n");
		return false;
	}

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

/*
 * This function returns the Completed Number of Therapies from the
 * MCA. 
 * Returns: true if successful, else false
 */
bool MCAReadCompletedTherapies (uint16_t *completedTherpies)
{
	uint8_t data_byte;
	uint16_t data_word;
	bool success;
	
	success = EEPROMRead(0, EEPROM_ADDR_MCA_NUM_OF_COMPLETED_THERAPIES, &data_byte);
	if (!success)
		return false;
	data_word = (uint16_t) data_byte << 8;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_NUM_OF_COMPLETED_THERAPIES + 1, &data_byte);
	if (!success)
		return false;
	data_word += data_byte;
	*completedTherpies = data_word;

	return success;
}

/*
 * This function saves (writes) the Completed Number of Therapies into the
 * MCA. 
 * Returns: true if successful, else false
 */
bool MCAWriteCompletedTherapies (uint16_t completedTherpies)
{
	uint8_t data_byte;
	bool success;
	
	data_byte = (completedTherpies >> 8) & 0xff;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_NUM_OF_COMPLETED_THERAPIES, data_byte);
	if (!success)
		return false;
	data_byte = completedTherpies & 0xff;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_NUM_OF_COMPLETED_THERAPIES + 1, data_byte);
	//if (!success)
	//	return false;

	return success;
}

/*
 * This function prompts the user to enter a new Completed Number of Therapies
 * and stores the value in teh MCA. 
 * Returns: true if successful, else false
 */
bool MCAEnterCompletedTherapies(void)
{
	bool success = false;
	uint16_t entered_value;
	uint16_t completedTherpies;

	printf("\r\n");
	success = MCAReadCompletedTherapies(&completedTherpies);
	if (!success)
		return false;
	printf("Number of Completed Therapies: %u\r\n", completedTherpies);
	if (WaitForValidUInt16WithLimits("Enter New Number of Completed Therapies: ", &entered_value, 0, 60000) == INPUT_VALID)
	{
		success = MCAWriteCompletedTherapies(entered_value);
		success = MCAReadCompletedTherapies(&completedTherpies);
		if (success)
			printf("\r\nNumber of Completed Therapies: %u\r\n", completedTherpies);
	}
	else
		printf("\r\n");

	return success;
}

bool MCAReadElapsedTherapyTime(uint16_t *elapsedTherapyTime)
{
	uint8_t data_byte;
	uint16_t data_word;
	bool success;
	
	success = EEPROMRead(0, EEPROM_ADDR_MCA_CURRENT_THERAPY_ELAPSED_TIME, &data_byte);
	if (!success)
		return false;
	data_word = (uint16_t) data_byte << 8;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_CURRENT_THERAPY_ELAPSED_TIME + 1, &data_byte);
	if (!success)
		return false;
	data_word += data_byte;
	*elapsedTherapyTime = data_word;

	return success;
}

/*
 *  This function writes the passed int16 to the MCA EEPROM in the 
 * Elapsed Therapy location.
 */

bool MCAWriteElapsedTherapyTime(uint16_t elapsedTherapyTime)
{
	uint8_t data_byte;
	bool success;
	
	data_byte = (elapsedTherapyTime >> 8) & 0xff;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_CURRENT_THERAPY_ELAPSED_TIME, data_byte);
	if (!success)
		return false;
	data_byte = elapsedTherapyTime & 0xff;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_CURRENT_THERAPY_ELAPSED_TIME + 1, data_byte);
	//if (!success)
	//	return false;

	return success;
}

/*
 * This function increments the Elapsed Therapy time in the MCA.
 */
bool MCAIncrementWriteElapsedTherapyTime(void)
{
    bool success;
    uint8_t Iteration;
    uint16_t elapsedTherapyTime;
    uint16_t elapsedTherapyTimePlusOne;
    uint8_t EEPROM_ReadErrorCounterMax = 5; // [SDS#7.5.16]
    
    //Read the total usage time, try several times if there are failures
    Iteration = 1;
    success = false;
    while(!success)
    {
        success = MCAReadElapsedTherapyTime(&elapsedTherapyTime);
        if(Iteration >= EEPROM_ReadErrorCounterMax)
            return false;   //we can't read
        Iteration++;
    }
        
    //Increment the total usage time and write it back, try several times if there are failures   
    EEPROM_ReadErrorCounterMax = 5; // [SDS#7.5.16]
    Iteration = 1;
    success = false;
    while(!success)
    {
        success = MCAWriteElapsedTherapyTime(elapsedTherapyTime + 1);
        if(Iteration >= EEPROM_ReadErrorCounterMax)
            return false;   //we can't write
        Iteration++;
    }
    
     //Read back the new value and verify it is expected   
    Iteration = 1;
    EEPROM_ReadErrorCounterMax = 5; // [SDS#7.5.16]
    success = false;
    while(!success)
    {
        success = MCAReadElapsedTherapyTime(&elapsedTherapyTimePlusOne);
        if(Iteration >= EEPROM_ReadErrorCounterMax)
            return false;   //we can't read it back
        Iteration++;
    }

    // Check to see if we updated the MCA with the right information.
    if (elapsedTherapyTimePlusOne == (elapsedTherapyTime + 1))
        return true;
    else
        return false;
}

/*
 * This Diagnostic function sets the Elapsed time of current Therapy Session
 */

bool MCASetElapsedTherapyTime(void)
{
	bool success = false;
	uint16_t data_word;
	uint16_t timeValue;

	printf("\r\n");
	success = MCAReadElapsedTherapyTime(&timeValue);
	if (!success)
		return false;
	printf("Elapsed time of Current Therapy (s): %u\r\n", timeValue);
	if (WaitForValidUInt16WithLimits("Enter new Elapsed Therapy Time: ", &data_word, 0, 60000) == INPUT_VALID)
	{
		success = MCAWriteElapsedTherapyTime(data_word);
		success = MCAReadElapsedTherapyTime(&timeValue);
		if (success)
			printf("\r\nElapsed Therapy time (s): %u\r\n", timeValue);
	}
	else
		printf("\r\n");

	return success;
}

bool MCAReadMaxNumOfTherapies(uint16_t *maxNumOfTherapies)
{
	uint8_t data_byte;
    uint16_t data_word;
	bool success;
	
    *maxNumOfTherapies = 41;    // Return the "default" number of therapies allowed
	success = EEPROMRead(0, EEPROM_ADDR_MCA_MAX_NUM_OF_THERAPIES, &data_byte);
	if (!success)
		return false;
    data_word = (uint16_t)data_byte << 8;   // Put in upper byte
	success = EEPROMRead(0, EEPROM_ADDR_MCA_MAX_NUM_OF_THERAPIES + 1, &data_byte);
	if (!success)
		return false;
    data_word = data_word | (uint16_t)data_byte;
    
	*maxNumOfTherapies = data_word;

	return success;
}

bool MCAWriteMaxNumOfTherapies(uint16_t maxNumOfTherapies)
{
    uint8_t data_byte;
	bool success;

    data_byte = (maxNumOfTherapies >> 8);
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_MAX_NUM_OF_THERAPIES, data_byte);
    if (!success)
        return false;
    data_byte = maxNumOfTherapies;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_MAX_NUM_OF_THERAPIES + 1, data_byte);
    //if (!success)
    //    return false;
    
	return success;
}

bool MCAEnterMaxNumOfTherapies(void)
{
	bool success = false;
	uint16_t data_word;
	uint16_t maxNumOfTherapies;

	printf("\r\n");
	success = MCAReadMaxNumOfTherapies(&maxNumOfTherapies);
	if (!success)
		return false;
	printf("Max Number of Therapies: %u\r\n", maxNumOfTherapies);
	if (WaitForValidUInt16WithLimits("Enter new Max Number of Therapies: ", &data_word, 0, 65535) == INPUT_VALID)
	{
		success = MCAWriteMaxNumOfTherapies(data_word);
		success = MCAReadMaxNumOfTherapies(&maxNumOfTherapies);
		if (success)
			printf("\r\nMax Number of Therapies: %u\r\n", maxNumOfTherapies);
	}
	else
		printf("\r\n");
	return success;
}

bool MCAReadCalibration(uint16_t *MCACalibration)
{
	uint8_t data_byte;
    uint16_t data_word;
	bool success;
	
	success = EEPROMRead(0, EEPROM_ADDR_MCA_CALIBRATION, &data_byte);
	if (!success)
		return false;
    data_word = (uint16_t)data_byte*256;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_CALIBRATION + 1, &data_byte);
	if (!success)
		return false;
    data_word = data_word | (uint16_t)data_byte;
    
	*MCACalibration = data_word;

	return success;
}

bool MCAWriteCalibration(uint16_t MCACalibration)
{
    uint8_t data_byte;
	bool success;

    data_byte = (MCACalibration >> 8);
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_CALIBRATION, data_byte);
    if (!success)
        return false;
    data_byte = MCACalibration;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_CALIBRATION + 1, data_byte);
    //if (!success)
    //    return false;
 
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
		return false;
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

	return success;
}

bool MCAReadTimeStamp(uint32_t *MCATimeStamp)
{
	uint8_t data_byte = 0;
	uint32_t data_lword = 0l;
	bool success;
	
    *MCATimeStamp = 0;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_TIME_STAMP, &data_byte);
	if (!success)
		return false;
	data_lword = (uint32_t)data_byte*256*256*256;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_TIME_STAMP + 1, &data_byte);
	if (!success)
		return false;
    data_lword = data_lword | (uint32_t)data_byte*256*256;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_TIME_STAMP + 2, &data_byte);
	if (!success)
		return false;
    data_lword = data_lword | (uint32_t)data_byte*256;
	success = EEPROMRead(0, EEPROM_ADDR_MCA_TIME_STAMP + 3, &data_byte);
	if (!success)
		return false;
    data_lword = data_lword | (uint32_t)data_byte;

	*MCATimeStamp = data_lword;

	return success;
}

bool MCAWriteTimeStamp(uint32_t MCATimeStamp)
{
	uint8_t data_byte;
	bool success;
	
	data_byte = (MCATimeStamp >> 24);
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_TIME_STAMP, data_byte);
	if (!success)
		return false;
	data_byte = MCATimeStamp >> 16;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_TIME_STAMP + 1, data_byte);
	if (!success)
		return false;
	data_byte = MCATimeStamp >> 8;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_TIME_STAMP + 2, data_byte);
	if (!success)
		return false;
	data_byte = MCATimeStamp;
	success = EEPROMWrite(0, EEPROM_ADDR_MCA_TIME_STAMP + 3, data_byte);
	//if (!success)
	//	return false;
    
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
		return false;
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
    		return false;
		}
		printf("%02X ", data[i]);
	}
	success = crc8(data, 8) == 0;
	printf("(CRC is %s)\r\n", success ? "OK" : "INCORRECT");
	if (!success)
		return false;
	printf("Performing write test: ");
	address = 0x78;
	// Write out a string of 8 test data bytes
	success = EEPROMWritePage(0, address, test_data, 8);
	if (!success)
		return false;
	// Verify each of the 8 test bytes one by one
	for (i = 0; i < 8; i++)
	{
		success = EEPROMRead(0, 0x78 + i, data);
		if (!success)
    		return false;
		if (data[0] != test_data[i])
		{
			printf("FAILED: Expected 0x%02X but read 0x%02X\r\n", test_data[i], data[0]);
    		return false;
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
        		return false;
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
           		return false;
			}
			else
				printf(".");
		}
	}
	printf("PASSED\r\n");

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
 *                1    MCA expired, max number of therapies achieved
 *                2    MCA period, attempting to perform another therapy within 12 hours.
 * 
 * Preconditions:  1-wire EEPROM communications and I2C communications to RTCC
 * Arguments:  pointer to status byte
 * Returns:  pass/fail indication if function worked
 */
bool MCAGetStatus(uint8_t *MCAStatus)
{
    bool MCAReadingSuccess;    
	uint16_t elapsedTherapyTime = 0;
    uint16_t maxNumOfTherapies = 0;
    uint16_t completedTherapies = 0;
	uint32_t MCATimeStamp, LCUTimeStamp;   
    
    

    // Read the LCU's timestamp, MCA's timestamp, the MCA's Completed Therapies,
    // the Max Number of Therapies and the elapsed time.
    RTCCGetTimestamp(&LCUTimeStamp);
    MCAReadingSuccess = EEPROMIdentify();
    MCAReadingSuccess = MCAReadingSuccess && MCAReadElapsedTherapyTime(&elapsedTherapyTime);
    MCAReadingSuccess = MCAReadingSuccess && MCAReadMaxNumOfTherapies(&maxNumOfTherapies);
    MCAReadingSuccess = MCAReadingSuccess && MCAReadCompletedTherapies(&completedTherapies);
    MCAReadingSuccess = MCAReadingSuccess && MCAReadTimeStamp(&MCATimeStamp);

    printf("\n\r MCA Completed Therapies = %d", completedTherapies);
    printf("\n\r MCA Elapsed Therapy Time = %d", elapsedTherapyTime);
    printf("\n\r MCA Max Number of Therapies = %d", maxNumOfTherapies);
    printf("\n\r MCA Last Time Stamp = %ld", MCATimeStamp);
    printf("\n\r LCUTimeStamp = %ld,     diff = %ld", LCUTimeStamp, LCUTimeStamp - MCATimeStamp);
    printf("\n\r");

    *MCAStatus = 0;
    //Check for MCA EEPROM reading error
    if (!MCAReadingSuccess)   
    {
        *MCAStatus = MCA_READING_ERROR;
        return false;
    }
    //Check to make sure this particular MCA has not exceeded the maximum amount of therapy time allowed
    if (completedTherapies >= maxNumOfTherapies)
    {
        *MCAStatus |= MCA_EXPIRED;
        return MCAReadingSuccess;    // No need to look any further.
    }
    // Check to see if the therapy has NOT been started for today. If not started
    // then simply return with an OK status.
    if (elapsedTherapyTime == MCA_THERAPY_NOT_STARTED_TIME)
        return MCAReadingSuccess;    // Return with NO MCA errors and OK to use.
    
    // [SDS#7.5.19] Check to make sure this particular MCA was not used within the last minimum period
    if ((LCUTimeStamp - MCATimeStamp) < (uint32_t)MCA_TIME_LIMIT)  // 12 hours in seconds. 60*60*ReadUISetting(READING_MCA_STATE, SPARE1))
    {
        // If its within 12 hours and the Therapy has been completed,
        // return a MCA_PERIOD error.
        if (elapsedTherapyTime == MCA_THERAPY_COMPLETE_TIME)
        {
            *MCAStatus |= MCA_PERIOD;
        }
        else
        {   // It's within 12 hours and it's less than a completed therapy.
            // We are going to continue with this therapy session.
            return MCAReadingSuccess;    // Return with NO MCA errors and OK to use.
        }
    }
    else // It's been at least 12 hours, set Elapsed Time and Number of 
        // Completed Therapies.
    {
        // We need to catch when the Mouthpiece was removed during Therapy
        if (elapsedTherapyTime > MCA_THERAPY_NOT_STARTED_TIME)
        {
            if (elapsedTherapyTime < MCA_THERAPY_COMPLETE_TIME)
            {
                // update the EEPROM with a completed therapy
                ++completedTherapies;
                MCAWriteCompletedTherapies (completedTherapies);
                printf("\n\r -----> Changed MCA Completed Therapies = %d", completedTherapies);
                // Indicate that another therapy was completed.
                if (completedTherapies >= maxNumOfTherapies)
                {
                    *MCAStatus |= MCA_EXPIRED;
                    return MCAReadingSuccess;    // No need to look any further.
                }
            }
            MCAWriteElapsedTherapyTime (MCA_THERAPY_NOT_STARTED_TIME); // Set it indicate "Therapy Not Started".
            MCAReadElapsedTherapyTime(&elapsedTherapyTime);
            printf("\n\r ----_> Changed MCA Elapsed Therapy Time = %d", elapsedTherapyTime);
            printf("\n\r");
        }
    } 
    return MCAReadingSuccess;
}