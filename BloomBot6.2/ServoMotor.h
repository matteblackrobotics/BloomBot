#ifndef ServoMotors_h
#define ServoMotors_h



// ---------------- PCA9685 --------------- //
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwmPCA9685 = Adafruit_PWMServoDriver();
// SCL Purple - Nano Pin D5
// SDA Blue - Nano Pin D4
// Vcc - 5v
// GND - GND

int usMin = 340; // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
int usMax = 2600; // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
int servoFrequency = 50; // Analog servos run at ~50 Hz updates
const int servoTotal = 6;

float deg;  // system deg
float degTarget;  // system deg
float degMin = 0;
float degMax = 50;

// state matrix
float uss[servoTotal];  // microsecond matrix
float degs[servoTotal] = {0,0,0,0,0,0};  // degree matrix 
float degOffsets[servoTotal] = {30.0,25.0,35.0,30.0,25.0,25.0}; // offset matrix

// Servo Calibration MG996 TIANKONGRC DIGI HIGH SPEED
int degMinLimit = 0;
int degMaxLimit = 212;
int usMinLimit = 340;
int usMaxLimit = 2600;

// write single servo
// include offset and convert to us
// numbered servos on hardware [1-8], writefunction[0-7]
// store values in degs[] and uss[] arrays
void writeServo(int servoNumber, float deg){
  degs[servoNumber-1] = deg + degOffsets[servoNumber-1];
  uss[servoNumber-1] = degs[servoNumber-1] * ((usMaxLimit - usMinLimit)/(degMaxLimit - degMinLimit)) + usMinLimit;
  pwmPCA9685.writeMicroseconds(servoNumber-1, uss[servoNumber-1]);
}


/*
void stepServo(int servoNumber, float degTarget, float stepSize, float tolerance){
  int iServo = servoNumber - 1;
  degs[iServo] = stepFloat(degs[iServo], )

}
*/


// writeAllServos using function above
void writeAllServos(float deg){
   for(int i=1; i<=servoTotal; i++){
    writeServo(i, deg);
   }
}


// moves group of servos
void writeServoGroup(int startingServo, int groupSize, float deg){
  int servoA;
  int servoB;
  int servoC;
  int totalServos = 6;

  if(groupSize == 2){
    servoA = startingServo - 1;
    servoB = servoA + (totalServos/groupSize); // +3
  }
  else if(groupSize == 3){
    servoA = startingServo - 1;
    servoB = servoA + (totalServos/groupSize); // +2
    servoC = servoB + (totalServos/groupSize); // +2;
  }

  degs[servoA] = deg + degOffsets[servoA];
  degs[servoB] = deg + degOffsets[servoB];
  uss[servoA] = degs[servoA] * ((usMaxLimit - usMinLimit)/(degMaxLimit - degMinLimit)) + usMinLimit; 
  uss[servoB] = degs[servoB] * ((usMaxLimit - usMinLimit)/(degMaxLimit - degMinLimit)) + usMinLimit; 

  if(groupSize == 3){
    degs[servoC] = deg + degOffsets[servoC];
    uss[servoC] = degs[servoC] * ((usMaxLimit - usMinLimit)/(degMaxLimit - degMinLimit)) + usMinLimit; 
    };

  pwmPCA9685.writeMicroseconds(servoA, uss[servoA]);
  pwmPCA9685.writeMicroseconds(servoB, uss[servoB]);
  if(groupSize == 3){pwmPCA9685.writeMicroseconds(servoC, uss[servoC]);}
}



void setupServos(){
  pwmPCA9685.begin();
  pwmPCA9685.setOscillatorFrequency(27000000);
  pwmPCA9685.setPWMFreq(servoFrequency);  // Analog servos run at ~50 Hz updates
  int startDeg = 20;
  int openDeg = startDeg + 10;
  int closeDeg = 0;
  int deg = startDeg;
  int delay1 = 50;
  writeAllServos(startDeg);
  while(deg <= openDeg){
    writeAllServos(deg); delay(delay1);
    deg++;
  }
  while(deg >= closeDeg){
    writeAllServos(deg); delay(delay1);
    deg--;
  }
}

#endif

