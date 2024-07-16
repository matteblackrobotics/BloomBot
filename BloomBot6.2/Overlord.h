#ifndef Overlord_h
#define Overlord_h

#include "Sonar.h"
#include "LED.h"
#include "ServoMotor.h"
#include "Utilities.h"
#include <MBR_Functions.h>
#include <MBR_Timer.h>
#include <MBR_Wave.h>

Sonar sonar1(7, 512, 5000.0, 10);

// Global variables
float range; // range of sensor
float rangeMax; // max distance from sensor
float rangeMin = 6; // min distance from sensor
float rangeSpan;    // span of detection

// setups
int setupsTotal = 5;
int setupNumber = 0;

// states
//enum States{SLEEP, AWAKE, ALERT, DANGER};
int state = 1;
int stateLast;
int states[] = {1,2,3,4,5};

MBR_Timer sleepTimer(8000);    // got to sleep after microseconds


// range triggers
const int rangeTriggersCount = 2;
// float rangeTriggerDistribution[3] = {0.80, 0.5, 0.1};  // %distribution in the range 
float rangeTriggerDistribution[rangeTriggersCount] = {.5, .25};  // %distribution in the range from furthers to closest
//float rangeTriggers[3] = {0,0,0}; 
float rangeTriggers[] = {0,0}; 

void setupLEDOverlord(){
    setupNumber = 1;
    Serial.println();
    Serial.println(F("setupLEDOverlord"));
    setupLED();
    LED1.setPixelColor(20, 10, 10, 10);
    LED1.show();
    
    // setupIndicator
    LED0.setPixelColor(0, 10, 0, 10); LED0.show(); delay(100);
    LED0.setPixelColor(setupsTotal+1, 10, 0, 10); LED0.show(); delay(100);
    LED0.setPixelColor(setupNumber, 0, 10, 10); LED0.show(); delay(100);
}


// set range triggers based off range trigger distibution
void setAutoRangeTriggers(float rangeMax){
    Serial.println();
    Serial.println(F("autoRangeTriggers [inches]: "));
    LED1.clear();
    setupNumber = 4;
    LED0.setPixelColor(setupNumber, 0, 10, 10); LED0.show();
    rangeSpan = rangeMax - rangeMin;
    LED1.setPixelColor(ledTotals[1] - rangeMax,0,0,20); LED1.show();   // set rangeMin
    for(int i=0; i<rangeTriggersCount; i++){
        rangeTriggers[i] = rangeTriggerDistribution[i] * rangeSpan + rangeMin;
        Serial.println(rangeTriggers[i]);
        int indicatorLED = round(ledTotals[1] - rangeTriggers[i]);
        LED1.setPixelColor(indicatorLED, 0, 20, 0); LED1.show();
        delay(200);
    }
}

// find the maximum range of the lamp system
void setupSonar(){
    Serial.println();
    Serial.println(F("setupSonar:"));
    float samples = 30.0;
    int rangeMaxBucket = 0;
    int indicatorLED;
    int endLED = ledTotals[1] - 30;
    float range;
    setupNumber = 3;
    LED1.clear();
    LED0.setPixelColor(setupNumber, 0, 10, 10); LED0.show();
    LED1.setPixelColor(endLED,0,50,0); LED1.show(); // end

    // average 20 range samples
    for(int i=0; i<samples; i++){
        range = sonar1.getRange2();
        range = constrain(range, 0, 1000);
        rangeMaxBucket = rangeMaxBucket + range;
        indicatorLED = ledTotals[1] - i;
        LED1.setPixelColor(indicatorLED, 0, 0, 20);
        LED1.show();
        Serial.print(String(indicatorLED)+" ");
        Serial.println(range);
        delay(15);
    }
    rangeMax = rangeMaxBucket/samples;
    rangeSpan = rangeMax - rangeMin;
    LED1.setPixelColor(ledTotals[1] - round(rangeMax), 40, 0, 0); LED1.show(); // set rangeMax to LED indicator
    Serial.println("rangeMin [inches] = "+String(rangeMin));
    Serial.println("rangeMax [inches] = "+String(rangeMax));
    Serial.println("rangeSpan [inches] = "+String(rangeSpan));
    delay(500);
}


