/* 
 * File:   tachometer.h
 * Author: DVelasco
 *
 * Created on November 17, 2018, 11:33 AM
 */

#ifndef TACHOMETER_H
#define	TACHOMETER_H

#ifdef	__cplusplus
extern "C" {
#endif

bool TestFan(int *rpm, char *message);
void DetectedStalledTachometer(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TACHOMETER_H */

