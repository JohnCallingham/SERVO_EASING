#ifndef SERVO_EASING_H
#define SERVO_EASING_H

#include <Arduino.h>
#include <ESP32Servo.h>

/**
 * TO DO: Need to indicate the direction of movement when passing the mid angle.
 */

/**
 * Contains the servo current angle.
 * Handles the movement of the current angle towards the target angle.
 */
class ServoEasing {
  public:
    void initialise(Servo *servo, uint8_t initialAngle) { this->currentAngle = initialAngle; this->servo = servo; }
    void setTargetAngle(uint8_t targetAngle) { this->targetAngle = targetAngle; }
    void setMidAngle(uint8_t midAngle) { this->midAngle = midAngle; }
    void setReachedTargetAngleCallbackFunction(void (*reachedTargetAngle)()) { this->reachedTargetAngle = reachedTargetAngle; }
    void setReachedMidAngleCallbackFunction(void (*reachedMidAngle)()) { this->reachedMidAngle = reachedMidAngle; }

    /**
     * delaymS is the delay between each change to the current angle.
     */
    void setDelaymS(unsigned long delaymS) { this->delaymS = delaymS; }

    /**
     * Checks to see if the currentAngle needs to be changed.
     */
    void update();

  private:
    uint8_t currentAngle;
    uint8_t targetAngle;
    uint8_t midAngle = 90; // Defaults to 90 degrees.
    unsigned long delaymS = 100; // Default is 100 mS.
    unsigned long nextUpdate = 0;
    void (*reachedTargetAngle)(); // A callback function for when the current angle reaches the target angle.
    void (*reachedMidAngle)(); // A callback function for when the current angle reaches the mid angle.
    Servo *servo;

};

#endif
