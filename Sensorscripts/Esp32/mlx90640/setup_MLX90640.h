#include <Arduino.h>
#include <Wire.h>

#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

const byte MLX90640_address = 0x33;  // Default 7-bit unshifted address of the MLX90640

#define TA_SHIFT 8  // Default shift for MLX90640 in open air

// Extern to define it as global variables
extern float mlx90640To[768];
extern paramsMLX90640 mlx90640;

// Returns true if the MLX90640 is detected on the I2C bus.
boolean isConnected();

// Converts the image to a string of floats.
String img_to_string(const float* mlx90640To, const int size_bytes);

// Convert to byte array for transmission.
// Returns the length of byte array.
int to_byte_array(const float* data, int arr_len, byte* byte_arr);

// Initialise the MLX90640
void setup_MLX90640();

// Sample once.
// Image is then extracted and stored to float mlx90640To[768].
void sample_MLX90640(float* mlx90640To);
