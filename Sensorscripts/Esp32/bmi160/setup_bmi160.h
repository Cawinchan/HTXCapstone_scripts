#include "DFRobot_BMI160.h"

struct IMUReading {
    double timestamp;
    double accel[3];
    double gyro[3];
};

constexpr int sizeof_IMUReading = sizeof(IMUReading::timestamp) +
                                  sizeof(IMUReading::accel) +
                                  sizeof(IMUReading::gyro);

// Returns -1 if failed to set up.
int setup_bmi160();

// This function returns -1 for IMUReading.timestamp if read not successful.
// 200Hz is 5ms per sample
// This sensor is capable up to 1600Hz
// TODO:
// Somehow 5ms interval is not achieving it, it becomes more like 25ms interval instead...
// 40hz instead of 200hz
int sample_bmi160(IMUReading &imu_reading);

// Convert IMUReading to a string for printing.
String IMUReading_to_string(const IMUReading &imu_reading);

// Convert to byte array for transmission.
// Returns the length of byte array.
// Sequence of bytes are the same as the IMUReading struct.
int to_byte_array(const IMUReading &data, byte *byte_arr);
