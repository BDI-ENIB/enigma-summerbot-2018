#ifndef MOTIONBASE_H
#define MOTIONBASE_H
 
#include <Arduino.h>
#include "DualDRV8825.h"

#define STEP_PER_REVOLUTION 200
#define RPM 240
#define MOTOR_ACCEL 100
#define MOTOR_DECEL 100
#define TRANSLATION_MICROSTEPS 8 //micro-steps while moving in straight line
#define ROTATION_MICROSTEPS 32 //micro-steps while rotating
class Move{
    public:
        bool isRotation_,direction_;
        long steps_;
        Move* next_=0;
        Move(bool isRotation,bool direction,long steps):isRotation_{isRotation},direction_{direction},steps_{steps}{};
        Move* getNext(){
            return next_;
        }
        Move* getLast(){
            if(next_) return next_->getLast();
            else return this;
        }
        void append(Move* nn){
            if(next_){
                next_->append(nn);
            }else{
                next_ = nn;
            }
        };
        void clear(){
            if(next_){
                next_->clear();
                delete next_;
            }
        }
        String toString(){
          return "|"+String(isRotation_)+","+String(direction_)+","+String(steps_)+">";
        }
};
 
class MotionBase{
    private:
        double prevX_;
        double prevY_;
        double prevA_;
        double lastMoveX_;
        double lastMoveY_;
        double lastMoveA_;
        bool paused_ = false;
        bool motionStarted_ = false;
        Move *moves_=(Move*)0;//chained list
        DualDRV8825 *driver_=(DualDRV8825*)0;
        double wheelRadius_;
        double robotRadius_;
    public:
        MotionBase(DualDRV8825* dd,double wheelRadius,double robotRadius,double x=0,double y=0,double a=0)
        :driver_{dd},wheelRadius_{wheelRadius},robotRadius_{robotRadius},prevX_{x},prevY_{y},prevA_{a}{
          driver_->begin(RPM,TRANSLATION_MICROSTEPS);
          driver_->setSpeedProfile(driver_->LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
        };
        // --- moves management ---
        void nextMove();
        void clearMoves();
        void computeMoveCoords(double ix,double iy,double ia,Move* mv,double* ox, double* oy, double* oa);
        void computeMoveCoords(Move* mv,double* ox, double* oy, double* oa);
        void computeLastMoveCoords();
        String movesString();
        // --- moves creators ---
        void translate(double distance);
        void rotate(double rotation);
        void moveTo(double x,double y,double a);
        void moveTo(double x,double y);
        // ---  ---
        bool update();
        void pause();
        void resume();

        void setPosition(double x,double y,double a);
        double getX(){
          return prevX_;
        }
        double getY(){
          return prevY_;
        }
        double getA(){
          return prevA_;
        }
        double getLastMoveX(){
          return lastMoveX_;
        }
        double getLastMoveY(){
          return lastMoveY_;
        }
        double getLastMoveA(){
          return lastMoveA_;
        }
        bool isBusy(){
          return !!(moves_);
        }
};
#endif
