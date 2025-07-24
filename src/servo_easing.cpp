#include "servo_easing.h"

void ServoEasing::initialise(uint8_t servoNumber, uint8_t servoPin) {
  this->servoNumber = servoNumber;

  /**
   * Initialise the PWM pulses.
   * Uses servoNumber for the PWM channel.
   */
  ledcSetup(this->servoNumber, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(servoPin, this->servoNumber);

  // Start with no PWM pulses.
  ledcWrite(this->servoNumber, 0);
}

void ServoEasing::setInitialAngle(uint8_t initialAngle) {
  this->currentAngle = initialAngle;
  
  // Move the servo to its initial angle.
  updatePWM(initialAngle);
  // // Bypass updatePWM() in case servo easing is being used.
  // // Convert servoAngle to duty cycle (ticks).
  // uint16_t dutyCycle = map(initialAngle, MIN_ANGLE, MAX_ANGLE, MIN_TICKS, MAX_TICKS);
  // //Serial.printf("\ndutyCycle = %d", dutyCycle);

  // // Write the new duty cycle to the PWM controller.
  // ledcWrite(this->servoNumber, dutyCycle);

  // Allow some time for servo to move. How much??
  delay(1000);

  // Stop the PWM pulses.
  ledcWrite(this->servoNumber, 0);
}

void ServoEasing::setTargetAngle(uint8_t targetAngle) {
  this->targetAngle = targetAngle;

  // Update the start and finish angles and ticks for this movement.
  movementStartAngle = currentAngle;
  movementFinishAngle = targetAngle;
  movementStartTick = map(currentAngle, MIN_ANGLE, MAX_ANGLE, MIN_TICKS, MAX_TICKS);
  movementFinishTick = map(targetAngle, MIN_ANGLE, MAX_ANGLE, MIN_TICKS, MAX_TICKS);
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
  if (currentAngle == targetAngle) return;

  //Serial.printf("\ndelaymS = %d", this->delaymS);

  /**
   * Movement is required so check which direction.
   */
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

  // Has the current angle reached the mid angle yet?
  if (currentAngle == midAngle) {
     // Call the callback function if one has been set.
    if (reachedAngle) reachedAngle(servoNumber, currentAngle, direction);
  }

  // Has the current angle reached the target angle yet?
  if (currentAngle == targetAngle) {
    // Stop any more PWM pulses.
    ledcWrite(this->servoNumber, 0);

    // Call the callback function if one has been set.
    if (reachedAngle) reachedAngle(servoNumber, currentAngle, direction);
  }

  return;
}

void ServoEasing::updatePWM(uint8_t servoAngle) {
  // Convert servoAngle to duty cycle (ticks).
  uint16_t dutyCycle = map(servoAngle, MIN_ANGLE, MAX_ANGLE, MIN_TICKS, MAX_TICKS);
  Serial.printf("\ndutyCycle = %d", dutyCycle);

  // Write the new duty cycle to the PWM controller.
  ledcWrite(this->servoNumber, dutyCycle);
}
