/* 
 * File:   flashmem.h
 * Author: DVelasco
 *
 * Created on November 9, 2018, 3:21 PM
 */

#ifndef FLASHMEM_H
#define	FLASHMEM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "dma.h"

bool TestFlash(uint16_t *data, char *message);
uint32_t ReadFlashStatus(bool print_out);
void ReadFlash(uint32_t address, uint32_t *p_data, uint32_t num_longs, bool print_out);
void ReadFlashWordsSwapped(uint32_t address, uint32_t *p_data, uint32_t num_longs);
void EraseFlashPages(uint32_t page_from, uint32_t page_to);
void WriteFlash(uint32_t address, uint32_t *p_data, uint32_t num_longs, bool print_out);
void EraseDataFromFlash(void);
void EraseDataFromFlashInPages(void);
void ReadDataFromFlash(void);
void WriteDataToFlash(void);
void WriteImageToFlash(void);

void ReadFlashDMA(uint32_t address, uint16_t *data, uint16_t num_ints);

void PrintLEDBoardEEPROMDefaultsFromFlash(void);
void StoreLEDBoardEEPROMDefaultsToFlash(void);
void ReadLEDBoardEEPROMDefaultsFromFlash(uint32_t *data);
void StoreLEDBoardDefaultCurrentToFlash(void);

#ifdef	__cplusplus
}
#endif

#endif	/* FLASHMEM_H */

