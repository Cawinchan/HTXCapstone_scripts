#include <ESP32Servo.h>

Servo myservo;  

int pos = 0; 
int servoPin = 13;
int default_degree = 10;
//String data_packet = "esp pan 1\n"; //sample input
String data_packet = "";

void setup() {
//   Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 13 to the servo object
  
  pos = myservo.read(); //updates the last sent position of the servo
  
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep
  Serial.begin(115200);

} 

void loop()
{  
    // Implement reading the ESP32 serial 
    while (!Serial.available());
    data_packet = Serial.readStringUntil('\n');
    Serial.println(data_packet);
    Serial.println("read");

    if (data_packet == "esp pan -1"){ // pan left 
        pos = pos-default_degree;
        if (pos < 0){
            pos = 0;
            // Can create a signal to reveal over movement
        }
        myservo.write(pos);                   // tell servo to move from current position 10 degrees to the left (0-10 = -10 degrees)
        delay(15);                            // waits 15ms for the servo to reach the position
        char strBuf[8];                       // The max number of chars is 8, len("pan -180") == 8
        sprintf(strBuf, "pan %d", pos); 
        Serial.println(strBuf);               // print to serial for movement confirmation (Only after checking if position has moved)
    }
    if (data_packet == "esp pan 1"){ // pan right 
        pos = pos+default_degree;
        if (pos > 180){ 
            pos = 180;
            // Can create a signal to reveal over movement
        }
        myservo.write(pos);    // tell servo to move from current position 10 degrees to the right (0+10 = 10 degrees)
        delay(15);                            // waits 15ms for the servo to reach the position
        char strBuf[8];                       // The max number of chars is 8, len("pan -180") == 8
        sprintf(strBuf, "pan %d", pos); 
        Serial.println(strBuf);               // print to serial for movement confirmation (Only after checking if position has moved)
    }
}
