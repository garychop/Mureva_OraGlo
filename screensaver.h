/* 
 * File:   screensaver.h
 * Author: DVelasco
 *
 * Created on December 19, 2018, 3:51 PM
 */

#ifndef SCREENSAVER_H
#define	SCREENSAVER_H

#ifdef	__cplusplus
extern "C" {
#endif

// To be called by user interface routines
void ScreenSaverInit(uint32_t image_id, uint16_t time_before_screensaver_starts,
                     uint16_t max_screensaver_time, uint16_t screensaver_refresh_interval);
void EnableScreenSaver(void);
void DisableScreenSaver(void);
void ExecuteScreenSaverMode(void);

// To be called by real-time clock routines
void ActivateScreenSaver(bool enable);
void ActivateBlankScreen(void);
bool ScreenSaverIsActive(void);
bool ScreenShouldBeBlank(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SCREENSAVER_H */

