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
#include <ros.h>
#include <motor_controller/UserMovementCmd.h>

#include "DFRobot_MCP4725.h"
#define  REF_VOLTAGE    5150

DFRobot_MCP4725 DAC;

//For calibration, enter 5000 below and measure the output voltage... change the REF_VOLTAGE above to the measured value
uint16_t OUTPUT_VOLTAGE = 2000;    // Enter default desired DAC output voltage (unit: mV)

bool ITV_ON = false;

//unreeling - increase to 1000mV voltage 
//panning - 0000mV
//homming - 3000mV

void callback(const motor_controller::UserMovementCmd &movement_msg) {
  if (movement_msg.pan != 0) {
    // Panning does not require added pressure
    OUTPUT_VOLTAGE = 0000; //in mV
  } else if (movement_msg.move > 0) {
    // Unreeling requires added low pressure to grow the tube
    OUTPUT_VOLTAGE = 1000; //in mV
    ITV_ON = true;
  } else if (movement_msg.move < 0) {
    // Retraction requires pressure to be 0 
    OUTPUT_VOLTAGE = 0000; //in mV
  } else if (movement_msg.home_srm) {
    // Homing requires added higher pressure to push SRM
    OUTPUT_VOLTAGE = 3000; //in mV
    ITV_ON = true;
  } else {
    // Do nothing
    return;
  }
  //assign output voltage to the DAC board
  DAC.outputVoltage(OUTPUT_VOLTAGE); 
}

void off_itv() {
  DAC.outputVoltage(0000);
  ITV_ON = false;
}

ros::NodeHandle  nh;

ros::Subscriber<motor_controller::UserMovementCmd> sub("/user_movement", &callback);

void setup() {
  /* MCP4725A0_address is 0x60 or 0x61
   * MCP4725A0_IIC_Address0 -->0x60
   * MCP4725A0_IIC_Address1 -->0x61
   */
  DAC.init(MCP4725A0_IIC_Address0, REF_VOLTAGE);
  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  nh.spinOnce();  // process all callbacks
  delay(50);  // this is arbitrarily set
  off_itv();
}
