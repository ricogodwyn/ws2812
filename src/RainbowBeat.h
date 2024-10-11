
#include "config.h"

class RainbowBeat
{
public:
    RainbowBeat() {};
    void runPattern(CRGB *ledsArray);

private:
};

void RainbowBeat::runPattern(CRGB *ledsArray)
{

    uint16_t beatA = beatsin16(30, 0, 255);
    uint16_t beatB = beatsin16(20, 0, 255);
    fill_rainbow(ledsArray, NUM_LEDS, (beatA + beatB) / 2, 8);

    btn.tick();
    FastLED.show();
}