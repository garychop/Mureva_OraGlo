/* 
 * File:   lcd.h
 * Author: DVelasco
 *
 * Created on December 5, 2018, 12:51 PM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t xpos;
    uint16_t ypos;
    uint16_t xdim;
    uint16_t ydim;
} rect;

void InitializeLCDDMA(void);

//Functions from crystalfontz example code for CFAF320240F-035T
void LCDWriteCommand(uint8_t command);
void LCDWriteData(uint16_t data);
void InitializeLCD(void);
void LCDDisplayHome(void);
void DisplayTestImage(void);

void DisplayText(const char *p_String, uint16_t xpos, uint16_t ypos);
void BlankOutDisplay(bool complete_blankout);
    
//Functions to update LCD with images
void WriteImageToLCD(uint32_t identifier, bool erase_previous, bool print_out);
void DisplayImageAtRandomLocation(uint32_t identifier);

bool TestLCD(char *message, bool post);

void LCDTest(void); // Temporary debugging/diagnostic func

void TurnBacklightOff(void);
void TurnBacklightOn(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

