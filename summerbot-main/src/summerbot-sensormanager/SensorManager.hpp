#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H 1
#define SHARP ((unsigned char)1)

#define DEFAULTPRECISIONLEVEL 5

#include <Arduino.h>

class SensorManager{
	public:
 
   /**
    * The default constructor
    */
   SensorManager();
   
   /**
    * The advanced constructor
    */
   SensorManager(unsigned char inputPinMin, unsigned char inputPinMax);
   
	 /**
	  * Create a normalised var type SensorType
	  */
	 typedef unsigned char SensorType;
	
	 /**
	  * Register a new sensor
	  */
	 void registerNewSensor(unsigned char id, SensorType type);
	 
	 /**
	  * Return the output (distance, converted in mm) of the specified sensor with the default precision level
	  */
	 double readSensorData(unsigned char id);
   
   /**
    * Return the average raw value of the last <measureCount> output of the specified sensor
    */
   int readRawSensorData(unsigned char id, unsigned char measureCount);
   
   /**
    * Detect if something is in front of the specified sensor placed at a distance less than <thresholdDistance>
    */
   bool detectObject(unsigned char id, double thresholdDistance); // distance in mm
    
	private:	//private stuff; don't try to modifie this! x)
   unsigned char *__IdToType;
};

#endif
