#ifndef CONFIG_H
#define CONFIG_H

#include <FastLED.h>
#include <FastLED.h>
#include <OneButton.h>
#define NUM_LEDS 60
extern CRGB leds[NUM_LEDS];
extern CRGB ledsNext[NUM_LEDS];
extern OneButton btn;

#endif // CONFIG_H
