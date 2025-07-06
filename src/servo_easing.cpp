#include "servo_easing.h"

void ServoEasing::setTargetAngle(uint8_t targetAngle) {
  this->targetAngle = targetAngle;

  // Start the servo's PWM pulses.
  // No PWM pulses until servo is (re)attached !!!
  servo.attach(this->servoPin);
}

void ServoEasing::update() {
  // Implement a non blocking delay for delaymS.
  if (millis() < nextUpdate) return;
  nextUpdate = millis() + delaymS;

  uint8_t differenceAngle;

  // The delay has expired so it's time to check if any movement is required.
  if (currentAngle == targetAngle) return;

  if (currentAngle < targetAngle) {
    differenceAngle = targetAngle - currentAngle;
    currentAngle++; // currentAngle needs to increase.
    direction = AngleDirection::INCREASING_ANGLE;
  }

  if (currentAngle > targetAngle) {
    differenceAngle = currentAngle - targetAngle;
    currentAngle--; // currentAngle needs to decrease.
    direction = AngleDirection::DECREASING_ANGLE;
  }

  // this->servo->write(currentAngle);
  this->servo.write(currentAngle);
  Serial.printf("\n%6ld servo %d current angle = %d", millis(), servoNumber, currentAngle);

  // If the current angle is nearing the target angle then increase the delay between updates.
  if (differenceAngle < 10) {
    nextUpdate += delaymS;
  }
  if (differenceAngle < 5) {
    nextUpdate += delaymS;
  }

  // Has the current angle reached the mid angle yet?
  if (currentAngle == midAngle) {
     // Call the callback function if one has ben set.
    if (reachedAngle) reachedAngle(servoNumber, currentAngle, direction);
  }

  // Has the current angle reached the target angle yet?
  if (currentAngle == targetAngle) {
    //servo->detach(); // Stop any more PWM pulses. // Stops PWM completely !!!
    // Call the callback function if one has been set.
    if (reachedAngle) reachedAngle(servoNumber, currentAngle, direction);
  }

  return;
}
