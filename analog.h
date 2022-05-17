/* 
 * File:   analog.h
 * Author: DVelasco
 *
 * Created on November 13, 2018, 5:24 PM
 */

#ifndef ANALOG_H
#define	ANALOG_H

#ifdef	__cplusplus
extern "C" {
#endif

#define ADC_REF 2.5

float GetADCReadingCh8(void);
float GetADCReadingCh9(void);
float GetADCReadingCh10(void);
float GetADCReadingCh11(void);
float GetADCReadingCh12(void);
void ClearADCCh11Offset(void);
void SetADCCh11Offset(void);

void WriteDAC(float value_from, float value_to, bool print_out);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

