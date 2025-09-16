#ifndef SERVO_EASING_H
#define SERVO_EASING_H

#include <Arduino.h>

#define PWM_FREQ               50 // 50 Hz.
#define MICRO_SECONDS_PER_CYCLE 20000 // 20,000 microSeconds in one cycle of frequency 50 Hz.

// #define PWM_RESOLUTION         13  // Needs a minimum of 10 bit resolution, 8 bit doesn't work down to 50 Hz !!
#define PWM_RESOLUTION         14  // Needs a minimum of 10 bit resolution, 8 bit doesn't work down to 50 Hz !!

// #define TICKS_PER_CYCLE      8192 // For a PWM_RESOLUTION of 13 bits.
#define TICKS_PER_CYCLE         16384 // For a PWM_RESOLUTION of 14 bits.

#define MIN_ANGLE               0
#define MAX_ANGLE             180
#define DEFAULT_ANGLE          90

#define MIN_PULSE_WIDTH       544 // The number of microSeconds for MIN_ANGLE.
#define MAX_PULSE_WIDTH      2400 // The number of microSeconds for MAX_ANGLE.
#define DEFAULT_PULSE_WIDTH  1500 // The number of microSeconds for DEFAULT_ANGLE.

const long MIN_TICKS = (MIN_PULSE_WIDTH * TICKS_PER_CYCLE) / MICRO_SECONDS_PER_CYCLE;
const long MAX_TICKS = (MAX_PULSE_WIDTH * TICKS_PER_CYCLE) / MICRO_SECONDS_PER_CYCLE;

/**
 * Contains the servo current angle.
 * Handles the movement of the current angle towards the target angle.
 */
class ServoEasing {
  public:
    enum AngleDirection { INCREASING_ANGLE, DECREASING_ANGLE };

    void initialise(uint8_t servoNumber, uint8_t servoPin);
    void setInitialAngle(uint8_t initialAngle);
    void setMidAngle(uint8_t midAngle) { this->midAngle = midAngle; }

    void setReachedAngleCallbackFunction(void (*reachedAngle)(uint8_t servoNumber, uint8_t currentAngle, AngleDirection direction)) { this->reachedAngle = reachedAngle; }

    uint8_t getCurrentAngle() { return this->currentAngle; }

    /**
     * Causes the servo to move from its current angle to the target angle with easing in and out.
     */
    void easeTo(uint8_t targetAngle);

    /**
     * Causes the servo to move to the target angle with no easing in or out.
     */
    void moveTo(uint8_t targetAngle);

    /**
     * delaymS is the delay between each change to the current angle.
     */
    void setDelaymS(unsigned long delaymS) { this->delaymS = delaymS; }

    /**
     * Called repeadedly and checks to see if the current angle needs to be changed.
     */
    // void update();
    void loop();

  private:
    enum EasingAction { EASE_NONE, EASE_IN, EASE_OUT, EASE_IN_OUT };
    EasingAction easingAction = EASE_IN_OUT;
    
    AngleDirection direction;

    uint8_t servoNumber;

    uint8_t currentAngle;
    uint8_t targetAngle;
    uint8_t midAngle = 90; // Defaults to 90 degrees, but user configurable.

    unsigned long delaymS; // User configuratble.
    unsigned long nextUpdate = 0;

    void setTargetAngle(uint8_t targetAngle);

    // A callback function for when the current angle reaches the mid or target angles.
    void (*reachedAngle)(uint8_t servoNumber, uint8_t currentAngle, AngleDirection direction);

    // Calculates the duty cycle for the current angle and updates the PWM.
    void updatePWM(uint8_t currentAngle);

    // Stores the start and finish angles for the current movement.
    uint8_t movementStartAngle;
    uint8_t movementFinishAngle;
};

#endif
