#include <ESP32Servo360.h>

// TODO: Read more about the API - it seems quite useful...

// Returns the pin OUTPUT to servo
int setup_reel_servo();

void actuate_reel_servo(float degree);

float get_reel_servo_degrees();

void stop_reel_servo();