#include "setup_mic.h"

namespace mic {
    // Initialise the reading buffer.
    int16_t mic_reading[reading_len];
    I2SSampler* sampler;
    boolean disconnected = true;
}  // namespace mic

void setup_mic(void) {
    // Starting up

    // Creating Microphone
    mic::sampler = new ADCSampler(ADC_UNIT_1, ADC1_CHANNEL_7, i2s_adc_config);

    // TODO: What if it is disconnected?
}

int sample_mic(int16_t* mic_reading) {
    // One sample takes 12ms.
    // Start recording
    mic::sampler->start();
    mic::sampler->read(mic_reading, mic::reading_len);
    // Stop recording
    mic::sampler->stop();

    // TODO: Return -1 if it fails.

    return 0;
}

String mic_reading_to_string(const int16_t* mic_reading, const int size_bytes) {
    String mic_string;

    const int arr_len = size_bytes / sizeof(mic_reading[0]);

    for (int i = 0; i < arr_len; i++) {
        mic_string += String(mic_reading[i]);
        if (i != arr_len - 1) {
            mic_string += ",";
        }
    }
    return mic_string;
}

int to_byte_array(const int16_t* data, const int size_bytes, byte* byte_arr) {
    memcpy(byte_arr, data, size_bytes);
    return size_bytes;
}
