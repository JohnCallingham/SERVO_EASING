#include "servo_easing.h"

void ServoEasing::initialise(uint8_t servoNumber, uint8_t servoPin) {
  this->servoNumber = servoNumber;
  this->servoPin = servoPin;

  /**
   * Initialsie the PWM pulses.
   * Uses servoNumber for the PWM channel.
   */
  ledcSetup(this->servoNumber, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(this->servoPin, this->servoNumber);

  // Start with no PWM pulses.
  ledcWrite(this->servoNumber, 0);
}

void ServoEasing::setInitialAngle(uint8_t initialAngle) {
  this->currentAngle = initialAngle;
  
  // Move the servo to its initial angle.
  updatePWM(this->servoNumber, initialAngle);
  delay(1000); // Allow some time for servo to move. How much??

  // Stop the PWM pulses.
  ledcWrite(this->servoNumber, 0);
}

void ServoEasing::setTargetAngle(uint8_t targetAngle) {
  this->targetAngle = targetAngle;
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

  // Update the PWM duty cycle.
  updatePWM(this->servoNumber, currentAngle);
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
    // Stop any more PWM pulses.
    ledcWrite(this->servoNumber, 0);

    // Call the callback function if one has been set.
    if (reachedAngle) reachedAngle(servoNumber, currentAngle, direction);
  }

  return;
}

void ServoEasing::updatePWM(uint8_t servoNumber, uint8_t servoAngle) {

  // // Convert currentAngle to microSeconds.
  // uint16_t microSeconds = map(currentAngle, 0, 180, MIN_PULSE_WIDTH,  MAX_PULSE_WIDTH);
  // Serial.printf("\nmicroSeconds = %d", microSeconds);

  // Convert servoAngle to duty cycle.
  uint16_t dutyCycle = map(servoAngle, MIN_ANGLE, MAX_ANGLE, MIN_TICKS, MAX_TICKS);
  Serial.printf("\ndutyCycle = %d", dutyCycle);

  ledcWrite(this->servoNumber, dutyCycle);
}