void setStaticRangeTriggers(float rangeMax){
    Serial.println();
    Serial.println(F("staticRangeTriggers [inches]: "));
    LED1.clear();
    setupNumber = 4;
    rangeTriggers[0] = rangeMax-48;
    rangeTriggers[1] = 24;
    rangeTriggers[2] = 12;
    for(int i= 0; i<rangeTriggersCount; i++){
        Serial.print(" "+String(rangeTriggers[i]));
    }
}


void readSonar(){
    while(1){
        range = sonar1.getRange();
        Serial.println(range);
    }
}


int checkState(){
    int range = sonar1.getRange2();
    if(range > rangeTriggers[0]){state = 1;}
    else(state = 2);
    return state;
}


// reads input, min, max, and maps core to range
void rangeSectionsLED0(int sections, float input, float inputMin, float inputMax){
    int sectionLength = ledTotals[0]/sections;
    int startLED = 0;
    int rangeLED;
    float range;
    int startColors[] = {10,0,10};
    int rangeColors[] = {0,0,10};
   
    LED0.clear();
    LED0.setPixelColor(startLED, startColors[0],startColors[1],startColors[2]);
    LED0.setPixelColor(startLED + sectionLength*1, startColors[0],startColors[1],startColors[2]);
    LED0.setPixelColor(startLED + sectionLength*2, startColors[0],startColors[1],startColors[2]);

    //range = sonar1.getRange();
    rangeLED = mapFloat(input, inputMin, inputMax, sectionLength, 0);
    for(int i=1; i<rangeLED; i++){
        LED0.setPixelColor(i, rangeColors[0], rangeColors[1], rangeColors[2]);
        LED0.setPixelColor(i + sectionLength*1, rangeColors[0], rangeColors[1], rangeColors[2]);
        LED0.setPixelColor(i + sectionLength*2, rangeColors[0], rangeColors[1], rangeColors[2]);
    }
    LED0.show();
}


void mapRangeLED1(){
    Serial.println(F("mapRange()"));
    //Serial.println(" rangeMax = "+String(rangeMax)); delay(1000);
    setupNumber = 5;
    LED0.setPixelColor(setupNumber, 0, 10, 10); LED0.show();
    LED1.clear();
    int rangeLED = 0;
    int flashDelay = 30;
    int b = 70;
    int degTarget = 50;
    while(state == 1){
        checkState();    
        range = sonar1.getRange();
        rangeLED = ledTotals[1] - ((range-rangeMin) * ((ledTotals[1]/rangeSpan)));
        rangeLED = round(rangeLED);
        rangeLED = constrain(rangeLED, 0, ledTotals[1]);

        if(deg != degTarget){
            if(deg > degTarget){deg--;}
            else if(deg < degTarget){deg++;}
            writeAllServos(deg); delay(40);
        }

        // flash minLED
        if(rangeLED == 0){
            LED1.setPixelColor(1,0,0,b); LED1.show(); delay(flashDelay); 
            LED1.setPixelColor(1,0,0,0); LED1.show(); delay(flashDelay);
        }
        // flash maxLED
        else if(rangeLED == ledTotals[1]){
            LED1.setPixelColor(ledTotals[1]-1,0,0,b); LED1.show(); delay(flashDelay); 
            LED1.setPixelColor(ledTotals[1]-1,0,0,0); LED1.show(); delay(flashDelay);
        }
        // mapLED
        else if(rangeLED > 0 && rangeLED < ledTotals[1]){
            LED1.setPixelColor(rangeLED, 0, 0, b); LED1.show(); delay(flashDelay); LED1.clear();
        }
        Serial.println("  range = "+String(range)+"  rangeLED = "+String(rangeLED));
    }
}


