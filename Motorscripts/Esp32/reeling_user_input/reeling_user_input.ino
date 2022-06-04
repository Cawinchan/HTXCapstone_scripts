//continuous servo
// need to find exact loacation of starting point
//record direction 
//rotate one direction, and rotate the other direction and stop it... depending on quality of servo, there might be a lag

//use ASCII table to map characters on the keyboard to values e.g. L-108, R - 114 (check if this value is received, send one value to the servo)

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

#include <ESP32Servo360.h>

ESP32Servo360 myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int incomingByte = 0;   // for incoming serial data


void setup() {

  myservo.attach(14, 16); // Control pin, signal/feedback pin but our motor does not have this (put random pin)
    
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep
  Serial.begin(115200);
}



void loop() {

        // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("received: ");
                Serial.print (incomingByte);
                if(incomingByte == 76){                        //Press L - unreel (check serial monitor to view received value)
                 Serial.println(" sent 0 Rotaing CW "); 
                 myservo.spin(40);           //define rounds per minute... but writing 40 == 6 rpm
                }else if(incomingByte == 82){                  //Press R - retract
                  Serial.println(" sent 180 Rotaing CCW "); 
                  myservo.spin(-140); 
                }else if(incomingByte == 83){                   //Press S - stop
                  Serial.println(" sent Stopped "); 
                  myservo.spin(0); 
                }else{
                  Serial.println(" moving Random"); 
                  myservo.spin(0); 
                }
                  
                 
        }

  
}
