#include "servo_easing.h"

void ServoEasing::update() {
  // Implement a non blocking delay for delaymS.
  if (millis() < nextUpdate) return;
  nextUpdate = millis() + delaymS;

  // The delay has expired so it's time to check if any movement is required.
  if (currentAngle == targetAngle) return;

  if (currentAngle < targetAngle) {
    currentAngle++; // currentAngle needs to increase.
    direction = AngleDirection::INCREASING_ANGLE;
  }

  if (currentAngle > targetAngle) {
    currentAngle--; // currentAngle needs to decrease.
    direction = AngleDirection::DECREASING_ANGLE;
  }

  this->servo->write(currentAngle);
  Serial.printf("\ncurrent angle = %d", currentAngle);

  // Has the current angle reached the mid angle yet?
  if (currentAngle == midAngle) {
    if (reachedMidAngle) reachedMidAngle(currentAngle, direction); // Call the callback function if one has ben set.
  }

  // Has the current angle reached the target angle yet?
  if (currentAngle == targetAngle) {
    if (reachedTargetAngle) reachedTargetAngle(currentAngle, direction); // Call the callback function if one has ben set.
  }

  return;
}
