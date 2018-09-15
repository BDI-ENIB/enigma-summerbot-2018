#include "MotionBase.h"
// --- moves management ---
void MotionBase::nextMove(){
  Move* mv = moves_;
  moves_ = moves_->getNext();
  delete mv;
}
void MotionBase::clearMoves(){
  if(moves_) moves_->clear();
  delete moves_;
  moves_ = 0;
}
void MotionBase::computeMoveCoords(double ix,double iy,double ia,Move* mv,double* ox, double* oy, double* oa){
  double x=ix,y=iy,a=ia;
  if(mv->isRotation_){
    a+=(mv->direction_?-1:1)*mv->steps_*wheelRadius_*2.0*3.141592654359/STEP_PER_REVOLUTION/ROTATION_MICROSTEPS/robotRadius_;
  }else{
    double d=(mv->direction_?-1:1)*mv->steps_*wheelRadius_*2.0*3.141592654359/STEP_PER_REVOLUTION/TRANSLATION_MICROSTEPS;
    x+=cos(a)*d;
    y+=sin(a)*d;
  }
  *ox=x;
  *oy=y;
  *oa=a;
}
void MotionBase::computeMoveCoords(Move* mv,double* ox, double* oy, double* oa){
  computeMoveCoords(prevX_,prevY_,prevA_,mv,ox,oy,oa);
}
void MotionBase::computeLastMoveCoords(){
  Move* mv =moves_;
  double x=prevX_,y=prevY_,a=prevA_;
  while(mv){
    computeMoveCoords(x,y,a,mv,&x,&y,&a);
    mv=mv->getNext();
  }
  lastMoveX_=x;
  lastMoveY_=y;
  lastMoveA_=a;
}
String MotionBase::movesString(){
  String r="";
  Move* mv =moves_;
  while(mv){
    r+=mv->toString();
    mv=mv->getNext();
  }
  return r;
}
// --- moves creators ---
void MotionBase::translate(double distance){
    if(distance==0)return;
    long steps=fabs(distance)*TRANSLATION_MICROSTEPS*STEP_PER_REVOLUTION/wheelRadius_/2.0/3.141592654359;
    Move* mv = new Move(false,distance<0,steps);
    if(moves_)moves_->append(mv);
    else moves_ = mv;
}
void MotionBase::rotate(double rotation){
    if(rotation==0)return;
    long steps=fabs(rotation)*ROTATION_MICROSTEPS*STEP_PER_REVOLUTION*robotRadius_/wheelRadius_/2.0/3.141592654359;
    //Serial.println(steps);
    Move* mv = new Move(true,rotation<0,steps);
    if(moves_)moves_->append(mv);
    else moves_ = mv;
}
void MotionBase::moveTo(double x,double y){
  computeLastMoveCoords();
  double r=atan2(y-lastMoveY_,x-lastMoveX_);
  rotate(r-lastMoveA_);
  translate(sqrt((x-lastMoveX_)*(x-lastMoveX_)+(y-lastMoveY_)*(y-lastMoveY_)));
}
void MotionBase::moveTo(double x,double y,double a){
  computeLastMoveCoords();
  double r=atan2(y-lastMoveY_,x-lastMoveX_);
  //Serial.println(r-lastMoveA_);
  rotate(r-lastMoveA_);
  translate(sqrt((x-lastMoveX_)*(x-lastMoveX_)+(y-lastMoveY_)*(y-lastMoveY_)));
  //Serial.println(a-r);
  rotate(a-r);
}
// ---  ---
bool MotionBase::update(){
  if(paused_)return false;
  bool ended = false;
  if(motionStarted_){
    if(moves_){
      const DualDRV8825::State driverState = driver_->getCurrentState();
      if(driverState == DualDRV8825::STOPPED){ //the robot is not moving
        computeMoveCoords(moves_,&prevX_,&prevY_,&prevA_);
        moves_= moves_->getNext();
        motionStarted_ = false;
        //Serial.println("stopped");
      }
    }else{
      //error handling
      
    }
  }else if(moves_){
    
    if(moves_->isRotation_){
      driver_->setMicrostep(ROTATION_MICROSTEPS);
    }else{
      driver_->setMicrostep(TRANSLATION_MICROSTEPS);
    }
    //Serial.println("asyncMove");
    //Serial.println(movesString());
    driver_->asyncMove((moves_->direction_?-1:1)*moves_->steps_,moves_->isRotation_);
    motionStarted_ = true;
  }else{
    ended = true;
  }
  driver_->update();
  return ended;
}
void MotionBase::pause(){
  const DualDRV8825::State driverState = driver_->getCurrentState();
  if(driverState != DualDRV8825::STOPPED){ //the robot is moving
    if(moves_){
      const long remSteps = driver_->getRemainingSteps();
      const long doneSteps = moves_->steps_ - remSteps;
      moves_->steps_=doneSteps;
      computeMoveCoords(moves_,&prevX_,&prevY_,&prevA_);
      moves_->steps_=remSteps;
      motionStarted_=false;
    }
    driver_->stop();
  }
  paused_=true;
}
void MotionBase::resume(){
  paused_=false;
}
void MotionBase::setPosition(double x,double y,double a){
  prevX_=x;
  prevY_=y;
  prevA_=a;
}
