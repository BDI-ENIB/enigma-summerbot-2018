#include "claw.hpp"
#include <Servo.h>

IntervalTimer motionTimer;
Servo tmplift;
Servo tmpClampL;
Servo tmpClampR;
Claw* claw;
bool init;

void setup() {

	//Timer
	motionTimer.begin(motionLoop,1000000);// 10kHz (100 is the period in microS)
	motionTimer.priority(1); //slightly above normal
  
	Serial.begin(9600);
	delay(5000);
		
	tmplift.attach(4);
	tmpClampL.attach(3);
	tmpClampR.attach(2);
	claw = new Claw(&tmplift,200,&tmpClampL,&tmpClampR,200);
	delay(100);
	claw->init();
	Serial.println("Claw initilized");
	Serial.println("waiting 3s for motion to complete");
	delay(3000);
	init = true;

}

void loop() {
	
	if(init) {
		
		//claw->moveLift(DOWN);	
		Serial.println(claw->movesString());
		claw->moveClamp(OPEN);
		Serial.println(claw->movesString());
		//claw->moveLift(DOWN);
		//claw->moveClamp(CLOSE);
		init = false;
		
	}
	
	//claw->moveLift(DOWN);
	//claw->moveLift(UP)
	
	Serial.print("isBusy :");
	Serial.println(claw->isBusy());
}

void motionLoop() {
	claw->update();
}
