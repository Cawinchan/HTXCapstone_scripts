#include "setup_MLX90640.h"

float mlx90640To[768];
paramsMLX90640 mlx90640;

boolean isConnected() {
    Wire.beginTransmission((uint8_t)MLX90640_address);
    if (Wire.endTransmission() != 0)
        return (false);  // Sensor did not ACK
    return (true);
}

std::string img_to_string(const float* mlx90640To, const int size_bytes) {
    std::string img_string;

    const int arr_len = size_bytes / sizeof(mlx90640To[0]);

    for (int i = 0; i < arr_len; i++) {
        img_string.append(std::to_string(mlx90640To[i]));
        img_string.append(", ");
    }
    return img_string;
}

void setup_MLX90640() {
    Wire.begin();
    Wire.setClock(400000);  // Increase I2C clock speed to 400kHz

    if (isConnected() == false) {
        Serial.println("MLX90640 not detected at default I2C address. Please check wiring. Freezing.");
        while (1)
            ;
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

    // MLX90640_SetRefreshRate(MLX90640_address, 0x02); //Set rate to 2Hz
    //  MLX90640_SetRefreshRate(MLX90640_address, 0x03); //Set rate to 4Hz
    //    MLX90640_SetRefreshRate(MLX90640_address, 0x04); //Set rate to 8Hz Most optimal speed to accuracy
    MLX90640_SetRefreshRate(MLX90640_address, 0x05);  // Set rate to 16Hz
}

void sample_MLX90640(float* mlx90640To) {
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
    }
}