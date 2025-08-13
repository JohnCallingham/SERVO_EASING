# SERVO_EASING

The file servo_easing.h contains a single class ServoEasing. This class uses the Arduino LedC library to control a servo using PWM pulses.

Movement of the servo is achieved by setting the target angle to be different to the current angle. A callback function can be set which is called when the servo reaches the target angle. If a mid angle is set then the callback function will also be called when the mid angle is passed.

The update() function will need to be called from the main program loop to update the servo angle.

A crude form of servo easing is achieved by increasing the delay between updates at the start and end of each servo movemnt.

This library has been developed using PlatformIO.
