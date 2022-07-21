#include <Arduino.h>
#include <Wire.h>

#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

namespace mlx {
    constexpr int reading_len = 768;  // This length is determined by the sensor
}

struct MLXReading {
    float tmin;
    float tmax;
    uint8_t image[mlx::reading_len];
};

constexpr int sizeof_MLXReading = sizeof(MLXReading::tmin) +
                                  sizeof(MLXReading::tmax) +
                                  sizeof(MLXReading::image);

// Initialise the MLX90640
void setup_MLX90640();

// Sample once.
// Image is then extracted and stored to int8_t mlx90640_reading[768].
int sample_MLX90640(MLXReading &mlx90640_reading);

// Converts the image to a string of ints.
String MLXReading_to_string(const MLXReading &mlx90640_reading);

// Convert to byte array for transmission.
// Returns the length of byte array.
// Sequence of bytes are the same as the BMEReading struct.
int to_byte_array(const MLXReading &data, byte *byte_arr);

// Returns true if the MLX90640 is detected on the I2C bus.
boolean isConnected();