void mapRangeArms(){
    float range; 
    float deg;
    float degMin = 0.0;
    float degMax = 40.0;
    range = sonar1.getRange();
    deg = mapFloat(range, rangeMin, rangeMax, degMax, degMin);
    writeAllServos(deg); delay(200);
}


// --------------------------------- pulse() ------------------------------- //
void pulse(){
    const int brightness = 2; // brightness value used to build LED wave
    int ledIndex = 0;   // index of lead led on the arm
    const int waveSpan = 5;   // size of the led wave

    float range;

    int delay1;         // current delay
    const int delayMin = 0;   // minimum loop delay controlled by range
    const int delayMax = 90; // maximum loop delay controlled by rante

    float deg1 = 0; // arm group 1
    float deg2 = 0; 
    float degTarget1;   // arm group 2
    float degTarget2 = 0;
    float degMin = 0.0;
    float degMax = 75.0;

    bool activeArms = false;
    const long duration1 = 2000;
    long startTime;
    long endTime;
    bool startTimeFlag;
    bool openOtherArmsFlag;
    float degOtherArms = 0;
    int rangeColor = 0;

    String pulseState = "far";

    int count;  // till opening all arms
    int countMax = 50;

    //sleepTimer.reset(); // reset sleep timer here
    LED0.clear(); LED0.show();
    LED1.clear(); LED1.show();

    // how to get servos home?
    
    while(state == 2){
        for(int ledIndex=ledTotals[1]; ledIndex>-waveSpan; ledIndex--){
            range = constrain(sonar1.getRange2(), rangeMin, rangeMax);
            state = checkState();   // from range
            // if(sleepTimer.justCheck() == true){state = 1;} // go to sleep if timer is up

            // exit function
            if(state != 2){
                Serial.println("exiting pulse()");
                LED1.clear(); 
                LED1.setPixelColor(35, 0, 50, 20);
                LED1.show();
                degTarget1 = 0;
                degTarget2 = 0;
                delay1 = 20;
                while(deg1 != degTarget1){
                    deg1 = stepFloat(deg1, degTarget1, .5, .5);
                    writeServo(1,deg1);
                    writeServo(3,deg1);
                    writeServo(5,deg1);
                    delay(delay1);
                }
                while(deg2 != degTarget2){
                    deg2 = stepFloat(deg2, degTarget2, .5, .5);
                    writeServo(2,deg2);
                    writeServo(4,deg2);
                    writeServo(6,deg2);
                    delay(delay1);
                }
                return;
            }


            // if closer to the device
            if(range <= rangeTriggers[1]){
                pulseState = " close ";

                // arms  
                if(range == rangeMin){count++;}
                else{count = 0;}

                if(count >= countMax){                        
                    degTarget2 = degTarget1;
                } 

                degTarget1 = mapFloat(range, rangeTriggers[1], rangeMin, degMin, degMax);
                degTarget1 = constrain(degTarget1, degMin, degMax);
                
                // system delay
                delay1 = mapFloat(range, rangeTriggers[0], rangeMin, delayMax, delayMin);
                delay1 = constrain(delay1, delayMin, delayMax);
            }

            else{
                count = 0;
                pulseState = " far ";
                delay1 = delayMax;
                degTarget1 = 0;
                degTarget2 = 0;
            }
            
            if(deg1 != degTarget1){
                deg1 = stepFloat(deg1, degTarget1, .5, .5);
                writeServo(1,deg1);
                writeServo(3,deg1);
                writeServo(5,deg1);
            }   

            if(deg2 != degTarget2){
                deg2 = stepFloat(deg2, degTarget2, .5, .5);
                writeServo(2,deg2);
                writeServo(4,deg2);
                writeServo(6,deg2);
            }
        
            // Arms groups 1 and 2
         
            //deg2 = stepFloat(deg2, degTarget2, 1, 1);
            // writeServoGroup(1, 3, deg1);
            //writeServo(1,deg1);
            //writeServo(3,deg1);
            //writeServo(5,deg1);
   
            //float us = mapFloat(deg, degMinLimit, degMaxLimit, usMinLimit, usMaxLimit);
            //pwmPCA9685.writeMicroseconds(1, us);
            

            // LED0, map range to core
            rangeSectionsLED0(3, range, rangeMin, rangeTriggers[0]);
            if(range == rangeMin){
                rangeSectionsLED0(3, count, 0, countMax);
            }

            // LED1, send pulse up the arms
            for(int i=0; i<waveSpan; i++){
                int x = constrain(pow(brightness, 2*i), 0, 255);   
                LED1.setPixelColor(ledIndex + waveSpan-i, rangeColor, x, x/2);  
            }
            LED1.show(); LED1.clear();

            //delay1 = mapFloat(range, rangeMin, rangeMax, delayMin, delayMax);
            // closer to next range trigger reduces loop delay
         
            delay(delay1);
            // Serial.print(" state = "+String(state)+" range = "+String(range)+" degTarget1 = "+String(degTarget1)+" deg1 = "+String(deg1)+" delay1 = "+String(delay1)+" ");
            //Serial.print(F(" state: ")); Serial.print(state); 
            //Serial.print(F(" RAM: ")); Serial.print(freeMemory());
            //Serial.print(F(" range: ")); Serial.print(range);
            //Serial.print(pulseState);
            //Serial.print(F(" count: ")); Serial.print(count);
            //Serial.print(F(" degTarget1: ")); Serial.print(degTarget1);
            //Serial.print(F(" deg1: ")); Serial.print(deg1);
            //Serial.print(F(" delay1: ")); Serial.print(delay1);
            //Serial.println();
        }
        ledIndex = ledTotals[1];
    }
    //allLEDS1Off();
}

