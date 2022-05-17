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

#define RTCC_INIT_STRING "MuReva RTCC INIT"

typedef struct rtcsec
{
	uint8_t	secone: 4;
	uint8_t	secten: 3;
	uint8_t	st: 1;
} rtcsec_t;

typedef struct rtcmin
{
	uint8_t	minone: 4;
	uint8_t	minten: 3;
	uint8_t	unused: 1;
} rtcmin_t;

typedef struct rtchour
{
	uint8_t	hrone: 4;
	uint8_t	hrten: 2;
	uint8_t	hrfmt: 1;
	uint8_t	unused: 1;
} rtchour_t;

typedef struct rtcwkday
{
	uint8_t	wkday: 3;
	uint8_t	vbaten: 1;
	uint8_t	pwrfail: 1;
	uint8_t	oscrun: 1;
	uint8_t	unused: 2;
} rtcwkday_t;

typedef struct rtcdate
{
	uint8_t	dateone: 4;
	uint8_t	dateten: 2;
	uint8_t	unused: 2;
} rtcdate_t;

typedef struct rtcmth
{
	uint8_t	mthone: 4;
	uint8_t	mthten: 1;
	uint8_t	lpyr: 1;
	uint8_t	unused: 2;
} rtcmth_t;

typedef struct rtcyear
{
	uint8_t	yrone: 4;
	uint8_t	yrten: 4;
} rtcyear_t;

typedef struct rtcc_data
{
	rtcsec_t	rtcsec;
	rtcmin_t	rtcmin;
	rtchour_t	rtchour;
	rtcwkday_t	rtcwkday;
	rtcdate_t	rtcdate;
	rtcmth_t	rtcmth;
	rtcyear_t	rtcyear;
} rtcc_data_t;

bool RTCCIdentify(void);
bool RTCCInit(void);
bool RTCCGetStartStatus(void);
bool RTCCSetStartStatus(bool start);
bool RTCCDisplayDateTime(void);
bool RTCCGetDateTimeData(rtcc_data_t *r);
bool RTCCSetDateTimeData(rtcc_data_t *r);
void RTCCPrint(rtcc_data_t *r);
bool RTCCSetTime(void);
bool RTCCSetDate(void);
bool RTCCGetVBATEN(void);
bool RTCCSetVBATEN(bool enable);
bool RTCCGetPWRFAIL(void);
bool RTCCClearPWRFAIL(void);
bool RTCCDisplayPowerFailStatus(void);
bool RTCCDumpRegisters(void);
bool RTCCReadRegister(void);
bool RTCCWriteRegister(void);
bool RTCCCalcLCUTimestamp(rtcc_data_t *r, uint32_t *LCUTimeStamp);
bool RTCCGetTimestamp(uint32_t *LCUTimeStamp);
bool RTCCInitSRAM(void);
bool RTCCGetSRAM(char *String);