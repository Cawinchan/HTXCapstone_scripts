#include <ESP32Servo.h>

// Returns the pin OUTPUT to servo
void setup_pan_servo();

void set_servo_pos();

// Actuate PanServo by degree.
// Negative pan left, positive pan right.
// Minimum interval of calling this function is 15ms to wait for the servo to reach the position
// Note that the final position will be rounded off to int.
// Note that the final position might not be the current pos + degree,
//     as servo might not complete the full turn
// Note that degree needs to be above 1 to see a change in position.
void actuate_pan_servo(int channel, float degree);

int get_pan_servo_degrees();
