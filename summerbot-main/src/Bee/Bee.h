#ifndef BEE_H
#define BEE_H 1

//Standard headers
#include <Arduino.h>
#include <Servo.h>

#define BEE_OFFSET 11 //on the right servo
#define DEPLOYED 77
#define RETRACTED 155

class Bee {

public :

	Bee(Servo *left, Servo *right, bool side); 
	//side is for extending one arm or the oder depending on the table side

	void deploy();
	void retract();

	bool isBusy();

private :
	
	bool side_;
	Servo *left_;
	Servo *right_;
	
};

#endif

