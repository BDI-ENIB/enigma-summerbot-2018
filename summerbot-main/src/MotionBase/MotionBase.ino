//#include <Arduino.h>

#include "DualDRV8825.h"
#include "MotionBase.h"
IntervalTimer motionTimer;
DualDRV8825* dd=new DualDRV8825(200, 32, 30, 31, 29, 26, 25, 24);// steps per rev,left dir pin, left step pin, right dir pin, right step pin, mode pin 0, mode pin 1, mode pin 2
MotionBase mb(dd,33,63); // motors, wheel radius, robot radius, x, y, a
int r = 0;

void setup (){

	//Timer
	motionTimer.begin(motionLoop,100);// 10kHz (100 is the period in microS)
	motionTimer.priority(1); //slightly above normal
	
	//Serial
	Serial.begin(250000);
	
	//Test move
	delay(5000);
	Serial.println("Start");
	mb.moveToRPM(100,100,3.141592/2,400); //move diagonaly to x=100 y=100 then turn to a=90deg
	mb.moveTo(200,200,3.141592/2);
	mb.moveTo(0,0,0);
	mb.moveTo(1000,0,3.141592);
	mb.moveTo(0,0,0);
	mb.moveToRPM(1000,0,3.141592,48);
	Serial.println(mb.movesString()); //should be 3 moves
	mb.computeLastMoveCoords();
	Serial.print(mb.getLastMoveX());
	Serial.print(" ");
	Serial.print(mb.getLastMoveY());
	Serial.print(" ");
	Serial.println(mb.getLastMoveA());
	delay(1000);
}

void loop (){
	//Debug display
	if(r>1000){
		Serial.println(mb.movesString());
		Serial.println(dd->getRemainingSteps());
		Serial.print(mb.getX());
		Serial.print(" ");
		Serial.print(mb.getY());
		Serial.print(" ");
		Serial.println(mb.getA());
		r=0;
	}
	r++;
}

void motionLoop(){
	mb.update();
}

