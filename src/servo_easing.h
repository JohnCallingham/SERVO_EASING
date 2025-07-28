#ifndef SERVO_EASING_H
#define SERVO_EASING_H

/**
 * FEETECH (formerly known as Fitec) FS90 servo.
 * Specifications;-
 * https://www.addicore.com/products/feetech-fitec-fs90-9g-mini-servo-with-accessories
 * 
 * Description;-
 * The FS90 is a 9g analog servo from FEETECH (formerly known as Fitec)
 * which is similar in form and function to the Towerpro SG90.
 * The specifications state that this servo has a 120° operating angle
 * for standard servo pulses between 900 µs and 2100 µs.
 * As with most servos, the pulse range can be expanded to achieve
 * an expanded operating angle,
 * but the limits of which are not specified by FEETECH.
 */

 #include <Arduino.h>

#define PWM_FREQ               50 // 50 Hz.
#define MICRO_SECONDS_PER_CYCLE 20000 // 20,000 microSeconds in a cycle of frequency 50 Hz.
// #define PWM_RESOLUTION         13  // Needs a minimum of 10 bit resolution, 8 bit doesn't work down to 50Hz !!
#define PWM_RESOLUTION         14  // Needs a minimum of 10 bit resolution, 8 bit doesn't work down to 50Hz !!

#define MIN_ANGLE               0
#define MAX_ANGLE             180
#define DEFAULT_ANGLE          90

#define MIN_PULSE_WIDTH       544 // The number of microSeconds for MIN_ANGLE.
#define MAX_PULSE_WIDTH      2400 // The number of microSeconds for MAX_ANGLE.
#define DEFAULT_PULSE_WIDTH  1500 // The number of microSeconds for DEFAULT_ANGLE.

// #define TICKS_PER_CYCLE      8192 // As PWM_RESOLUTION is 13 bits.
#define TICKS_PER_CYCLE      (32768/2) // As PWM_RESOLUTION is 14 bits.
// #define MIN_TICKS            (MIN_PULSE_WIDTH * TICKS_PER_CYCLE) / MICRO_SECONDS_PER_CYCLE
// #define MAX_TICKS            (MAX_PULSE_WIDTH * TICKS_PER_CYCLE) / MICRO_SECONDS_PER_CYCLE

const long MIN_TICKS = (MIN_PULSE_WIDTH * TICKS_PER_CYCLE) / MICRO_SECONDS_PER_CYCLE;
const long MAX_TICKS = (MAX_PULSE_WIDTH * TICKS_PER_CYCLE) / MICRO_SECONDS_PER_CYCLE;

// const long cMIN_TICKS = MIN_TICKS;
// const long cMAX_TICKS = MAX_TICKS;

enum AngleDirection { INCREASING_ANGLE, DECREASING_ANGLE };

/**
 * Contains the servo current angle.
 * Handles the movement of the current angle towards the target angle.
 */
class ServoEasing {
  public:
    void initialise(uint8_t servoNumber, uint8_t servoPin);
    void setInitialAngle(uint8_t initialAngle);
    void setTargetAngle(uint8_t targetAngle);
    void setMidAngle(uint8_t midAngle) { this->midAngle = midAngle; }

    void setReachedAngleCallbackFunction(void (*reachedAngle)(uint8_t servoNumber, uint8_t currentAngle, AngleDirection direction)) { this->reachedAngle = reachedAngle; }

    uint8_t getCurrentAngle() { return this->currentAngle; }

    /**
     * delaymS is the delay between each change to the current angle.
     */
    void setDelaymS(unsigned long delaymS) { this->delaymS = delaymS; }

    /**
     * Checks to see if the currentAngle needs to be changed.
     */
    void update();

  private:
    AngleDirection direction;
    uint8_t servoNumber;

    uint8_t currentAngle;
    uint8_t targetAngle;
    uint8_t midAngle = 90; // Defaults to 90 degrees.

    unsigned long delaymS; // Default is 100 mS. no default as now user configuratble.
    unsigned long nextUpdate = 0;

    // A callback function for when the current angle reaches the mid or target angles.
    void (*reachedAngle)(uint8_t servoNumber, uint8_t currentAngle, AngleDirection direction);

    // Calculates the duty cycle for the current angle and updates the PWM.
    void updatePWM(uint8_t currentAngle);

    // // Stores the start and finish angles and ticks for the current movement.
    // Stores the start and finish angles for the current movement.
    uint8_t movementStartAngle;
    uint8_t movementFinishAngle;
    // uint16_t movementStartTick;
    // uint16_t movementFinishTick;
};

#endif
