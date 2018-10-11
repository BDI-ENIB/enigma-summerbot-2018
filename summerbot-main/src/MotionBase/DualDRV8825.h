/*
 * Generic Stepper Motor Driver For Interupt Updates
 * 
 * By Titouan Baillon for ARENIB
 * Based on work from Laurentiu Badea
 */
#ifndef DUAL_DRV8825_H
#define DUAL_DRV8825_H
#include <Arduino.h>

// used internally by the library to mark unconnected pins
#define PIN_UNCONNECTED -1
#define IS_CONNECTED(pin) (pin != PIN_UNCONNECTED)

/*
 * calculate the step pulse in microseconds for a given rpm value.
 * 60[s/min] * 1000000[us/s] / microsteps / steps / rpm
 */
#define STEP_PULSE(steps, microsteps, rpm) (60*1000000L/steps/microsteps/rpm)

// don't call yield if we have a wait shorter than this
#define MIN_YIELD_MICROS 50

/*
 * Basic Stepper Driver class.
 * Microstepping level should be externally controlled or hardwired.
 */
class DualDRV8825 {
public:
    enum Mode {CONSTANT_SPEED, LINEAR_SPEED};
    enum State {STOPPED, ACCELERATING, CRUISING, DECELERATING};
    struct Profile {
        Mode mode = CONSTANT_SPEED;
        short accel = 1000;     // acceleration [steps/s^2]
        short decel = 1000;     // deceleration [steps/s^2]    
    };
    static inline void delayMicros(unsigned long delay_us, unsigned long start_us = 0){
        if (delay_us){
            if (!start_us){
                start_us = micros();
            }
            if (delay_us > MIN_YIELD_MICROS){
                yield();
            }
            // See https://www.gammon.com.au/millis
            while (micros() - start_us < delay_us);
        }
    }

private:
    // calculation remainder to be fed into successive steps to increase accuracy (Atmel DOC8017)
    long rest;
    unsigned long last_action_end = 0;
    unsigned long next_action_interval = 0;

protected:
    /*
     * Motor Configuration
     */
    short motor_steps;           // motor steps per revolution (usually 200)

    /*
     * Driver Configuration
     */
    short ldir_pin;
    short lstep_pin;
    short rdir_pin;
    short rstep_pin;
    short enable_pin = PIN_UNCONNECTED;
    static const uint8_t MS_TABLE[];
    short ms1_pin = PIN_UNCONNECTED;
    short ms2_pin = PIN_UNCONNECTED;
    short ms3_pin = PIN_UNCONNECTED;
    virtual const uint8_t* getMicrostepTable();
    virtual size_t getMicrostepTableSize();
    short getMaxMicrostep();
    // current microstep level (1,2,4,8,...), must be < getMaxMicrostep()
    short microsteps = 1;
    // tWH(STEP) pulse duration, STEP high, min value (us)
    static const int step_high_min = 2;
    // tWL(STEP) pulse duration, STEP low, min value (1.9us)
    static const int step_low_min = 2;
    // tWAKE wakeup time, nSLEEP inactive to STEP (1000us)
    static const int wakeup_time = 1700;

    short rpm = 0;

    /*
     * Movement state
     */
    struct Profile profile;

    long step_count;        // current position
    long steps_remaining = 0;   // to complete the current move (absolute value)
    long steps_to_cruise;   // steps to reach cruising (max) rpm
    long steps_to_brake;    // steps needed to come to a full stop
    long step_pulse;        // step pulse duration (microseconds)
    long nextStepTime;

    // DIR pin state
    short dir_state;
    short rot_state;

    void calcStepPulse(void);

    // this is internal because one can call the start methods while CRUISING to get here
    void alterMove(long steps,short rotate);

private:
    // microstep range (1, 16, 32 etc)
    static const short MAX_MICROSTEP = 32;

public:
    /*
     * Basic connection: DIR, STEP are connected.
     */
    DualDRV8825(short steps, short ldir_pin, short lstep_pin, short rdir_pin, short rstep_pin);
    DualDRV8825(short steps, short ldir_pin, short lstep_pin, short rdir_pin, short rstep_pin, short enable_pin);
    DualDRV8825(short steps, short ldir_pin, short lstep_pin, short rdir_pin, short rstep_pin, short ms1_pin, short ms2_pin, short ms3_pin);
    DualDRV8825(short steps, short ldir_pin, short lstep_pin, short rdir_pin, short rstep_pin, short enable_pin, short ms1_pin, short ms2_pin, short ms3_pin);
    /*
     * Initialize pins, calculate timings etc
     */
    void begin(short rpm=60, short microsteps=1);
    /*
     * Set current microstep level, 1=full speed, 32=fine microstepping
     * Returns new level or previous level if value out of range
     */
    short setMicrostep(short microsteps);
    short getMicrostep(void){
        return microsteps;
    }
    short getSteps(void){
        return motor_steps;
    }
    /*
     * Set target motor RPM (1-200 is a reasonable range)
     */
    void setRPM(short rpm);
    short getRPM(void){
        return rpm;
    };
    short getCurrentRPM(void){
        return (short)(60*1000000L / step_pulse / microsteps / motor_steps);
    }
    /*
     * Set speed profile - CONSTANT_SPEED, LINEAR_SPEED (accelerated)
     * accel and decel are given in [full steps/s^2]
     */
    void setSpeedProfile(Mode mode, short accel=1000, short decel=1000);
    void setSpeedProfile(struct Profile profile);
    struct Profile getSpeedProfile(void){
        return profile;
    }
    short getAcceleration(void){
        return profile.accel;
    }
    short getDeceleration(void){
        return profile.decel;
    }
    /*
     * Move the motor a given number of steps.
     * positive to move forward, negative to reverse
     */
    void syncMove(long steps,short rotate=0);
    /*
     * Turn off/on motor to allow the motor to be moved by hand/hold the position in place
     */
    void enable(void);
    void disable(void);
    /*
     * Methods for non-blocking mode.
     * They use more code but allow doing other operations between impulses.
     * The flow has two parts - start/initiate followed by looping with nextAction.
     * See NonBlocking example.
     */
    /*
     * Initiate a move over known distance (calculate and save the parameters)
     * Pick just one based on move type and distance type.
     */
    void asyncMove(long steps,short rotate=0);
    /*
     * Toggle step at the right time and return if the motor is working
     */
    bool update(void);
    /*
     * Optionally, call this to begin braking (and then stop) early
     * For constant speed, this is the same as stop()
     */
    void startBrake(void);
    /*
     * Immediate stop
     */
    void stop(void);
    /*
     * State querying
     */
    enum State getCurrentState(void);

    /*
     * Return calculated time to complete the given move
     */
    long getTimeForMove(long steps);
    long getRemainingSteps(){
      return steps_remaining;
    }
};
#endif // STEPPER_DRIVER_BASE_H

