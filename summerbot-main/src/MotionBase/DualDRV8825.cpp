
#include "DualDRV8825.h"
#include <Arduino.h>

const uint8_t DualDRV8825::MS_TABLE[] = {0b000, 0b001, 0b010, 0b011, 0b100, 0b111};

/*
 * Basic connection: only DIR, STEP are connected.
 * Microstepping controls should be hardwired.
 */
DualDRV8825::DualDRV8825(short steps, short ldir_pin, short lstep_pin, short rdir_pin, short rstep_pin)
:motor_steps(steps), ldir_pin(ldir_pin), lstep_pin(lstep_pin), rdir_pin(rdir_pin), rstep_pin(rstep_pin)
{}

DualDRV8825::DualDRV8825(short steps, short ldir_pin, short lstep_pin, short rdir_pin, short rstep_pin, short enable_pin)
:motor_steps(steps), ldir_pin(ldir_pin), lstep_pin(lstep_pin), rdir_pin(rdir_pin), rstep_pin(rstep_pin), enable_pin(enable_pin)
{}

DualDRV8825::DualDRV8825(short steps, short ldir, short lstep, short rdir, short rstep, short ms1, short ms2, short ms3)
:motor_steps(steps), ldir_pin(ldir), lstep_pin(lstep), rdir_pin(rdir), rstep_pin(rstep),ms1_pin(ms1), ms2_pin(ms2), ms3_pin(ms3)
{}

DualDRV8825::DualDRV8825(short steps, short ldir_pin, short lstep_pin, short rdir_pin, short rstep_pin, short enable_pin, short ms1_pin, short ms2_pin, short ms3_pin)
:motor_steps(steps), ldir_pin(ldir_pin), lstep_pin(lstep_pin), rdir_pin(rdir_pin), rstep_pin(rstep_pin), enable_pin(enable_pin),ms1_pin(ms1_pin), ms2_pin(ms2_pin), ms3_pin(ms3_pin)
{}
/*
 * Initialize pins, calculate timings etc
 */
void DualDRV8825::begin(short rpm, short microsteps){
    pinMode(ldir_pin, OUTPUT);
    pinMode(rdir_pin, OUTPUT);
    digitalWrite(ldir_pin, HIGH);
    digitalWrite(rdir_pin, HIGH);

    pinMode(lstep_pin, OUTPUT);
    pinMode(rstep_pin, OUTPUT);
    digitalWrite(lstep_pin, LOW);
    digitalWrite(rstep_pin, LOW);

    if IS_CONNECTED(enable_pin){
        pinMode(enable_pin, OUTPUT);
        digitalWrite(enable_pin, HIGH); // disable
    }

    this->rpm = rpm;
    setMicrostep(microsteps);

    enable();
    if (!IS_CONNECTED(ms1_pin) || !IS_CONNECTED(ms2_pin) || !IS_CONNECTED(ms3_pin)){
        return;
    }

    pinMode(ms1_pin, OUTPUT);
    pinMode(ms2_pin, OUTPUT);
    pinMode(ms3_pin, OUTPUT);
}

/*
 * Set target motor RPM (1-200 is a reasonable range)
 */
void DualDRV8825::setRPM(short rpm){
    if (this->rpm == 0){        // begin() has not been called (old 1.0 code)
        begin(rpm, microsteps);
    }
    this->rpm = rpm;
}

/*
 * Set stepping mode (1:microsteps)
 * Allowed ranges for DualDRV8825 are 1:1 to 1:128
 */
short DualDRV8825::setMicrostep(short microsteps){
    for (short ms=1; ms <= getMaxMicrostep(); ms<<=1){
        if (microsteps == ms){
            this->microsteps = microsteps;
            break;
        }
    }

    if (!IS_CONNECTED(ms1_pin) || !IS_CONNECTED(ms2_pin) || !IS_CONNECTED(ms3_pin)){
        return this->microsteps;
    }

    const uint8_t* ms_table = getMicrostepTable();
    size_t ms_table_size = getMicrostepTableSize();

    unsigned short i = 0;
    while (i < ms_table_size){
        if (this->microsteps & (1<<i)){
            uint8_t mask = ms_table[i];
            digitalWrite(ms3_pin, mask & 4);
            digitalWrite(ms2_pin, mask & 2);
            digitalWrite(ms1_pin, mask & 1);
            break;
        }
        i++;
    }
    return this->microsteps;
}

