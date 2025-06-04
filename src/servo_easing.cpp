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

  // Has the current angle reached the mid angle yet?
  // TO DO: need to test that the code runs OK if no mid angle callback function is set.
  if (currentAngle == midAngle) {
    if (reachedMidAngle) reachedMidAngle(); // Call the callback function if one has ben set.
  }

  // Has the current angle reached the target angle yet?
  if (currentAngle == targetAngle) {
    reachedTargetAngle(); // Call the callback function.
  }

  return;
}
