#ifndef SERVO_EASING_H
#define SERVO_EASING_H

#include <Arduino.h>

/**
 * Contains the servo current angle.
 * Handles the movement of the current angle towards the target angle.
 */
class SERVO_EASING {
  public:
    void initialise(uint8_t initialAngle) { this->currentAngle = initialAngle; }
    void setReachedTargetCallbackFunction(void (*reachedTarget)()) { this->reachedTarget = reachedTarget; }
    void setTargetAngle(uint8_t targetAngle) { this->targetAngle = targetAngle; }

    /**
     * delaymS is the delay between each change to the current angle.
     */
    void setDelaymS(unsigned long delaymS) { this->delaymS = delaymS; }

    /**
     * Checks to see if the currentAngle needs to be changed.
     * Returns false if there is no need to move the servo, or it is still moving.
     * Returns true if the currentAngle has reached the targetAngle.
     */
    bool update();

  private:
    uint8_t currentAngle;
    uint8_t targetAngle;
    unsigned long delaymS = 100; // Default is 100 mS.
    unsigned long nextUpdate = 0;
    void (*reachedTarget)(); // A callback function for when the current angle reaches the target angle.

};

#endif
