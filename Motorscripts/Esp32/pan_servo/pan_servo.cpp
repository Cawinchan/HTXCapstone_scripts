#include "pan_servo.h"
#include "esp32-hal-ledc.h"
// Note:
//
// This library is using default min/max of 500us and 2400us
// different servos may require different min/max settings
// for an accurate 0 to 180 sweep

namespace pan {
    // Define needed variables here as defining it in header file would make it global
    Servo PanServo;
    float pos = 90.0;                // initialise the initial position variable, not pin
    const int PAN_SERVO_PIN = 5;     // indicate the pin on ESP32
    const int PAN_SERVO_CHANNEL = 2; // indicate the channel 
    const int TIME_FREQ = 50; 
    const int TIMER_WIDTH = 16;
    const int COUNT_LOW = 1638;
    const int COUNT_HIGH = 7864;
}

void setup_pan_servo() {
//  We set the pan_servo to be on channel 2 as it is in channel 1 by default to ensure it does not interface with the 360 servo
    ledcSetup(pan::PAN_SERVO_CHANNEL, pan::TIME_FREQ, pan::TIMER_WIDTH); // channel 2, 50 Hz, 16-bit width
    ledcAttachPin(pan::PAN_SERVO_PIN, pan::PAN_SERVO_CHANNEL);   // GPIO 5 assigned to channel 2 
}

void set_servo_pos(float degree)
{
    // convert 0-180 degrees to 1638-7864
    uint32_t duty = ((degree / 180.0) * (pan::COUNT_HIGH-pan::COUNT_LOW)) + pan::COUNT_LOW;

    // set channel to pos
    ledcWrite(pan::PAN_SERVO_CHANNEL,duty);
}

void actuate_pan_servo(float degree) {
    // Updates the current position of the servo before adding degree.
    // This is so that this function just implement the change in degree.
    pan::pos = pan::PanServo.read();
    pan::pos += degree;

    // Send signal to motor through PWM
    // This function will clip the degrees to 0 - 180
    set_servo_pos(2,static_cast<int>(pan::pos + 0.5));
}

int get_pan_servo_degrees() {
    return pan::PanServo.read();
}
