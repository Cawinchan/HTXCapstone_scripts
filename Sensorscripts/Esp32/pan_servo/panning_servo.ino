#include <ESP32Servo.h>

Servo myservo;  

int pos = 0; 
int servoPin = 13;
int default_degree = 10;
String data_packet = "esp pan 1\n"; //sample input


void setup() {
  // Allow allocation of all timers
//  ESP32PWM::allocateTimer(0);
//  ESP32PWM::allocateTimer(1);
//  ESP32PWM::allocateTimer(2);
//  ESP32PWM::allocateTimer(3);
//  myservo.setPeriodHertz(50);    // standard 50 hz servo
//  myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 18 to the servo object
//  // using default min/max of 1000us and 2000us
//  // different servos may require different min/max settings
//  // for an accurate 0 to 180 sweep
  Serial.begin(9600);

} 

void loop()
{  
    // Implement reading the ESP32 serial 
    if (Serial.available() > 0) {
       data_packet = Serial.read();
    }
    if (data_packet == "esp pan -1\n"){ // pan left 
        myservo.write(pos-default_degree);    // tell servo to move from current position 10 degrees to the left (0-10 = -10 degrees)
        delay(15);                            // waits 15ms for the servo to reach the position
        pos = myservo.read();                 // updates on servo current postion (for debugging check if it has moved to location)
        char strBuf[8];                       // The max number of chars is 8, len("pan -180") == 8
        sprintf(strBuf, "pan %d", pos); 
        Serial.println(strBuf);               // print to serial for movement confirmation (Only after checking if position has moved)
    }
    if (data_packet == "esp pan 1\n"){ // pan right 
        myservo.write(pos+default_degree);    // tell servo to move from current position 10 degrees to the right (0+10 = 10 degrees)
        delay(15);                            // waits 15ms for the servo to reach the position
        pos = myservo.read();                 // updates on servo current postion (for debugging check if it has moved to location)
        char strBuf[8];                       // The max number of chars is 8, len("pan -180") == 8
        sprintf(strBuf, "pan %d", pos); 
        Serial.println(strBuf);               // print to serial for movement confirmation (Only after checking if position has moved)
    }
}
