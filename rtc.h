/* 
 * File:   rtc.h
 * Author: DVelasco
 *
 * Created on November 16, 2018, 1:53 PM
 */

#ifndef RTC_H
#define	RTC_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdbool.h>

bool TimeToUpdateDisplay(uint16_t *timer_val, bool *first_minute);
void StartGenericCountdownTimer(uint16_t num_seconds);
void ClearGenericCountdownTimer(void);
bool GenericCountdownTimerElapsed(void);
void WaitForGenericCountdownTimer(uint16_t restart_value);

void StartOperationStateCountdown(uint16_t num_seconds);
void PauseOperationStateCountdown(void);
void ResumeOperationStateCountdown(void);
uint16_t GetOperationStateTimer(void);
bool OperationCountDownNewlyRestarted(void);

bool TimeToPerformBIST(void);

void ResetScreenSaverTimer(uint16_t time_before_screensaver_starts,
                           uint16_t max_screensaver_time);
void StopScreenSaverTimer(void);
bool ScreenSaverIsActive(void);
bool ScreenToBeBlanked(void);

#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */

