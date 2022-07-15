#include <Arduino.h>
#include <Wire.h>

#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

namespace mlx {
    constexpr int reading_len = 768;  // This length is determined by the sensor
    // Extern to define it as global variables
    extern uint8_t mlx90640_reading[reading_len];  // This stores the pixel value normalised to 0-255
}

// Returns true if the MLX90640 is detected on the I2C bus.
boolean isConnected();

// Converts the image to a string of ints.
String img_to_string(const uint8_t* mlx90640_reading, const int size_bytes);

// Convert to byte array for transmission.
// Returns the length of byte array.
int to_byte_array(const uint8_t* data, int arr_len, byte* byte_arr);

// Initialise the MLX90640
void setup_MLX90640();

// Sample once.
// Image is then extracted and stored to int8_t mlx90640_reading[768].
int sample_MLX90640(uint8_t* mlx90640_reading);
