#ifndef PINCE_H
#define PINCE_H 1

//Standard headers
#include <Arduino.h>
#include <Servo.h>

#define UP 25
#define DOWN 5
#define OPEN 50
#define CLOSE 20
#define OFFSET 0

enum MoveType {
	None = 0,
	Lift = 1,
	Clamp = 2
};

class ClawMove {
	
    public:
        MoveType type_;
        int targPos_;
        ClawMove* next_=0;
        ClawMove(MoveType type,int targPos):type_{type},targPos_{targPos}{};
        ClawMove* getNext(){
            return next_;
        }
        ClawMove* getLast(){
            if(next_) return next_->getLast();
            else return this;
        }
        void append(ClawMove* nn){
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
          return "|"+String(type_)+","+String(targPos_)+">";
        }
};

class Claw {

private:
		ClawMove *moves_;
	
	Servo *lift;
	Servo *clampLeft;
	Servo *clampRight;
	unsigned char liftSpeed;
	unsigned char clampSpeed;
	double lastClampTime;
	double lastLiftTime;
	bool isPaused;

public:

	Claw(Servo *liftServo, unsigned char lift_speed, Servo *clpServoR, Servo *clpServoL, unsigned char clamp_speed);
	void init();
	//movements
	void moveLift(int moveAngle);
	void moveClamp(int moveAngle);
	void load();
	void unload();
	void stack();
	void openWide();
	void pause();
	void unpause();
	void clearMoves();
	void clearCurrentMove();

	//speeds
	void setClampSpeed(unsigned char clpSpeed);
	void setLiftSpeed(unsigned char ltSpeed);

	//update
	void update();

	//infos
	bool isBusy();
	String movesString();
};

#endif
