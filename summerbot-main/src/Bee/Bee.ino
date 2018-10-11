#include "Bee.h"
#include "pinout.h"
#include <Servo.h>

Bee *bee;
Servo *left;
Servo *right;

void setup() {

	Serial.begin(9600);
	delay(1); //to be sure the serial is set
	

	//creating servos
	left = new Servo;
	right = new Servo;
	right->write(180 - (BEE_OFFSET + RETRACTED));
	left->attach(SERVO6);
	right->attach(SERVO5);

	//creating the Bee object
	bee = new Bee(left, right, false);
	Serial.println("init");

	//just to let enought time for the serial to println
	delay(1000);

}

void loop() {

	//deploy and restract the arm quickly
	bee->deploy();
	delay(1000);
	bee->retract();
	delay(1000);

}

