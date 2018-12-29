#include <string.h>
#include <stdlib.h>
//https://github.com/Martinsos/arduino-lib-hc-sr04
#include <HCSR04.h>
//https://github.com/prampec/arduino-softtimer/blob/wiki/QuickStartGuide.md
//https://github.com/prampec/arduino-pcimanager
#include <SoftTimer.h>
//https://www.arduino.cc/en/Tutorial/MasterWriter
#include <Wire.h>

#define MEAN_NO_OF_STEPS 20
#define PRESENCE_DETECTION_THRESHOLD 10

// function declarations
UltraSonicDistanceSensor distanceSensor(13, 12);  // Initialize sensor that uses digital pins 13 and 12.
void callBack1(Task* me);


double mean_distance = 0;
int mean_step = 1;
Task t1(2000, callBack1);

void callBack1(Task* me) {
  char mean_str[40];
  char distance_str[40];
  // Every 100 miliseconds, do a measurement using the sensor and print the distance in centimeters
  double distance = distanceSensor.measureDistanceCm();
  mean_distance = (mean_distance*(mean_step-1) + distance)/(mean_step);
  if(++mean_step > MEAN_NO_OF_STEPS) { mean_step = MEAN_NO_OF_STEPS; };

  snprintf(mean_str, 40, "%f", mean_distance);
  snprintf(distance_str, 40, "%f", distance);
  Serial.println(distance);
  Serial.println(mean_distance);
  if((mean_distance - distance) > PRESENCE_DETECTION_THRESHOLD) { Serial.println("Eveniment detectat!");}

}

void requestEvent() {
  Wire.write("hello "); // respond with message of 6 bytes
  // as expected by master
  Serial.println("Call wire received");
}

void setup () {
  Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
  SoftTimer.add(&t1);
  // wire protocol
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  
}
