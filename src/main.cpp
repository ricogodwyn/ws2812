#include <FastLED.h>
#include <OneButton.h>
#include <MovingDot.h>
#include <RainbowBeat.h>
#include <RedWhiteBlue.h>
// #include <BluetoothSerial.h>

// BluetoothSerial SerialBT;

#define NUM_LEDS 60
#define LED_PIN 15
#define BTN_PIN 2
int brightness = 50;
int speed = 100;
CRGB leds[NUM_LEDS];
CRGB ledsNext[NUM_LEDS]; // Array for the next pattern
uint8_t patternCounter = 0;
bool isRunning = false;

// Push button connected between pin 7 and GND (no resistor required)
OneButton btn = OneButton(BTN_PIN, true, true);

void nextPattern()
{
  isRunning = false;
  patternCounter = (patternCounter + 1) % 3;
}

void runMovingDot(CRGB *ledsArray)
{
  MovingDot movingDot = MovingDot();
  movingDot.runPattern(ledsArray); // Run the pattern once
}

void runRainbowBeat(CRGB *ledsArray)
{
  RainbowBeat rainbowBeat = RainbowBeat();
  rainbowBeat.runPattern(ledsArray); // Run the pattern once
}

void runRedWhiteBlue(CRGB *ledsArray)
{
  RedWhiteBlue redWhiteBlue = RedWhiteBlue();
  redWhiteBlue.runPattern(ledsArray); // Run the pattern once
}

void crossfade(CRGB *from, CRGB *to, int duration)
{
  for (int step = 0; step <= 255; step++)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = blend(from[i], to[i], step);
    }
    FastLED.show();
    delay(duration / 256);
  }
}

String patterns;
String speedStr;
String brightnessStr;

void splitString(const String &str, char delimiter)
{
  int delimiterIndex = str.indexOf(delimiter);
  {
    int firstDelimiterIndex = str.indexOf(delimiter);
    int secondDelimiterIndex = str.indexOf(delimiter, firstDelimiterIndex + 1);

    if (firstDelimiterIndex != -1 && secondDelimiterIndex != -1)
    {
      patterns = str.substring(0, firstDelimiterIndex);
      speedStr = str.substring(firstDelimiterIndex + 1, secondDelimiterIndex);
      brightnessStr = str.substring(secondDelimiterIndex + 1);
      speed = speedStr.toInt();
      brightness = brightnessStr.toInt();
      Serial.println(speed);
      Serial.println(brightness);
    }
  }
}
String input;
void handleSerialInput(void *parameter)
{
  while (true)
  {
    if (Serial.available() > 0)
    {
      String input = Serial.readStringUntil('\n');
      Serial.println(input);
      splitString(input, ',');
      if (patterns == "0")
      {
        patternCounter = 0; // Switch to MovingDot
        isRunning = true;
      }
      else if (patterns == "1")
      {
        patternCounter = 1; // Switch to RainbowBeat
        isRunning = true;
      }
      else if (patterns == "2")
      {
        patternCounter = 2; // Switch to RedWhiteBlue
        isRunning = true;
      }
      // SerialBT.print(input);
    }
    vTaskDelay(5 / portTICK_PERIOD_MS); // Small delay to yield to other tasks
  }
}
void setup()
{
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  Serial.begin(9600);
  // SerialBT.begin("ESP32_BT");
  xTaskCreate(handleSerialInput, "SerialInputTask", 2048, NULL, 1, NULL);
  btn.attachClick(nextPattern);
}

void loop()
{
  // Run the current pattern into the leds array
  switch (patternCounter)
  {
  case 0:
    runMovingDot(ledsNext);
    break;
  case 1:
    runRainbowBeat(ledsNext);
    break;
  case 2:
    runRedWhiteBlue(ledsNext);
    break;
  }

  // Crossfade from the current pattern to the next pattern
  crossfade(leds, ledsNext, 1000); // 1000 ms duration for crossfade

  // Copy the next pattern to the current pattern
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ledsNext[i];
  }

  FastLED.show(); // Update the LEDs
  delay(speed);   // Small delay to avoid overloading the CPU
  FastLED.setBrightness(brightness);
  // Serial.println(speed);
}