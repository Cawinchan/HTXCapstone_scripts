#include "pan_servo.h"

// Note:
//
// This library is using default min/max of 1000us and 2000us
// different servos may require different min/max settings
// for an accurate 0 to 180 sweep

namespace pan {
    // Define needed variables here as defining it in header file would make it global
    Servo PanServo;

    float pos = 90.0;              // initialise the initial position variable, not pin
    const int PAN_SERVO_PIN = 13;  // indicate the pin on ESP32
}

int setup_pan_servo() {
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    pan::PanServo.setPeriodHertz(50);                // standard 50 hz servo
    pan::PanServo.attach(pan::PAN_SERVO_PIN, 500, 2400);  // attaches the servo on pin 13 to the servo object

    pan::PanServo.write(pan::pos);

    return pan::PAN_SERVO_PIN;
}

void actuate_pan_servo(float degree) {
    // Updates the current position of the servo before adding degree.
    // This is so that this function just implement the change in degree.
    pan::pos = pan::PanServo.read();
    pan::pos += degree;

    // Send signal to motor through PWM
    // This function will clip the degrees to 0 - 180
    pan::PanServo.write(static_cast<int>(pan::pos + 0.5));
}

int get_pan_servo_degrees() {
    return pan::PanServo.read();
}