const uint8_t* DualDRV8825::getMicrostepTable()
{
    return (uint8_t*)DualDRV8825::MS_TABLE;
}

size_t DualDRV8825::getMicrostepTableSize()
{
    return sizeof(MS_TABLE);
}

short DualDRV8825::getMaxMicrostep(){
    return DualDRV8825::MAX_MICROSTEP;
}


/*
 * Set speed profile - CONSTANT_SPEED, LINEAR_SPEED (accelerated)
 * accel and decel are given in [full steps/s^2]
 */
void DualDRV8825::setSpeedProfile(Mode mode, short accel, short decel){
    profile.mode = mode;
    profile.accel = accel;
    profile.decel = decel;
}
void DualDRV8825::setSpeedProfile(struct Profile profile){
    this->profile = profile;
}

/*
 * Move the motor a given number of steps.
 * positive to move forward, negative to reverse
 */
void DualDRV8825::syncMove(long steps,short rotate){
    asyncMove(steps,rotate);
    while (update());
}
/*
 * Set up a new move or alter an active move (calculate and save the parameters)
 */
void DualDRV8825::asyncMove(long steps,short rotate){
    long speed;
    if (steps_remaining>0){
        alterMove(steps,rotate);
    } else {
        // set up new move
        dir_state = (steps >= 0) ? HIGH : LOW;
        rot_state = rotate ? HIGH : LOW;
        last_action_end = 0;
        steps_remaining = abs(steps);
        step_count = 0;
        rest = 0;
        switch (profile.mode){
        case LINEAR_SPEED:
            // speed is in [steps/s]
            speed = rpm * motor_steps / 60;
            // how many steps from 0 to target rpm
            steps_to_cruise = speed * speed * microsteps / (2 * profile.accel);
            // how many steps are needed from target rpm to a full stop
            steps_to_brake = steps_to_cruise * profile.accel / profile.decel;
            if (steps_remaining < steps_to_cruise + steps_to_brake){
                // cannot reach max speed, will need to brake early
                steps_to_cruise = steps_remaining * profile.decel / (profile.accel + profile.decel);
                steps_to_brake = steps_remaining - steps_to_cruise;
            }
            // Initial pulse (c0) including error correction factor 0.676 [us]
            step_pulse = (1e+6)*0.676*sqrt(2.0f/(profile.accel*microsteps));
            break;
    
        case CONSTANT_SPEED:
        default:
            step_pulse = STEP_PULSE(rpm, motor_steps, microsteps);
            steps_to_cruise = 0;
            steps_to_brake = 0;
        }
    }
}
/*
 * Alter a running move by adding/removing steps
 * FIXME: This is a naive implementation and it only works well in CRUISING state
 */
void DualDRV8825::alterMove(long steps,short rotate){
    switch (getCurrentState()){
    case ACCELERATING: // this also works but will keep the original speed target
    case CRUISING:
        rot_state = rotate ? HIGH : LOW;
        if (steps >= 0){
            steps_remaining += steps;
        } else {
            steps_remaining = max(steps_to_brake, steps_remaining+steps);
        };
        break;
    case DECELERATING:
        // would need to start accelerating again -- NOT IMPLEMENTED
        break;
    case STOPPED:
        asyncMove(steps,rotate);
        break;
    }
}
/*
 * Brake early.
 */
