/* 
 * File:   rgbled.h
 * Author: DVelasco
 *
 * Created on February 11, 2019, 2:48 PM
 */

#ifndef RGBLED_H
#define	RGBLED_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    LED_RED = 0,
    LED_GREEN,
    LED_BLUE,
    LED_YELLOW,
    LED_WHITE,
    LED_BLACK,
    LED_TEST,
    LED_CUSTOM 
} led_color_t;

void WriteRGBLED(led_color_t color, uint32_t color1, uint32_t time1, uint32_t color2, uint32_t time2);
void SetLedRGB(uint16_t red, uint16_t green, uint16_t blue);
bool TestRGBLED(char *message, bool post);
bool TestLights(bool interactive_test);

#ifdef	__cplusplus
}
#endif

#endif	/* RGBLED_H */

