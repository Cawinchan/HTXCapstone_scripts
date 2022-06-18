// continuous servo
//  need to find exact loacation of starting point
// record direction
// rotate one direction, and rotate the other direction and stop it... depending on quality of servo, there might be a lag

// use ASCII table to map characters on the keyboard to values e.g. L-108, R - 114 (check if this value is received, send one value to the servo)

/*
 *
 *  Demonstration of Controlling Continous Servo (360 servo)
 *  this code allows you to control 360 degree servo by a command from Serial Monitor
 *
 * Modified by Ahmad Shamshiri for Robojax.com
 * on Sunday July 01, 2018 at 11:09 in Ajax, Ontario, Canada
 * Watch video instruction of this video:https://youtu.be/b_xvu6wWafA
 * Get this code from Robojax.com
 *
 Original code by BARRAGAN
 This example code is in the public domain.
 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include "reel_servo.h"

namespace reel {
	ESP32Servo360 myservo;  // create servo object to control a servo
	// twelve servo objects can be created on most boards

	int pos = 0;  // variable to store the servo position
	const int REEL_SERVO_PIN = 14;
}  // namespace reel

int setup_reel_servo() {
    reel::myservo.attach(reel::REEL_SERVO_PIN, 16);  // Control pin, signal/feedback pin but our motor does not have this (put random pin)
    return reel::REEL_SERVO_PIN;
}

void actuate_reel_servo(float degree) {
    // Rotate CW - unreel/move forward
    // Rotate CCW - retract/move backward
    // Define rounds per minute... but writing 40 == 6 rpm
    if (degree > 0) {
        reel::myservo.spin(140);  // was 40
    } else if (degree < 0) {
        reel::myservo.spin(-140);  // was 140
    } else {
        reel::myservo.spin(degree);
    }
}

float get_reel_servo_degrees() {
    return reel::myservo.getAngle();
}

void stop_reel_servo() {
    reel::myservo.spin(0);
    reel::myservo.stop();
}
