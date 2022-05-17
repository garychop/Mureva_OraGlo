/*
 * MCA parameter management
 */


#define MCA_SERIAL_NUM_LEN					9

// Bit assignments for MCA status
#define MCA_READING_ERROR  0x0001    //error reading the 1-wire EEPROM on the MCA
#define MCA_EXPIRED        0x0002    //the MCA has been used too many times and cannot be used again
#define MCA_PERIOD         0x0004    //the MCA is being used too soon from the last time it was used

#define MCA_ERROR          0x0007    //Any error


bool MCADisplayContents(void);
//
bool MCAReadSerialNumber(uint8_t *MCASerialNumber);
bool MCAWriteSerialNumber(uint8_t *MCASerialNumber);
bool MCASetSerialNumber(void);
//
bool MCAReadActiveUseTime(uint16_t *MCATotalUsageTime);
bool MCAWriteActiveUseTime(uint16_t MCATotalUsageTime);
bool MCASetActiveUseTime(void);
bool MCAIncActiveUseTime(void);
//
bool MCAReadMaxUseTime(uint16_t *MCAMaxUsageTime);
bool MCAWriteMaxUseTime(uint16_t MCAMaxUsageTime);
bool MCASetMaxUseTime(void);
//
bool MCAReadCalibration(uint16_t *MCAMCACalibration);
bool MCAWriteCalibration(uint16_t MCAMCACalibration);
bool MCASetCalibration(void);
//
bool MCAReadTimeStamp(uint32_t *MCATimeStamp);
bool MCAWriteTimeStamp(uint32_t MCATimeStamp);
bool MCASetTimeStamp(bool automatic);
//
bool MCADataIntegrityCheck(void);
bool MCAMemoryRead(void);
bool MCAMemoryWrite(void);
bool MCAMemoryErase(void);
bool MCALockSerial(void);
bool MCAGetStatus(uint8_t *MCAStatus);

