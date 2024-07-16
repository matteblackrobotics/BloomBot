#ifndef Sonar_h
#define Sonar_h

#include <MBR_MovingAverage.h>

class Sonar{
  public: 
    float inches;
    float voltage;
    float minInches = 10;
    float maxInches = 10;

    Sonar(int pin, float scalingFactor, float mVCC, int movingAverageSize) : avgVoltage(movingAverageSize){
      this->_pin = pin;
      this->_scalingFactor = scalingFactor; // 512 for EZ SONAR
      this->_mVCC = mVCC; 
      this->_movingAverageSize = movingAverageSize;
      pinMode(pin, INPUT);
    }

    // this uses moving average, may have issues in loops
    float getVoltage(){
      voltage = analogRead(_pin) * (_mVCC/1023.0);
      voltage = avgVoltage.movingAverage(voltage);
      return voltage;
    }

    // uses moving average, may have issues in loops
    float getRange(){
      inches = getVoltage() / (_mVCC/_scalingFactor);
      return inches;
    }

    // uses batch averages
    float getRange2(){
      long voltageBucket = 0;
      int samples = 20;
      for(int i=0; i<samples; i++){
        voltage = analogRead(_pin) * (_mVCC/1023.0);
        voltageBucket = voltageBucket + voltage;
      }
      voltage = voltageBucket/samples;
      inches = voltage / (_mVCC/_scalingFactor);
      return inches;
    }

    float getRangeRaw(){
      voltage = analogRead(_pin) * (_mVCC/1023.0);
      inches = voltage / (_mVCC/_scalingFactor);
      return inches;
    }


    float trackMax(){
      inches = getRange();
      if(inches > maxInches){
        maxInches = inches;
      }
      return maxInches;
    }

    float trackMin(){
      inches = getRange();
      if(inches < minInches){
        minInches = inches;
      }
      return minInches;
    }

    float resetMin(){
      minInches = 10;
      minInches = trackMin();
      return minInches;
    }

    float resetMax(){
      maxInches = 10;
      maxInches = trackMax();
      return maxInches;
    }

  private:
    int _pin;
    float _scalingFactor; // 5v~9.8mV/in, 3.3v~6.4mV/in
    float _mVCC; //[mV] input
    int _movingAverageSize;
    MBR_MovingAverage avgVoltage;
};

#endif
