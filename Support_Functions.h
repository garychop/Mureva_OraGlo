//	Filename:  Support_Functions.h
//	Author:  Paul Wittibschlager
//	Date Created:  08/27/2018
//	Company:  Valtronic, USA
//	Project:   Lumitex OraGlo Control Board
//	Project number:  DD400004
//	Description:  This file contains general functions to support the OraGlo project

#ifndef SUPPORT_FUNCTIONS_H
#define	SUPPORT_FUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum
{
    INPUT_VALID,
    INPUT_INVALID,
    INPUT_CANCELED        
} input_status_t;

#define CR ((char) 13)  // ASCII code for carriage return

char GetChar(void);
bool GetYesOrNo(void);
int16_t GetZeroOneOrCancel(void);
char GetCharWithinLimits(char lower_bound, char upper_bound);
uint16_t ReadUInt16(void);
uint32_t ReadUInt32(void);
input_status_t ReadUInt16OrCancel(uint16_t *value);
input_status_t ReadUInt32OrCancel(uint32_t *value);
input_status_t WaitForValidUInt16(const char *msg, uint16_t *value);
input_status_t WaitForValidUInt32(const char *msg, uint32_t *value);
input_status_t WaitForValidUInt16WithLimits(const char *msg, uint16_t *value,
                                 uint16_t lower_bound, uint16_t upper_bound);
input_status_t WaitForValidUInt32WithLimits(const char *msg, uint32_t *value,
                                 uint32_t lower_bound, uint32_t upper_bound);
double ReadFloat(void);
input_status_t ReadFloatOrCancel(float *value);
input_status_t WaitForValidFloat(const char *msg, float *value);
int ReadString(char *p_str, uint16_t max_length);

void DisableNonDMAInterrupts(void);
void EnableNonDMAInterrupts(void);
uint8_t crc8(uint8_t *data, uint16_t len);
size_t strnlen(const char *s, size_t maxlen);

#ifdef	__cplusplus
}
#endif

#endif	/* SUPPORT_FUNCTIONS_H */
