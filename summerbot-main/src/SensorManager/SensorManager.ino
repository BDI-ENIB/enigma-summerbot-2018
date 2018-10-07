#include "SensorManager.h"
#include <Arduino.h>

/* defines for every sensors on the summerbot board */

// IR
#define IRS1 A5
#define IRS2 A4
#define IRS3 A3
#define IRS4 A2
#define IRS5 A1
#define IRS6 A0
#define IRS7 A22
#define IRS8 A21

// US
#define USS1 34
#define USS2 33

/* SensorManager */
SensorManager* sensorManager;

/* Detection threshold distance for every sensor (mm) */
#define DISTANCETHRESHOLD 300

void setup (){
  sensorManager = new SensorManager();

  // registering 4 IR Sensors
  sensorManager->registerNewSensor(IRS1, SHARP);
  sensorManager->registerNewSensor(IRS2, SHARP);
  sensorManager->registerNewSensor(IRS3, SHARP);
  sensorManager->registerNewSensor(IRS4, SHARP);
  
  Serial.begin(9600);
}

void loop (){
  bool AdvDetected = sensorManager->detectObject(IRS1, DISTANCETHRESHOLD);
  
  if(AdvDetected)
    Serial.println("ADVERSAIRE DETECTE !");
  
  delay(300);
}

