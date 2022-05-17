/* 
 * File:   timers.h
 * Author: DVelasco
 *
 * Created on November 9, 2018, 9:55 AM
 */

#ifndef TIMERS_H
#define	TIMERS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define ONE_SEC_DELAY  62500
    
void TimerDelay(uint16_t time);
void StartTimer(uint16_t time);
void WaitForTimer(void);
bool TimerDone(void);
void ClearTimer(void);
void SetLedBlinkPattern(uint16_t red1, uint16_t green1, uint16_t blue1,
                        uint16_t time1, uint16_t red2, uint16_t green2,
                        uint16_t blue2, uint16_t time2);
void DisableLedBlinking(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMERS_H */

