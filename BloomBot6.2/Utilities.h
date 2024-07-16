#ifndef Utilities_h
#define Utilities_h

// RAM
#include <MemoryFree.h>
#include <pgmStrToRAM.h>

const int baud1 = 9600;
const int baud2 = 19200;
const long baud3 = 115200;
long baudRate = baud3;
String fileName = "Bloombot6.2.ino";
// String filePath = "Engineering/Projects/BloomBot/02 Software/BloomBot6.1";
String author = "Matt Pfeiffer - 24.06.07";


// setupUtilities
void setupUtilities()
{
  Serial.begin(baudRate); delay(200);
  Serial.println(); Serial.println();
  Serial.println(fileName);
  //Serial.println(filePath);
  Serial.println(author);
  Serial.println();
}

// printRAM
void printRAM()
{
  float totalNanoSRAM = 2000.0;
  float percentSRAMremaining = (float(freeMemory())/totalNanoSRAM) * 100.0;
  Serial.println();
  Serial.print("Free RAM = "); Serial.print(freeMemory()); Serial.print(" bytes, "); 
  Serial.print(percentSRAMremaining); Serial.print("%"); 
  Serial.println();
}


// stepFloat
float stepFloat(float current, float target, float stepSize, float threshold)
{
  float error = target - current; 
  if(abs(error) >= threshold)
  {
    if(error > 0) {current = current + stepSize;}
    if(error < 0) {current = current - stepSize;}
  }
  return current;
}


// mapFloat
float mapFloat(float x, float x1, float x2, float y1, float y2){
  // y = mx+b
  float m = (y2 - y1)/(x2 - x1);
  float b = y1 - m * x1;
  float y = m * x + b;
  return y;
}

#endif