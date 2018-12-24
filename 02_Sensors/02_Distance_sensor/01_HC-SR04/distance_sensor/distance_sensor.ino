#include <HCSR04.h>
#include <string.h>
#include <stdlib.h>

#define MEAN_NO_OF_STEPS 20
#define PRESENCE_DETECTION_THRESHOLD 10

UltraSonicDistanceSensor distanceSensor(13, 12);  // Initialize sensor that uses digital pins 13 and 12.

double mean_distance = 0;
int mean_step = 1;

void setup () {
    Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
}

void loop () {
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
    delay(100);
}
