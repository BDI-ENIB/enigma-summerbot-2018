//---Pince header

#include "Claw.h"


//---Constructor

/**
  *Pince(lift Servo, uchar lift_speed, clamp Servo, uchar clamp_speed) 
  */
Claw::Claw(Servo *liftServo, Servo *clpServoR, Servo *clpServoL, unsigned char lt_speed, unsigned char clp_speed){

	lift=liftServo;
	clampLeft = clpServoR;
	clampRight = clpServoL;
	//speed are inverted to make the use of the lib more logical
	liftSpeed = 255 - lt_speed;
	clampSpeed = 255 - clp_speed;
	lastClampTime = millis();
	lastLiftTime = millis();
	isPaused = false;
	moves_ = NULL;
	
}


//---Functions

/**
  *init( )
  *assure that the claw is in it's iit state : down and fully opened
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
  *moveLift(uchar pos)
  *add a clamp move on the vertical axis
  */
void Claw::moveLift(int targPos) {
	if(lift->read() == targPos) {
		ClawMove* mv = new ClawMove(MoveType::Lift,targPos);
		if(moves_)moves_->append(mv);
		else moves_ = mv;
	}
}

/**
  *moveClamp(uchar pos)
  *add an clamp move on the opening 
  */
void Claw::moveClamp(int targPos) {
	if(clampRight->read() == targPos) {
		ClawMove* mv = new ClawMove(MoveType::Clamp,targPos);
		if(moves_)moves_->append(mv);
		else moves_ = mv;
	}
}

/**
  *load()
  *add a list of move for the clamp to execute, moving down, then closing
  *before moving up again
  */
void Claw::load() {
	
	moveLift(DOWN);
	moveClamp(CLOSE);
	moveLift(UP);

}

/**
  *unload()
  *add a list of move for the clamp to execute, moving down, then opening
  *before moving up again
  */
void Claw::unload() {
	
	moveLift(DOWN);
	moveClamp(OPEN);
	moveLift(UP);

}

/**
  *stack()
  *add a list of move for the clamp to execute, moving down, then opening
  *a little, closing and moving up again (need some rework)
  */
void Claw::stack() {

	moveLift(DOWN);
	moveClamp(CLOSE-1); //just a little bit less close than usual
	moveClamp(CLOSE);
	moveLift(UP);
	
}

  
/**
  *pauses the claw whatever it's doing
  */
void Claw::pause() {
	isPaused = true;
}

/**
  *resuming the claw move that's been paused
  */
void  Claw::unpause() {
	//TODO rename resume
	if(isPaused) {
		isPaused = false;
	}
}

/**
  *clear all the registered move
  */
void Claw::clearMoves() {
	moves_->clear();
}

/*
 *clear the curent move
 */
void Claw::clearCurrentMove () {
	ClawMove* mv = moves_;
	moves_ = moves_->getNext();
	delete mv;
}

/**
  *return a string af all the moves registered, used for debug
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
  *set the clamp speed
  */
void Claw::setClampSpeed(unsigned char clpSpeed){
	clampSpeed=255-clpSpeed;
}

/**
  *set the lift speed
  */
void Claw::setLiftSpeed(unsigned char ltSpeed){
	liftSpeed=255-ltSpeed;
}

/*
 *has to be called regulary by the main program. Allow the
 *lib to be non-blocking
 */
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
	
/*
 *open the claw wide to alow the summerbot to push the cubes with it 
 */
void Claw::openWide() {
	//to be coded
}

/*
 *return a boolean indicating the current state of the lib
 */
bool Claw::isBusy() {
	return moves_;
}

