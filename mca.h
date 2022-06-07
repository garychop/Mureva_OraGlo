/*
 * MCA parameter management
 */


#define MCA_SERIAL_NUM_LEN					9

// Bit assignments for MCA status
#define MCA_READING_ERROR  0x0001    //error reading the 1-wire EEPROM on the MCA
#define MCA_EXPIRED        0x0002    //the MCA has been used too many times and cannot be used again
#define MCA_PERIOD         0x0004    //the MCA is being used too soon from the last time it was used

#define MCA_ERROR          (MCA_READING_ERROR | MCA_EXPIRED | MCA_PERIOD) //0x0007    //Any error

#define MCA_TIME_LIMIT (43200)          // 12 hours in seconds.
#define MCA_MINIMUM_THERAPY_TIME (270)  // 4:30 in seconds.
#define MCA_THERAPY_NOT_STARTED_TIME (0)// Indicate that therapy has not started.
#define MCA_THERAPY_COMPLETE_TIME (300)// 5:00 in seconds.


bool MCADisplayContents(void);
//
bool MCAReadSerialNumber(uint8_t *MCASerialNumber);
bool MCAWriteSerialNumber(uint8_t *MCASerialNumber);
bool MCASetSerialNumber(void);
//
bool MCAReadCompletedTherapies (uint16_t *completedTherpies);
bool MCAWriteCompletedTherapies (uint16_t completedTherpies);
bool MCAEnterCompletedTherapies(void);
//
bool MCAReadElapsedTherapyTime(uint16_t *elapsedTherapyTime);
bool MCAWriteElapsedTherapyTime(uint16_t elapsedTherapyTime);
bool MCASetElapsedTherapyTime(void);    // Diagnostic tool to set time in MCA
bool MCAIncrementWriteElapsedTherapyTime(void);
//
bool MCAReadMaxNumOfTherapies(uint16_t *maxNumOfTherapies);
bool MCAWriteMaxNumOfTherapies(uint16_t maxNumOfTherapies);
bool MCAEnterMaxNumOfTherapies(void);
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

