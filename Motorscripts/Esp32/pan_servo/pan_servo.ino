#include <ESP32Servo.h>

#include "pan_servo.h"

int default_degree = 10;  // a constant defined
// String data_packet = "esp pan 1"; //sample input --> input command in serial moniter
String data_packet = "";  // initilaise the data packet var

void setup() {
    Serial.begin(115200);
    setup_pan_servo();
}

void loop() {
    // Implement reading the ESP32 serial
    while (!Serial.available());
    data_packet = Serial.readStringUntil('\n');
    if (data_packet == "esp pan -1") {  // pan left
        actuate_pan_servo(-1 * default_degree);
    } else if (data_packet == "esp pan 1") {
        actuate_pan_servo(default_degree);
    } else {
        Serial.println("Invalid command, please input 'esp pan {-1, 1}'");
    }
    delay(15);

    int pos = get_pan_servo_degrees();
    char strBuf[8];  // The max number of chars is 8, len("pan -180") == 8
    sprintf(strBuf, "pan %d", pos);
    Serial.println(strBuf);  // print to serial for movement confirmation (Only after checking if position has moved)
}
