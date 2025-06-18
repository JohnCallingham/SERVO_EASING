#ifndef SERVO_EASING_H
#define SERVO_EASING_H

#include <Arduino.h>
#include <ESP32Servo.h>

enum AngleDirection { INCREASING_ANGLE, DECREASING_ANGLE };

/**
 * Contains the servo current angle.
 * Handles the movement of the current angle towards the target angle.
 */
class ServoEasing {
  public:
    void initialise(uint8_t servoNumber, Servo *servo) { this->servoNumber = servoNumber; this->servo = servo; }

    void setInitialAngle(uint8_t initialAngle) { this->currentAngle = initialAngle; }
    void setTargetAngle(uint8_t targetAngle) { this->targetAngle = targetAngle; }
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

    unsigned long delaymS = 100; // Default is 100 mS.
    unsigned long nextUpdate = 0;

    // A callback function for when the current angle reaches the mid or target angles.
    void (*reachedAngle)(uint8_t servoNumber , uint8_t currentAngle, AngleDirection direction);

    Servo *servo;
};

#endif
