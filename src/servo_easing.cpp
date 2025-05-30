#include "servo_easing.h"

void ServoEasing::update() {
  // Implement a non blocking delay for delaymS.
  if (millis() < nextUpdate) return;
  nextUpdate = millis() + delaymS;

  // The delay has expired so it's time to check if any movement is required.
  if (currentAngle == targetAngle) return;

  if (currentAngle < targetAngle) {
    currentAngle++; // currentAngle needs to increase.
  }

  if (currentAngle > targetAngle) {
    currentAngle--; // currentAngle needs to decrease.
  }

  this->servo->write(currentAngle);

  // Has the currentAngle reached the targetAngle yet?
  if (currentAngle == targetAngle) {
    reachedTarget(); // Call the callback function.
  }

  return;
}
