#include "servo_easing.h"

void ServoEasing::initialise(uint8_t servoNumber, uint8_t servoPin) {
  this->servoNumber = servoNumber;

  /**
   * Initialise the PWM pulses.
   * Uses servoNumber for the PWM channel.
   */
  Serial.printf("\n%6ld setting up PWM for servo %d", millis(), servoNumber);

  ledcSetup(this->servoNumber, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(servoPin, this->servoNumber);

  // Start with no PWM pulses.
  ledcWrite(this->servoNumber, 0);

  // Need a delay to ensure that the PWM duty cycle is 0.
  delay(100); // 50 mS doesn't work.
}

void ServoEasing::setInitialAngle(uint8_t initialAngle) {
  this->currentAngle = initialAngle;

  // Move the servo to its initial angle. There is no easing.
  updatePWM(initialAngle);

  // The update() function will remove PWM pulses.
}

void ServoEasing::setTargetAngle(uint8_t targetAngle) {
  this->targetAngle = targetAngle;

  // Update the start and finish angles for this movement.
  movementStartAngle = currentAngle;
  movementFinishAngle = targetAngle;
}

void ServoEasing::update() {
  /**
   * Implement a non blocking delay for delaymS.
   */
  if (millis() < nextUpdate) return;
  nextUpdate = millis() + delaymS;

  /**
   * The delay has expired so it's time to check if any movement is required.
   */
  if (this->currentAngle == this->targetAngle) {
    // PWM pulses are turned off here to allow time for the servo to complete its final step.
    if (ledcRead(this->servoNumber > 0)) {
      // Stop any more PWM pulses.
      Serial.printf("\n%6ld servo %d turning off PWM pulses", millis(), servoNumber);
      ledcWrite(this->servoNumber, 0);
    }

    return;
  }

  // Movement is required so check which direction.
  if (currentAngle < targetAngle) {
    currentAngle++; // currentAngle needs to increase.
    direction = AngleDirection::INCREASING_ANGLE;
  }
  if (currentAngle > targetAngle) {
    currentAngle--; // currentAngle needs to decrease.
    direction = AngleDirection::DECREASING_ANGLE;
  }

  // Update the PWM duty cycle.
  updatePWM(currentAngle);
  Serial.printf("\n%6ld servo %d current angle = %d", millis(), servoNumber, currentAngle);

  if (easingAction != EasingAction::EASE_NONE) {
    // TO DO: need to add code for EASE_IN or EASE_OUT only.

    /**
     * A crude form of servo easing.
     * Calculates the current percentage of the movement.
     * Varies the delay at the start and end of the movement.
     */
    uint16_t perCent = map(currentAngle, movementStartAngle, movementFinishAngle, 1, 100 );
    if (perCent < 10) nextUpdate += (3 * delaymS);
    if (perCent < 20) nextUpdate += (2 * delaymS);
    if (perCent > 80) nextUpdate += (2 * delaymS);
    if (perCent > 90) nextUpdate += (3 * delaymS);
  }

  // Has the current angle reached the mid angle yet?
  if (currentAngle == midAngle) {
     // Call the callback function if one has been set.
    if (reachedAngle) reachedAngle(servoNumber, currentAngle, direction);
  }

  // Has the current angle reached the target angle yet?
  if (currentAngle == targetAngle) {
    // Do not turn PWM pulses off here so that there is time for the servo to move before its PWM pulses are removed.
    // Moved to the top of this function.

    // Call the callback function if one has been set.
    if (reachedAngle) reachedAngle(servoNumber, currentAngle, direction);
  }

  return;
}

void ServoEasing::updatePWM(uint8_t servoAngle) {
  // Convert servoAngle to duty cycle (ticks).
  uint32_t dutyCycle = map(servoAngle, MIN_ANGLE, MAX_ANGLE, MIN_TICKS, MAX_TICKS);

  // Write the new duty cycle to the PWM controller.
  ledcWrite(this->servoNumber, dutyCycle);
}

void ServoEasing::easeTo(uint8_t targetAngle) {
  easingAction = EasingAction::EASE_IN_OUT;
  setTargetAngle(targetAngle);
}

void ServoEasing::moveTo(uint8_t targetAngle) {
  easingAction = EasingAction::EASE_NONE;
  setTargetAngle(targetAngle);
}
