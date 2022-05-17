/* 
 * File:   hw_watchdog.h
 * Author: DVelasco
 *
 * Created on November 19, 2018, 11:20 AM
 */

#ifndef HW_WATCHDOG_H
#define	HW_WATCHDOG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
// For reference, the HW_WDOG_Status has the following bit assignments
#define MOUTHPIECE_ATTACHED       0x0400
#define THERMISTOR_TEMP_TOO_HIGH  0x0200
#define HW_WDOG_TIMEOUT           0x0100
#define ENABLE_HW_WDOG            0x0080
#define THERAPY_ON                0x0040
#define START_WDOG                0x0020
#define CURRENT_SOURCE_FAULT      0x0010 // Only needed when therapy is on
#define HW_DETECTED_ERROR         0x0008
#define CURRENT_TOO_HIGH          0x0004
#define CURRENT_TOO_LOW           0x0002
#define SW_WDOG_TIMEOUT           0x0001
    
// Exclude MOUTHPIECE_ATTACHED since this never specifically
// indicates an error condition 
#define ALL_WDOG_ERROR_CONDITIONS (THERMISTOR_TEMP_TOO_HIGH | HW_WDOG_TIMEOUT | \
                                   ENABLE_HW_WDOG | THERAPY_ON | START_WDOG | \
                                   CURRENT_SOURCE_FAULT | HW_DETECTED_ERROR | \
                                   CURRENT_TOO_HIGH | CURRENT_TOO_LOW | \
                                   SW_WDOG_TIMEOUT)
// CURRENT_SOURCE_FAULT is only needed when therapy is on. It can be
// triggered when when the output voltage is too low, such as when
// therapy is on, but that is not a concern.
#define BASIC_DEVICE_READY_ERROR (THERMISTOR_TEMP_TOO_HIGH | HW_WDOG_TIMEOUT | \
                    HW_DETECTED_ERROR | SW_WDOG_TIMEOUT) 
    

void InitializeHW_WatchdogData();
bool TestHWWdog(uint16_t *status, char *message, bool print_out);

#ifdef	__cplusplus
}
#endif

#endif	/* HW_WATCHDOG_H */