// --------------------------------- breathe() ------------------------------- //
void breathe(){
    // build sine wave
    // map sine wave to deg
    // map sine wave to brightness
    float brightness;
    float brightnessMin = 5;
    float brightnessMax = 200;

    float deg;
    float degMin = 0;
    float degMax = 15;

    int waveSize = 500;
    float wave; 
    Wave breathWave{waveSize, 1};
    MBR_Timer timer2(1000);

    int delay1 = 1500;
    int delay2 = 0;

    //long timeTillSleep = 10000;
    //long sleepTimer = millis() + timeTillSleep;

    // keep in loop 
    // if timer is up, exicute, independent of baud rate
    while(state == 2){
        state = checkState();
        if(state != 2){return;}
        delay(delay1);
        for(int i=0; i<waveSize; i++){
            wave = breathWave.outputWave(i, waveSize); // i dont fully understand this yet
            deg = mapFloat(wave, 0.0, 1.0, degMin, degMax);
            writeAllServos(deg);

            brightness = mapFloat(wave, 0.0, 1.0, brightnessMin, brightnessMax);
            for(int i=0; i<ledTotals[1]; i++){
                LED1.setPixelColor(i, 0, brightness/2, brightness);
                LED0.setPixelColor(i, 0, brightness/2, brightness);
            }
            LED1.show();
            LED0.show();
            //range = sonar1.getRange2();
            //Serial.println("state = "+String(state)+" range = "+String(range)+" wave = "+String(wave)+" deg = "+String(deg)+" brightness = "+ String(brightness));
            Serial.print(F(" state: ")); Serial.print(state); 
            Serial.print(F(" RAM: ")); Serial.print(freeMemory());
            Serial.print(F(" range: ")); Serial.print(range);
            Serial.print(F(" wave: ")); Serial.print(range);
            Serial.print(F(" deg: ")); Serial.print(deg);
            Serial.print(F(" bri: ")); Serial.print(brightness);
            Serial.println();
            delay(delay2);
        }
    }
}