void DualDRV8825::startBrake(void){
    switch (getCurrentState()){
    case CRUISING:  // this applies to both CONSTANT_SPEED and LINEAR_SPEED modes
        steps_remaining = steps_to_brake;
        break;

    case ACCELERATING:
        steps_remaining = step_count * profile.accel / profile.decel;
        break;

    default:
        break; // nothing to do if already stopped or braking
    }
}
/*
 * Stop movement immediately.
 */
void DualDRV8825::stop(void){
    steps_remaining = 0;
}
/*
 * Return calculated time to complete the given move
 */
long DualDRV8825::getTimeForMove(long steps){
    long t;
    switch (profile.mode){
        case LINEAR_SPEED:
            asyncMove(steps);
            t = (long)(
                  sqrt((double)(2 * steps_to_cruise / profile.accel)) + 
                  (steps_remaining - steps_to_cruise - steps_to_brake) * STEP_PULSE(rpm, motor_steps, microsteps) +
                  sqrt((double)(2 * steps_to_brake / profile.decel))
                );
            break;
        case CONSTANT_SPEED:
        default:
            t = STEP_PULSE(rpm, motor_steps, microsteps);
    }
    return t;
}
/*
 * calculate the interval til the next pulse
 */
void DualDRV8825::calcStepPulse(void){
    if (steps_remaining <= 0){  // this should not happen, but avoids strange calculations
        return;
    }

    steps_remaining--;
    step_count++;

    if (profile.mode == LINEAR_SPEED){
        switch (getCurrentState()){
        case ACCELERATING:
            step_pulse = step_pulse - (2*step_pulse+rest)/(4*step_count+1);
            rest = (step_count < steps_to_cruise) ? (2*step_pulse+rest) % (4*step_count+1) : 0;
            break;

        case DECELERATING:
            step_pulse = step_pulse - (2*step_pulse+rest)/(-4*steps_remaining+1);
            rest = (2*step_pulse+rest) % (-4*steps_remaining+1);
            break;

        default:
            break; // no speed changes
        }
    }
}
/*
 * Yield to step control
 * Toggle step and return if the motor is working
 */
bool DualDRV8825::update(void){
    if (steps_remaining > 0){
        if(micros()>next_action_interval+last_action_end){
          /*
           * DIR pin is sampled on rising STEP edge, so it is set first
           */
          digitalWrite(ldir_pin, dir_state);
          digitalWrite(rdir_pin, dir_state != rot_state? HIGH:LOW);
          digitalWrite(lstep_pin, HIGH);
          digitalWrite(rstep_pin, HIGH);
          unsigned m = micros();
          long pulse = step_pulse; // save value because calcStepPulse() will overwrite it
          calcStepPulse();
          m = micros() - m;
          // We should pull HIGH for 1-2us (step_high_min)
          if (m < step_high_min){ // fast MCPU or CONSTANT_SPEED
              delayMicros(step_high_min-m);
              m = step_high_min;
          };
          digitalWrite(lstep_pin, LOW);
          digitalWrite(rstep_pin, LOW);
          // account for calcStepPulse() execution time; sets ceiling for max rpm on slower MCUs
          last_action_end = micros();
          next_action_interval = (pulse > (long)m) ? pulse - m : 1;
        }
        return true;
    } else {
        // end of move
        last_action_end = 0;
        next_action_interval = 0;
        return false;
    }
}

enum DualDRV8825::State DualDRV8825::getCurrentState(void){
    enum State state;
    if (steps_remaining <= 0){
        state = STOPPED;
    } else {
        if (steps_remaining <= steps_to_brake){
            state = DECELERATING;
        } else if (step_count <= steps_to_cruise){
            state = ACCELERATING;
        } else {
            state = CRUISING;
        }
    }
    return state;
}

/*
 * Enable/Disable the motor by setting a digital flag
 */
void DualDRV8825::enable(void){
    if IS_CONNECTED(enable_pin){
        digitalWrite(enable_pin, LOW);
    }
}

void DualDRV8825::disable(void){
    if IS_CONNECTED(enable_pin){
        digitalWrite(enable_pin, HIGH);
    }
}

