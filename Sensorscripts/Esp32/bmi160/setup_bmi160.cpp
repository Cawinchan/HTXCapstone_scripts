#include "setup_bmi160.h"

namespace imu{
    DFRobot_BMI160 bmi160;
    const int8_t i2c_addr = 0x69;
    boolean disconnected = true;
}

int setup_bmi160() {
    // Init the hardware bmi160
    if (imu::bmi160.softReset() != BMI160_OK) {
        Serial.println("setup_bmi160: unable to reset sensor");
        imu::disconnected = true;
        return -1;
    }

    // Set and init the bmi160 i2c address
    // Can change this function such that it will change the config of the sensor....
    // Currently default is +/-2g range (can measure max 2g)
    if (imu::bmi160.I2cInit(imu::i2c_addr) != BMI160_OK) {
        Serial.println("setup_bmi160: unable to communicate with sensor - disconnected");
        imu::disconnected = true;
        return -1;
    }

    imu::disconnected = false;
    return 0;
}

int sample_bmi160(IMUReading &imu_reading) {
    if (imu::disconnected) {
        return -1;
    }

    int16_t accelGyro[6] = {0};
    uint32_t timestamp[2] = {0};

    // Get both accel and gyro data from bmi160
    // Parameter accelGyro and timestamp is the pointer to store the data
    int rslt = imu::bmi160.getAccelGyroData(accelGyro, timestamp);
    // Possible nonsense data: When the device is not connected, somehow rslt will still be valid
    // timestamp will be 16777215 and every element of accelGyro will be -1.
    // TODO: Solve for such an issue so that nonsense data doesn't get passed on to ROS.

    if (rslt == 0) {
        // timestamp[0] should be the same as timestamp[1], and therefore we only calc one.
        imu_reading.timestamp = (timestamp[0] * 39) / 1000000.0;
        for (int i = 0; i < 6; i++) {
            if (i < 3) {
                // The first three are gyro datas
                // Angular velocity is in degrees [-180, 180], converted to rad
                imu_reading.gyro[i] = (accelGyro[i] * 3.14159) / 180.0;
            } else {
                // The following three data are accel datas
                // In g after normalising by 2 ** 14
                // * 9.80665 to convert to acceleration
                imu_reading.accel[i - 3] = (accelGyro[i] / 16384.0) * 9.80665;
            }
        }

    } else {
        Serial.println("sample_bmi160: error - unable to retrieve IMU reading");
        return -1;
    }
    return 0;
}

String IMUReading_to_string(const IMUReading &imu_reading) {
    String output;

    output += String(imu_reading.timestamp);
    for (int i = 0; i < 3; i++) {
        output += String(imu_reading.accel[i]) + "\t";
    }
    for (int i = 0; i < 3; i++) {
        output += String(imu_reading.gyro[i]) + "\t";
    }

    return output;
}

int to_byte_array(const IMUReading &data, byte *byte_arr) {
    int data_in = 0;
    int num_bytes = 0;

    num_bytes = sizeof(data.timestamp);
    memcpy(byte_arr, &data.timestamp, num_bytes);
    data_in += num_bytes;

    num_bytes = sizeof(data.accel);
    memcpy(byte_arr + data_in, &data.accel, num_bytes);
    data_in += num_bytes;

    memcpy(byte_arr + data_in, &data.gyro, sizeof(data.gyro));
    return (sizeof_IMUReading);
}