// --------------------------------- sleep() ------------------------------- //
void sleep(){
    int waveSize = 100;
    int lightArray[waveSize];
    Wave twinkleWave(waveSize, 1);

    int ledIndex = 0;
    int ledMax = 36;
    float ledBrightness[ledMax];
    int ledIndexes[ledMax];  // where is each LED in the lightArray
    int lightArrayIndex = 0;

    int randomOn = 0; // if the 
    int randomOnMax = 1000; // how likely to turn on

    float r, g, b;

    float degTarget = 0;
    
    int delay1 = 30;
    LED0.clear(); LED0.show();
    LED1.clear(); LED1.show();

    // set ledIndexes all to 0 to start so all leds start off
    for(int i=0; i<ledMax; i++){
        ledIndexes[i] = 0;
    }

    // build and store lightArray, setup
    Serial.println();
    // Serial.println("lightArray:");
    for(int i=0; i<waveSize; i++){
        lightArray[i] = twinkleWave.outputWave(i, waveSize) * brightnessMax;
        // Serial.println(lightArray[i]);
    }

    // loop
    while(state == 1){
        state = checkState();
        range = sonar1.getRange2();
        // if(range < 10){state = 2;}

        // all motor to 0
        /*
        for(int i=0; i<servoTotal; i++){
            degs[i] = stepFloat(degs[i], degTarget, 1, 1);
            writeServo(i, degs[i]);
        }
        */

        for(ledIndex = 0; ledIndex < ledMax; ledIndex++){
            if(ledIndexes[ledIndex] == 0){   // if LED is at 0 in the lightArray, roll dice for turn on
                randomOn = random(0, randomOnMax);  // random number generator between values
                if(randomOn >= randomOnMax-1){
                    ledIndexes[ledIndex]++; // increase the index value in indexArray at the location of the current led
                }
            }
            else{
                ledIndexes[ledIndex]++;
            }
            if(ledIndexes[ledIndex] >= waveSize){   // send back to 0 once the array is complete
                ledIndexes[ledIndex] = 0;
            }
            lightArrayIndex = ledIndexes[ledIndex];
            ledBrightness[ledIndex] = lightArray[lightArrayIndex];
            r = 0;
            g = ledBrightness[ledIndex]/2;
            b = ledBrightness[ledIndex];
            LED1.setPixelColor(ledIndex, r, g, b);
            LED0.setPixelColor(ledIndex, r, g, b);
        }
        LED1.show();
        LED0.show();
        //Serial.println(" state = "+String(state)+" range = "+String(range));
        Serial.print(F(" state: ")); Serial.print(state); 
        Serial.print(F(" RAM: ")); Serial.print(freeMemory());
        Serial.print(F(" range: ")); Serial.print(range);
        Serial.println();
        delay(delay1);
    }
    //allLEDS1Off();  // dim all LEDs
    return;
}


// --------------------------------- bloom() ------------------------------- //
void bloom(){
    Serial.println("bloom()");
    
    // if timer is up
    // open arms up and brighten up
    float degTarget1 = degMax;
    float degTarget2 = 30;
    float degStep1 = degTarget1 / degMax;
    float degStep2 = degTarget2 / degMax;
    float deg1 = 0;   // arms group 1 // degs[0] // why is this 31
    float deg2 = 0;   // arms group 2 // degs[1]

    // timer
    long time1Start = millis();
    long timeDuration =  2000;
    long timeCurrent;

    int delay1 = 20;
    // if true
    while(1){
        range = sonar1.getRange2();
        deg1 = stepFloat(deg1, degTarget1, degStep1, 1);
        deg2 = stepFloat(deg2, degTarget2, degStep2, 1);

        //writeServo(1, deg1);
        writeServoGroup(1, 3, deg1);
        writeServoGroup(2, 3, deg2);
        Serial.print(" state = "); Serial.print(state); Serial.print(" range = "); Serial.print(range);
        Serial.print(" deg1 = "+String(deg1)+" deg2 = "+String(deg2));
        Serial.println();
        delay(delay1);
    }
}


