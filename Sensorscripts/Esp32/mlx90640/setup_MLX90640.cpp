#include "setup_MLX90640.h"

#define TA_SHIFT 8  // Default shift for MLX90640 in open air

namespace mlx {
    // Local variables
    const byte address = 0x33;  // Default 7-bit unshifted address of the MLX90640
    float degrees_buffer[reading_len];  // This stores the pixel value in degree celcius
    uint8_t mlx90640_reading[reading_len];  // This stores the pixel value normalised to 0-255
    paramsMLX90640 params;
    boolean disconnected = true;
}

void setup_MLX90640() {
    Wire.begin();
    Wire.setClock(400000);  // Increase I2C clock speed to 400kHz

    if (isConnected() == false) {
        Serial.println("setup_MLX90640: unable to communicate with sensor - disconnected");
        mlx::disconnected = true;
        return;
    }

    // Get device parameters - We only have to do this once
    int status;
    uint16_t eeMLX90640[832];
    status = MLX90640_DumpEE(mlx::address, eeMLX90640);
    if (status != 0) {
        Serial.println("setup_MLX90640: failed to load system parameters");
        mlx::disconnected = true;
        return;
    }

    status = MLX90640_ExtractParameters(eeMLX90640, &mlx::params);
    if (status != 0) {
        Serial.println("setup_MLX90640: parameter extraction failed");
        mlx::disconnected = true;
        return;
    }

    // Once params are extracted, we can release eeMLX90640 array

    // MLX90640_SetRefreshRate(mlx::address, 0x02);  // Set rate to 2Hz
    // MLX90640_SetRefreshRate(mlx::address, 0x03);  // Set rate to 4Hz
    MLX90640_SetRefreshRate(mlx::address, 0x04);  // Set rate to 8Hz Most optimal speed to accuracy
    // MLX90640_SetRefreshRate(mlx::address, 0x05);  // Set rate to 16Hz
    mlx::disconnected = false;
}

int sample_MLX90640(MLXReading &mlx90640_reading) {
    if (mlx::disconnected) {
        return -1;
    }
    for (byte x = 0; x < 2; x++) {
        uint16_t mlx90640Frame[834];
        int status = MLX90640_GetFrameData(mlx::address, mlx90640Frame);

        if (status < 0) {
            // An error has occured retrieving data
            Serial.println("error - MLX90640_API::MLX90640_GetFrameData() failed to get data");
            return -1;
        }

        float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx::params);
        float Ta = MLX90640_GetTa(mlx90640Frame, &mlx::params);

        float tr = Ta - TA_SHIFT;  // Reflected temperature based on the sensor ambient temperature
        float emissivity = 0.95;

        MLX90640_CalculateTo(mlx90640Frame, &mlx::params, emissivity, tr, mlx::degrees_buffer);
    }
    // Normalise float to uint8_t
    // This is to safe data transmission bandwidth.
    // This is acceptable since the image will eventually be normalised to 0-255 range
    // in the processing of life detection.
    // Was previously degrees celcius, so we are losing some information here.
    auto normalise_255_get_reading = [&mlx90640_reading]() {
        // Find max and min
        // Initialise the variables to be way higher than possible
        float max_temp = -100.0;
        float min_temp = 100.0;
        for (auto i : mlx::degrees_buffer) {
            if (max_temp < i) {
                max_temp = i;
            }
            if (min_temp > i) {
                min_temp = i;
            }
        }
        const float temp_range = max_temp - min_temp;

        for (int i = 0; i < mlx::reading_len; i++) {
            // normalized_temp range = [0,1]
            float normalized_temp = (mlx::degrees_buffer[i] - min_temp) / temp_range;
            // Round off temp by int(x + 0.5)
            mlx90640_reading.image[i] = int(normalized_temp * 255.0 + 0.5);
        }
        mlx90640_reading.tmin = min_temp;
        mlx90640_reading.tmax = max_temp;
    };
    normalise_255_get_reading();
    return 0;
}

String MLXReading_to_string(const MLXReading &mlx90640_reading) {
    String output = String(mlx90640_reading.tmin);
    output += ", " + String(mlx90640_reading.tmax);
    output += "\n";

    String img_string;

    const int arr_len = sizeof(mlx90640_reading.image) / sizeof(mlx90640_reading.image[0]);

    for (int i = 0; i < arr_len; i++) {
        img_string += String(mlx90640_reading.image[i]);
        if (i != arr_len - 1) {
            img_string += ",";
        }
    }
    return output + img_string;
}

int to_byte_array(const MLXReading &data, byte *byte_arr) {
    // memcpy(byte_arr, data, size_bytes);
    // return size_bytes;

    int data_in = 0;

    // This lambda function is to increment the data_in pointer,
    // so that it writes into the whole data array.
    // Since C++ does not allow multiple type arguments, we need to cast to char* first.
    auto load_data = [&data, &data_in, &byte_arr](const char *element, const int size) {
        memcpy(byte_arr + data_in, element, size);
        data_in += size;
    };

    load_data((char *)&data.tmin, sizeof(data.tmin));
    load_data((char *)&data.tmax, sizeof(data.tmax));
    load_data((char *)&data.image, sizeof(data.image));

    return (sizeof_MLXReading);
}

boolean isConnected() {
    Wire.beginTransmission((uint8_t)mlx::address);
    if (Wire.endTransmission() != 0)
        return (false);  // Sensor did not ACK
    return (true);
}
