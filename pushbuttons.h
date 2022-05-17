/* 
 * File:   pushbuttons.h
 * Author: DVelasco
 *
 * Created on November 9, 2018, 5:02 PM
 */

#ifndef PUSHBUTTONS_H
#define	PUSHBUTTONS_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum 
{
    PUSHBUTTON_1 = 0,
    PUSHBUTTON_2 = 1,
} PushButton_t;

bool PushbuttonPressed(PushButton_t button);
bool TestPushbutton(bool interactive_test, char *message);

#ifdef	__cplusplus
}
#endif

#endif	/* PUSHBUTTONS_H */

