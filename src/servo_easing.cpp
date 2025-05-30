#include "servo_easing.h"

bool ServoEasing::update() {
  // Implement a non blocking delay for delaymS.
  if (millis() < nextUpdate) return false;
  nextUpdate = millis() + delaymS;

  // The delay has expired so it's time to check if any movement is required.
  if (currentAngle == targetAngle) return false;

  if (currentAngle < targetAngle) {
    currentAngle++; // currentAngle needs to increase.
  }

  if (currentAngle > targetAngle) {
    currentAngle--; // currentAngle needs to decrease.
  }

  // Has the currentAngle reached the targetAngle yet?
  if (currentAngle == targetAngle) {
    reachedTarget(); // Call the callback function.
    return true;
  }

  return false;
}


// HOW to pass back the current servo angle so that the servo can move???
