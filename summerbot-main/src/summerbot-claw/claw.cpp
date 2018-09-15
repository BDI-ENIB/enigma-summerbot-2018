//---Pince header

#include "claw.hpp"


//---Constructor

/**
  *	Pince(lift Servo, uchar lift_speed, clamp Servo, uchar clamp_speed) 
  */
Claw::Claw(Servo *liftServo, unsigned char lt_speed, Servo *clpServoR, Servo *clpServoL, unsigned char clp_speed){

	lift=liftServo;
	clampLeft = clpServoR;
	clampRight = clpServoL;
	liftSpeed = 255 - lt_speed;
	clampSpeed = 255 - clp_speed;
	lastClampTime = millis();
	lastLiftTime = millis();
	isPaused = false;
	moves_ = NULL;
	
}


//---Functions
/**
  * init( )
  */

void Claw::init(){
  lift->write(DOWN);
  delay(20);
  clampLeft->write(CLOSE);
  delay(20);
  clampRight->write(300-CLOSE+OFFSET);
  delay(20);
}

/**
  * moveLift(uchar pos)
  */
void Claw::moveLift(int targPos) {
	if(lift->read() == targPos) {
		ClawMove* mv = new ClawMove(MoveType::Lift,targPos);
		if(moves_)moves_->append(mv);
		else moves_ = mv;
	}
}

/**
  * moveClamp(uchar pos)
  */
void Claw::moveClamp(int targPos) {
	if(clampRight->read() == targPos) {
		ClawMove* mv = new ClawMove(MoveType::Clamp,targPos);
		if(moves_)moves_->append(mv);
		else moves_ = mv;
	}
}

/**
  * load()
  */
void Claw::load() {
	
	moveLift(DOWN);
	moveClamp(CLOSE);
	moveLift(UP);

}

/**
  * unload()
  */
void Claw::unload() {
	
	moveLift(DOWN);
	moveClamp(OPEN);
	moveLift(UP);

}

/**
  * stack()
  */
void Claw::stack() {

	moveLift(DOWN);
	moveClamp(CLOSE-1);
	moveClamp(CLOSE);
	moveLift(UP);
	
}

  
/**
  * pause
  */
void Claw::pause() {
	isPaused = true;
}

/**
  * unpause
  */
void  Claw::unpause() {
	if(isPaused) {
		isPaused = false;
	}
}

/**
  * clearMoves
  */
void Claw::clearMoves() {
	moves_->clear();
}

/**
  * movesString
  */
String Claw::movesString(){
  String r="";
  ClawMove* mv =moves_;
  while(mv){
    r+=mv->toString();
    mv=mv->getNext();
  }
  return r;
}

/**
  * setClampSpeed
  */
void Claw::setClampSpeed(unsigned char clpSpeed){
	clampSpeed=255-clpSpeed;
}

/**
  * setLiftSpeed
  */
void Claw::setLiftSpeed(unsigned char ltSpeed){
	liftSpeed=255-ltSpeed;
}

void Claw::update() {
	
	if(moves_ && !isPaused) {
		if(moves_->type_ == MoveType::Lift){
			const int currentPos = lift->read();
			const int increment = (moves_->targPos_-currentPos > 0 ? 1 : -1);
			const int newPos = currentPos + increment;
			lift->write(newPos);
			if(abs(newPos - moves_->targPos_) < 1) {
				clearCurrentMove();
			}
		}
	
		if(moves_->type_ == MoveType::Clamp){
			const int currentPos = clampRight->read();
			const int increment = (moves_->targPos_-currentPos > 0 ? 1 : -1);
			clampRight->write(currentPos + increment);
			clampLeft->write(300-currentPos - increment);
			if(abs(currentPos + increment - moves_->targPos_) < 1) {
				clearCurrentMove();
			}
		}
	}
}
	
void Claw::clearCurrentMove () {
	ClawMove* mv = moves_;
	moves_ = moves_->getNext();
	delete mv;
}

void Claw::openWide() {
	//to be coded
}

bool Claw::isBusy() {
	return moves_;
}
