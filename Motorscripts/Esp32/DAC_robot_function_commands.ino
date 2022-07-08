/*
 * file OutputVoltage.ino
 *
 * @ https://github.com/DFRobot/DFRobot_MCP4725
 *
 * connect MCP4725 I2C interface with your board (please reference board compatibility)
 *
 * Output a constant voltage value and print through the serial port.
 *
 * Copyright   [DFRobot](https://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  2018-1-15
 */
#include "DFRobot_MCP4725.h"
#define  REF_VOLTAGE    5150

DFRobot_MCP4725 DAC;

//For calibration, enter 5000 below and measure the output voltage... change the REF_VOLTAGE above to the measured value
uint16_t OUTPUT_VOLTAGE = 2000;    // Enter default desired DAC output voltage (unit: mV)

String data_packet = ""; //initilaise the data packet var
int desired_voltage = 2000; // Enter default desired DAC output voltage (unit: mV)

int sensorPin = A0;    // select the input pin for the regulator
int sensorValue = 0;  // variable to store the value coming from the sensor
int voltageRead = 0;  //inititate variable to read voltage from command line

void setup() {       //setup - runs once at the very beginning of your program and then never again

  Serial.begin(115200);
  /* MCP4725A0_address is 0x60 or 0x61
   * MCP4725A0_IIC_Address0 -->0x60
   * MCP4725A0_IIC_Address1 -->0x61
   */
  DAC.init(MCP4725A0_IIC_Address0, REF_VOLTAGE);

}

//unreeling - increase to 1000mV voltage 
//panning - 0000mV
//homming - 3000mV

void loop()      //loops repeatedly from start to end
{ 

    while (!Serial.available());
    data_packet = Serial.readStringUntil('\n');  //read data packet
    Serial.print("Current Input command: "); 
    Serial.println(data_packet); //using println begins next message from next line

    if (data_packet == "P") //Panning does not require added pressure
    {
      OUTPUT_VOLTAGE = 0000; //in mV
    }

    else if (data_packet == "UR") //Unreeling requires added low pressure to grow the tube
    {
      OUTPUT_VOLTAGE = 1000; //in mV
    }

    else if (data_packet == "H") //Homing requires added higher pressure to push SRM
    {
      OUTPUT_VOLTAGE = 3000; //in mV
    }

    else if (data_packet == "R") //retraction requires pressure to be 0 
    {
      OUTPUT_VOLTAGE = 0000; //in mV
    }
    
    //assign output voltage to the DAC board
    DAC.outputVoltage(OUTPUT_VOLTAGE); 

  //not necessarily needed --> reads output from regulator
    //read ITV monitor output
    sensorValue = analogRead(sensorPin);  
    voltageRead = sensorValue*5/1023;      //conversion

    //print current state indicators
    Serial.print("Output voltage signal: ");
    Serial.print(OUTPUT_VOLTAGE);
    Serial.println("mV");                  //using println begin next message in next line
   
    Serial.print("ITV monitor output read: ");
    Serial.println(voltageRead);          //using println begin next message ("") in next line
    Serial.println("");                   //using println begin next message in next line i.e. this line is blank and next line will be next message printed
    delay(500);

}
