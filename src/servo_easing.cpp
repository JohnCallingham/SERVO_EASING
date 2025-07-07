#include "servo_easing.h"

#define PWM_FREQ 50 // 50 Hz.
#define PWM_RESOLUTION 12  // Needs 10 bit resolution, 8 bit doesn't work down to 50Hz !!

void ServoEasing::initialise(uint8_t servoNumber, uint8_t servoPin) {
  this->servoNumber = servoNumber;
  this->servoPin = servoPin;
  //servo.attach(this->servoPin);

  ledcSetup(this->servoNumber, PWM_FREQ, PWM_RESOLUTION); // uses servoNumber for the PWM channel.
  ledcAttachPin(this->servoPin, this->servoNumber); // uses servoNumber for the PWM channel.

  // // With a pwm resolution of 10 bits then 100% duty cycle is 1024.
  // // With a pwm resolution of 12 bits then 100% duty cycle is 4096.
  // ledcWrite(this->servoNumber, 4096/4);

  // Start with no PWM pulses.
  ledcWrite(this->servoNumber, 0);
}

void ServoEasing::setInitialAngle(uint8_t initialAngle) {
  this->currentAngle = initialAngle;
  //servo.attach(this->servoPin);
}

void ServoEasing::setTargetAngle(uint8_t targetAngle) {
  this->targetAngle = targetAngle;

  // Start the PWM pulses.
  ledcWrite(this->servoNumber, 500);


//   // Start the servo's PWM pulses.
//   // No PWM pulses until servo is (re)attached !!!
//   //servo.attach(this->servoPin);
//   ledcAttachPin(this->servoPin, this->servoNumber); // uses servoNumber for the PWM channel.
//   uint32_t retVal =  ledcSetup(this->servoNumber, 50, 10); // Needs 10 bit resolution, 8 bit doesn't work down to 50Hz !!
//   Serial.printf("\nretVal = %d", retVal);
// // // Setup function
// // void setup() {
// // // Attach the LED pin to the PWM channel
// // ledcAttachPin(LED_PIN, PWM_CHANNEL);

// // // Configure the PWM channel with frequency and resolution
// // ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
//   ledcWrite(this->servoNumber, 500);
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
  //this->servo.write(currentAngle);
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
    //servo.detach(); // Stop any more PWM pulses. // Stops PWM completely !!!
    ledcWrite(this->servoNumber, 0);
    // Call the callback function if one has been set.
    if (reachedAngle) reachedAngle(servoNumber, currentAngle, direction);
  }

  return;
}
