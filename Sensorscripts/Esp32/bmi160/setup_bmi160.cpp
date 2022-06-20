#include "setup_bmi160.h"

DFRobot_BMI160 bmi160;
const int8_t i2c_addr = 0x69;

int setup_bmi160() {
    // Init the hardware bmi160
    if (bmi160.softReset() != BMI160_OK) {
        Serial.println("reset false");
        return -1;
    }

    // Set and init the bmi160 i2c address
    // Can change this function such that it will change the config of the sensor....
    // Currently default is +/-2g range (can measure max 2g)
    if (bmi160.I2cInit(i2c_addr) != BMI160_OK) {
        Serial.println("init false");
        return -1;
    }

    return 0;
}

IMUReading sample_bmi160() {
    int16_t accelGyro[6] = {0};
    uint32_t timestamp[2] = {0};
    IMUReading output;

    // Get both accel and gyro data from bmi160
    // Parameter accelGyro and timestamp is the pointer to store the data
    int rslt = bmi160.getAccelGyroData(accelGyro, timestamp);
    if (rslt == 0) {
        for (int i = 0; i < 6; i++) {
            if (i < 3) {
                // The first three are gyro datas
                // Angular velocity is in degrees [-180, 180], converted to rad
                output.gyro[i] = (accelGyro[i] * 3.14159) / 180.0;
            } else {
                // The following three data are accel datas
                // In g after normalising by 2 ** 14
                // * 9.80665 to convert to acceleration
                output.accel[i - 3] = (accelGyro[i] / 16384.0) * 9.80665;
            }
        }
        // timestamp[0] should be the same as timestamp[1], and therefore we only calc one.
        output.timestamp = (timestamp[0] * 39) / 1000000.0;

    } else {
        Serial.println("sample_bmi160: error - unable to retrieve IMU reading");
    }
    return output;
}

String IMUReading_to_string(const IMUReading &imu_reading) {
    String output;

    for (int i = 0; i < 3; i++) {
        output += String(imu_reading.gyro[i]) + "\t";
    }
    for (int i = 0; i < 3; i++) {
        output += String(imu_reading.accel[i]) + "\t";
    }
    output += String(imu_reading.timestamp);

    return output;
}

int to_byte_array(const IMUReading &data, byte *byte_arr) {
    memcpy(byte_arr, &data, sizeof(data));
    return (sizeof(data));
}
