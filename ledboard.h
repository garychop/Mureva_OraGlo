/* 
 * File:   ledboard.h
 * Author: DVelasco
 *
 * Created on November 13, 2018, 4:43 PM
 */

#ifndef LEDBOARD_H
#define	LEDBOARD_H

#ifdef	__cplusplus
extern "C" {
#endif


bool TestLEDBoardEEPROM(char *message, bool *timeout);
bool TestLEDBoardTemp(float *temperature, char *message, bool *timeout);
bool TestLEDCurrent(float expected, float *measured, char *message);
bool TestLEDCurrentOffset(char *message);
void WriteLEDBoardEEPROM(int address, const char *data, int num_bytes, bool *timeout);
void ReadLEDBoardEEPROM(char *data, bool *timeout);
float ReadLEDVoltage(void);
void WriteLEDCurrent(float current_level, bool print_out);


bool LEDCurrentOn(void);

uint16_t ReadLEDBoardSpecifiedCurrent(void);
void LEDBoardEEPROMSubmenu(void);
void LEDBoardEEPROMPrintNotes(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LEDBOARD_H */

