#include "pan_servo.h"

// Note:
//
// This library is using default min/max of 1000us and 2000us
// different servos may require different min/max settings
// for an accurate 0 to 180 sweep

// Define needed variables here as defining it in header file would make it global
Servo PanServo;

float pos = 90.0;              // initialise the initial position variable, not pin
const int PAN_SERVO_PIN = 13;  // indicate the pin on ESP32

void setup_pan_servo() {
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    PanServo.setPeriodHertz(50);                // standard 50 hz servo
    PanServo.attach(PAN_SERVO_PIN, 500, 2400);  // attaches the servo on pin 13 to the servo object

    PanServo.write(pos);
}

void actuate_pan_servo(float degree) {
    // Updates the current position of the servo before adding degree.
    // This is so that this function just implement the change in degree.
    pos = PanServo.read();
    pos += degree;

    // Send signal to motor through PWM
    // This function will clip the degrees to 0 - 180
    PanServo.write(static_cast<int>(pos + 0.5));
}

int get_pan_servo_degrees() {
    return PanServo.read();
}
