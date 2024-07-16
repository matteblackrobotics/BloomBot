// LED

#ifndef LED_h
#define LED_h

#include <Adafruit_NeoPixel.h>

// Key:
// LED0 = LED Ring
// LED1 = LED Strips

int brightnessTargets[] = {0,0};  // target brightness for LED0, LED1, LED2 <<<<< I think this is old??

const int ledPins[] = {9,8};
const int ledTotals[] = {24,36};
int ledIndexes[] = {0,0};

// RGB current and target arrays for {LED0, LED1, LED2} for full base color
int Rs[] = {0,0};
int Gs[] = {0,0};
int Bs[] = {0,0};

int RTargets[] = {0,0};
int GTargets[] = {0,0};
int BTargets[] = {0,0};

// LED stepping
const int brightnessStep = 5;     // steps between brightness values
const int brightnessThreshold = 0; // error to no trigger step
const int brightnessMax = 255;

// Trigger brightnesses for each trigger state T0 to T4 [0:100]??
int RTriggersLED0[] = {10, 80, 80, 40, 80};
int GTriggersLED0[] = {10, 0, 0, 10, 0};
int BTriggersLED0[] = {80, 30, 30, 80, 80};

int RTriggersLED1[] = {10, 100, 100, 40, 80};
int GTriggersLED1[] = {10, 0, 0, 10, 0};
int BTriggersLED1[] = {80, 30, 30, 80, 80};

// Initialize LEDs
Adafruit_NeoPixel LED0(ledTotals[0], ledPins[0], NEO_GRB + NEO_KHZ800);  //  LED Ring
Adafruit_NeoPixel LED1(ledTotals[1], ledPins[1], NEO_RGB + NEO_KHZ800);  //  LED Strips

/*

// ---------------------------------------- LED0: LED Ring ----------------------------------- //
// --------------------- setLED1Base ---------------- //
// Step RGBs towards targets
void stepLED0Base(int RTarget, int GTarget, int BTarget)
{
  Rs[0] = stepInt(Rs[0], RTarget, brightnessStep, brightnessThreshold);
  Gs[0] = stepInt(Gs[0], GTarget, brightnessStep, brightnessThreshold);
  Bs[0] = stepInt(Bs[0], BTarget, brightnessStep, brightnessThreshold);
}

// Set pixels
void setLED0Base(int R, int G, int B)
{
  for(int i=0; i<ledTotals[0]; i++)
  {
    LED0.setPixelColor(i, R, G, B);
  }
}

// Show LED0
void showLED0()
{
  LED0.show();
}



// ---------------------------------------- LED1: LED Arms ----------------------------------- //
// --------------------- setLED1Base ---------------- //
// input targets
// calculate next steps
// set led pixels
void stepLED1Base(int RTarget, int GTarget, int BTarget)
{
  // RGB next steps for LED0 base color
  Rs[1] = stepInt(Rs[1], RTarget, brightnessStep, brightnessThreshold);
  Gs[1] = stepInt(Gs[1], GTarget, brightnessStep, brightnessThreshold);
  Bs[1] = stepInt(Bs[1], BTarget, brightnessStep, brightnessThreshold);
}

// set pixels
void setLED1Base(int R, int G, int B)
{
  for(int i=0; i<ledTotals[1]; i++)
  {
    LED1.setPixelColor(i, R, G, B);
  }
}

void showLED1()
{
  LED1.show();
}


// -------------------- setBrightenssTarget ------------------ //
// (rangeNorm)
int setBrightnessTarget(float rangeNorm)
{
  int brightnessTarget = linearMap(rangeNorm, 0.0, 1.0, 0, brightnessMax);
  brightnessTarget = constrain(brightnessTarget, 0, 255);
  return brightnessTarget;
}

// -------------------- rangeLED --------------- //
// maps range to LED
void rangeLED(float range)
{
  int R = 200;
  int G = 0;
  int B = 0;
  range = constrain(range, rangeMin, rangeMax);
  int rangeLED = map(range, rangeMin, rangeMax, 0, ledTotals[1]);
  //Serial.println(rangeLED);
  LED1.setPixelColor(rangeLED, R, G, B);
  LED1.show();
}
*/
// --------------- setupLED --------------- //
void setupLED()
{
 LED0.begin();
 LED1.begin();
 LED0.setBrightness(100);  // set default brightness
 LED1.setBrightness(100);
 LED0.show();  // set all LEDs to off
 LED1.show();
}

void allLED1s(int r, int g, int b){
  for(int i = 0; i<ledTotals[1]; i++){
    LED1.setPixelColor(i, r, g, b);
  }
  LED1.show();
}




#endif