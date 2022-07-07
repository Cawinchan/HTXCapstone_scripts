#include "setup_MLX90640.h"

float mlx90640To[mlx_reading_len];  // This stores the pixel value in degree celcius
uint8_t mlx90640_reading[mlx_reading_len];  // This stores the pixel value normalised to 0-255
paramsMLX90640 mlx90640;

void setup_MLX90640() {
    Wire.begin();
    Wire.setClock(400000);  // Increase I2C clock speed to 400kHz

    if (isConnected() == false) {
        Serial.println("MLX90640 not detected at default I2C address. Please check wiring.");
    }

    // Get device parameters - We only have to do this once
    int status;
    uint16_t eeMLX90640[832];
    status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
    if (status != 0)
        Serial.println("Failed to load system parameters");

    status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
    if (status != 0)
        Serial.println("Parameter extraction failed");

    // Once params are extracted, we can release eeMLX90640 array

    // MLX90640_SetRefreshRate(MLX90640_address, 0x02);  // Set rate to 2Hz
    // MLX90640_SetRefreshRate(MLX90640_address, 0x03);  // Set rate to 4Hz
    MLX90640_SetRefreshRate(MLX90640_address, 0x04);  // Set rate to 8Hz Most optimal speed to accuracy
    // MLX90640_SetRefreshRate(MLX90640_address, 0x05);  // Set rate to 16Hz
}

void sample_MLX90640(uint8_t* mlx90640_reading) {
    for (byte x = 0; x < 2; x++) {
        uint16_t mlx90640Frame[834];
        int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);

        if (status < 0) {
            // An error has occured retrieving data
            Serial.println("error - MLX90640_API::MLX90640_GetFrameData() failed to get data");
        }

        float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
        float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

        float tr = Ta - TA_SHIFT;  // Reflected temperature based on the sensor ambient temperature
        float emissivity = 0.95;

        MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);

        // Normalise float to uint8_t
        // This is to safe data transmission bandwidth.
        // This is acceptable since the image will eventually be normalised to 0-255 range
        // in the processing of life detection.
        // Was previously degrees celcius, so we are losing some information here.
        auto normalise_255 = [&mlx90640_reading]() {
            // Find max and min
            float max_temp;
            float min_temp;
            for (auto i : mlx90640To) {
                if (max_temp < i) {
                    max_temp = i;
                }
                if (min_temp > i) {
                    min_temp = i;
                }
            }
            float temp_range = max_temp - min_temp;

            for (int i = 0; i < mlx_reading_len; i++) {
                // normalized_temp range = [0,1]
                float normalized_temp = (mlx90640To[i] - min_temp) / temp_range;
                // Round off temp by int(x + 0.5)
                mlx90640_reading[i] = int(normalized_temp * 255.0 + 0.5);
            }
        };
        normalise_255();
    }
}

String img_to_string(const uint8_t* mlx90640_reading, const int size_bytes) {
    String img_string;

    const int arr_len = size_bytes / sizeof(mlx90640_reading[0]);

    for (int i = 0; i < arr_len; i++) {
        img_string += String(mlx90640_reading[i]);
        if (i != arr_len - 1) {
            img_string += ",";
        }
    }
    return img_string;
}

int to_byte_array(const uint8_t* data, const int size_bytes, byte* byte_arr) {
    memcpy(byte_arr, data, size_bytes);
    return size_bytes;
}

boolean isConnected() {
    Wire.beginTransmission((uint8_t)MLX90640_address);
    if (Wire.endTransmission() != 0)
        return (false);  // Sensor did not ACK
    return (true);
}