// ----------------------------------- bloomAll() ---------------------------------- //
void bloomAll()
{
    Serial.println("bloomAll()");
    
    // if timer is up
    // open arms up and brighten up
    float degTarget1 = degMax;
    float degStep1 = .5;
    float deg1 = 0;   // arms group 1 // degs[0] // why is this 31

    // timer
    long time1Start = millis();
    long timeDuration =  10000;  // run for 5 sec then turn off
    long timeEnd = time1Start + timeDuration;
    long timeRemaining = timeDuration;

    // LED0
    int ledIndex = 0;
    LED0.clear();
    for(int i=0; i<ledTotals[0]; i++){
        LED0.setPixelColor(i, 0, 0, 20);
    }

    // LED1
    int x1 = 20;
    int x2 = 40;
    int x3 = 100;
    int x4 = 200;
    int x;
    float r = 0;
    float g = 0;
    float b = 0;

    int delay1 = 20;
    // if true
    while(timeRemaining > 0){
        timeRemaining = timeEnd - millis();

        // LED0
        ledIndex = mapFloat(timeRemaining, timeDuration, 0, (ledTotals[0]/2)-1, 0);
        LED0.setPixelColor(ledIndex, 0, 0, 0); 
        LED0.setPixelColor(ledIndex + (ledTotals[0]/2), 0,0,0);
        LED0.show();

        // LED1
        for(int i=0; i<ledTotals[1]; i++){
            r = stepFloat(r, x4, 0.05, .05);
            g = stepFloat(g, x4, 0.0, .05);
            b = stepFloat(b, x3, 0.05, .05);
            LED1.setPixelColor(i, r*0,g,b);
        }
        LED1.show();
        
        // Arms
        range = sonar1.getRange2();
        deg1 = stepFloat(deg1, degTarget1, degStep1, degStep1);
        writeAllServos(deg1);
        Serial.print(" state = "); Serial.print(state); Serial.print(" range = "); Serial.print(range);
        Serial.print(" deg1 = "+String(deg1));
        Serial.print(" timeRemaining = "+String(timeRemaining));
        Serial.println();
        delay(delay1);
    }
    degTarget1 = 0;

    for(int i=0; i<ledTotals[0]; i++){
        LED0.setPixelColor(i, 0, 20, 20);
    }
    LED0.show();

    while(deg1 != degTarget1){
        deg1 = stepFloat(deg1, degTarget1, degStep1, degStep1);
        writeAllServos(deg1);
        Serial.print(" deg1 = "+String(deg1));
        Serial.println();
        delay(delay1);
    }

    for(int i=0; i<ledTotals[0]; i++){
        LED0.setPixelColor(i, 0, 10, 10);
    }
    LED0.show();
    LED1.clear();
    LED1.show();
    Serial.print("end BloomAll()");
}


void servosHome(){
  Serial.println("-------- servosHome --------");
  int degTarget = degMin;
  while(deg != degTarget){
    deg = stepFloat(deg, degTarget, 1, 1);
    writeAllServos(deg);
    Serial.println("deg: "+String(deg));
    delay(100);
  }
}


// --------------------------------- danger() ------------------------------- //
void danger(){
    int r = 20;
    int deg1 = 10;
    int deg2 = 14;
    int delay1 = 10;
    MBR_Timer timer1(3000);
    long timeRemaining;
    
    while(state == 4){
        state = checkState();
        int range = sonar1.getRange();
        if(timer1.check() == false){
            for(int i=0; i<ledTotals[1]; i++){
                LED1.setPixelColor(i, r, 0, 0); LED1.show();
            }
            //writeAllServos(deg1); delay(delay1);

            for(int i=0; i<ledTotals[1]; i++){
                LED1.setPixelColor(i, r, r, r); LED1.show();
            }
            //writeAllServos(deg2); delay(delay1);
            timeRemaining = timer1.remaining();
            Serial.println(
                " timeRemaining = "+String(timeRemaining)+
                " systemSate = "+String(state));
        }
        else{
            state = 5;
            return;
        }
    }
}

#endif