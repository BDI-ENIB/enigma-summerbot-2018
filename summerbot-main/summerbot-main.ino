#include <Arduino.h>
#include <Servo.h>

#include "src/summerbot-claw/claw.hpp"
#include "src/summerbot-motionbase/MotionBase.h"
#include "src/summerbot-sensormanager/sensormanager.hpp"
#include "pinout.h"
#include "terrain.h"
#include "robot.h"
#include "ia.hpp"

#define STEPS_PER_REV 200
#define DISTANCE_THRESHOLD_MOVING_FORWARD 300 //15cm
#define DISTANCE_THRESHOLD_MOVING_BACKWARD 200 //5cm
#define MATCHLENGHT 100'000 //millisec

IntervalTimer motionTimer;
DualDRV8825* dd=new DualDRV8825(200, 32, 30, 31, 29, 26, 25, 24);// steps per rev,left dir pin, left step pin, right dir pin, right step pin, mode pin 0, mode pin 1, mode pin 2
MotionBase *mb = new MotionBase(dd,109/2.0,180/2.0); // motors, wheel radius, robot radius, x, y, a

Servo tmplift;
Servo tmpClampL;
Servo tmpClampR;
Claw *claw;

IA ia(mb, claw);
double startTime;

SensorManager* sensorManager;

void setup (){
	
	pinMode(SIDE, INPUT);
	pinMode(STARTER, INPUT_PULLUP);  
	
	//Claw
	tmplift.attach(9);
	tmpClampL.attach(10);
	tmpClampR.attach(11);
	claw = new Claw(&tmplift,220,&tmpClampL,&tmpClampR,220);
	
	//SensorManager
	sensorManager = new SensorManager();	
	sensorManager->registerNewSensor(IRS1, SHARP);
	sensorManager->registerNewSensor(IRS2, SHARP);
	sensorManager->registerNewSensor(IRS3, SHARP);
	sensorManager->registerNewSensor(IRS4, SHARP);
	
	//Timer
	motionTimer.begin(motionLoop,100);// 10kHz (100 is the period in microS)
	motionTimer.priority(1); //slightly above normal
	
	//Serial
	Serial.begin(250000);
	commands_init();
	
	//Entering Starter Loop
	delayStarter();
	
	//Moves
	//Rappel: CommandType{forward, rotate, moveTo, load, unload, stack}
	mb->pause();
	boolean isRightSide = digitalRead(SIDE); //Bouton SIDE: Si activé, robot coté droit
	#define IRS ((!isRightSide)?1:-1)*
    IA::Command coms[]={
      {ia.CommandType::forward,{400.0}},
      {ia.CommandType::forward,{-100.0}},
      {ia.CommandType::rotate,{IRS 3.14159265/2}},
      {ia.CommandType::forward,{145.0}},
      {ia.CommandType::rotate,{IRS 3.14159265/2}},
      {ia.CommandType::forward,{-730.0}},
      {ia.CommandType::rotate,{IRS -3.14159265/2}},
      {ia.CommandType::forward,{-260.0}},
      {ia.CommandType::forward,{200.0}},
      {ia.CommandType::forward,{-220.0}},
      {ia.CommandType::forward,{740.0}},
      {ia.CommandType::rotate,{IRS 3.14159265/2}},
      {ia.CommandType::forward,{280.0}},
      {ia.CommandType::rotate,{IRS 3.14159265/2}},
      {ia.CommandType::buldozer,{}},
      {ia.CommandType::forward,{382.0}},
      {ia.CommandType::recalibrate,{200.0}},
      {ia.CommandType::forward,{-100.0}},
      {ia.CommandType::rotate,{IRS 3.14159265/2}},
      {ia.CommandType::forward,{-1010.0}},
      {ia.CommandType::recalibrate,{-30.0}},
      {ia.CommandType::forward,{130.0}},
      {ia.CommandType::rotate,{IRS 3.14159265/2}},
      {ia.CommandType::forward,{-700.0}},
      {ia.CommandType::recalibrate,{-30.0}},
      {ia.CommandType::forward,{1680.0}},
      {ia.CommandType::rotate,{-3.14159265/2}},
      {ia.CommandType::forward,{IRS -130.0}},
      {ia.CommandType::forward,{IRS 300.0}}
    };
		ia.addCommands(coms, 24);
   startTime = millis();
   Serial.println("STARTER RETIRÉ");
}

void delayStarter(){
	double tmp = 0;
	boolean hasStarterBeenInserted = false;
	while (true){
		tmp = (double)(tmp*99.0+digitalRead(STARTER))/100.0;
		if (!hasStarterBeenInserted && tmp >= 0.99){
			hasStarterBeenInserted=true;		
		}
		if (hasStarterBeenInserted && tmp <= 0.01){
			return;
		}
		delay(1);
	}
}

void loop (){
	if(millis()>=MATCHLENGHT+startTime||
			sensorManager->detectObject(IRS1, DISTANCE_THRESHOLD_MOVING_BACKWARD) ||
			sensorManager->detectObject(IRS2, DISTANCE_THRESHOLD_MOVING_BACKWARD) ||
			sensorManager->detectObject(IRS3, DISTANCE_THRESHOLD_MOVING_FORWARD) ||
			sensorManager->detectObject(IRS4, DISTANCE_THRESHOLD_MOVING_FORWARD)){
		mb->pause();
		return;
	}
	commands_update();
	mb->resume();
	ia.update();
  
}
void motionLoop(){
	mb->update();
}
