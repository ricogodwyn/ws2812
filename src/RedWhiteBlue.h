
#include "config.h"

class RedWhiteBlue
{
public:
    RedWhiteBlue() {};
    void runPattern(CRGB *ledsArray);

private:
};

void RedWhiteBlue::runPattern(CRGB *ledsArray)
{

    uint16_t sinBeat = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
    uint16_t sinBeat2 = beatsin16(30, 0, NUM_LEDS - 1, 0, 21845);
    uint16_t sinBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 43690);

    ledsArray[sinBeat] = CRGB::Blue;
    ledsArray[sinBeat2] = CRGB::Red;
    ledsArray[sinBeat3] = CRGB::White;

    fadeToBlackBy(ledsArray, NUM_LEDS, 10);

    btn.tick();
    FastLED.show();
}