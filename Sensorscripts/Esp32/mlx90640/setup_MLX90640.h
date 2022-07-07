#include <Arduino.h>
#include <Wire.h>

#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

const byte MLX90640_address = 0x33;  // Default 7-bit unshifted address of the MLX90640

#define TA_SHIFT 8  // Default shift for MLX90640 in open air

constexpr int mlx_reading_len = 768;  // This length is determined by the sensor
// Extern to define it as global variables
extern uint8_t mlx90640_reading[mlx_reading_len];
extern paramsMLX90640 mlx90640;

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
void sample_MLX90640(uint8_t* mlx90640_reading);
