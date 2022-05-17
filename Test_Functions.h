//	Filename:  Test_Functions.h
//	Author:  Paul Wittibschlager
//	Date Created:  08/27/2018
//	Company:  Valtronic, USA
//	Project:   Lumitex OraGlo Control Board
//	Project number:  DD400004
//	Description:  This file contains functions to support power on self test (POST),
//                built in self test (BIST) and the test interface (TI)

#ifndef TEST_FUNCTIONS_H
#define	TEST_FUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Bit assignments for the non-watchdog test results
#define SUPPLY_24VDC_VALID      0x0001
#define SUPPLY_5VDC_VALID       0x0002
#define SUPPLY_33VDC_VALID      0x0004
#define ALARM_WORKS             0x0008
#define RGB_LED_WORKS           0x0010
#define LCD_WORKS               0x0020
#define FAN_WORKS               0x0040
#define FLASH_MEM_WORKS         0x0080
#define CTRL_BOARD_TEMP_WORKS   0x0100
#define LED_BOARD_EEPROM_WORKS  0x0200
#define LED_BOARD_TEMP_WORKS    0x0400
#define PUSHBUTTON_WORKS        0x0800
#define HW_WATCHDOG_TEST_PASSED 0x1000   
#define LED_CURRENT_ACCURATE    0x2000   
#define I2C_DIDNT_TIMEOUT       0x4000
#define BATTERY_VALID           0x8000    

#define ALL_NONWATCHDOG_RESULTS (SUPPLY_24VDC_VALID  | SUPPLY_5VDC_VALID |\
                                 SUPPLY_33VDC_VALID | ALARM_WORKS | \
                                 RGB_LED_WORKS | LCD_WORKS | FAN_WORKS | \
                                 FLASH_MEM_WORKS | CTRL_BOARD_TEMP_WORKS | \
                                 LED_BOARD_EEPROM_WORKS | \
                                 LED_BOARD_TEMP_WORKS | PUSHBUTTON_WORKS | \
                                 HW_WATCHDOG_TEST_PASSED | LED_CURRENT_ACCURATE | \
                                 I2C_DIDNT_TIMEOUT | BATTERY_VALID )
#define COMMON_TEST_REQUIREMENTS (SUPPLY_24VDC_VALID | SUPPLY_5VDC_VALID | \
                                  SUPPLY_33VDC_VALID | FAN_WORKS | \
                                  CTRL_BOARD_TEMP_WORKS | LED_BOARD_TEMP_WORKS \
                                  | PUSHBUTTON_WORKS | I2C_DIDNT_TIMEOUT )
#define POST_REQUIREMENTS  (COMMON_TEST_REQUIREMENTS | RGB_LED_WORKS | \
                            LCD_WORKS | FLASH_MEM_WORKS | \
                            LED_BOARD_EEPROM_WORKS | BATTERY_VALID )
#define BIST_REQUIREMENTS  (COMMON_TEST_REQUIREMENTS | LED_CURRENT_ACCURATE)
    

void TestInterface(char cmd);
void PrintMenu(void);
bool TestPOST(int num_iterations, uint16_t *test_results, bool print_out);
bool TestBIST(int num_iterations, uint16_t *test_results, bool print_out);

bool Test5VDC(float *voltage, char *message);

#ifdef	__cplusplus
}
#endif

#endif	/* TEST_FUNCTIONS_H */

