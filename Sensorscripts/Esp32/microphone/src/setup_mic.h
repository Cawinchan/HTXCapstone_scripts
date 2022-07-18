#include <Arduino.h>
#include <Sensorscripts/Esp32/microphone/src/ADCSampler.h>
#include <Sensorscripts/Esp32/microphone/src/I2SMEMSSampler.h>
#include <stdio.h>
// For usage of script without ROS
// #include <I2SMEMSSampler.h>
// #include <ADCSampler.h>

#include "config.h"

namespace mic {
    constexpr int reading_len = 1536;

    // Extern to define it as global variables
    extern int16_t mic_reading[reading_len];  // This stores the microphone values
}

// Initialise Microphone
void setup_mic(void);

// Sample once.
// This function takes some time.
int sample_mic(int16_t* mic_reading);

// Convert microphone reading sample to a string for printing.
String mic_reading_to_string(const int16_t* mic_reading, const int size_bytes);

// Convert to byte array for transmission.
// Returns the length of byte array.
int to_byte_array(const int16_t* data, const int size_bytes, byte* byte_arr);
