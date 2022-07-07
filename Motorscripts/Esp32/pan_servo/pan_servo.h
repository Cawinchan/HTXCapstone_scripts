#include <Arduino.h>

// Returns the pin OUTPUT to servo
int setup_pan_servo();

// Actuate PanServo by degree.
// Negative pan left, positive pan right.
// Minimum interval of calling this function is 15ms to wait for the servo to reach the position
// Note that the final position will be rounded off to int.
// Note that the final position might not be the current pos + degree,
//     as servo might not complete the full turn
// Note that degree needs to be above 1 to see a change in position.
void actuate_pan_servo(float degree);

// Write duty cycle to servo.
// Converts degrees to duty cycle.
void set_pan_servo_pos(float degree);

// Read degrees.
// Converts duty cycle to degrees.
float get_pan_servo_pos();

// Clip pan::pos to within [0, 180]
void clip_pan_servo_degrees();
