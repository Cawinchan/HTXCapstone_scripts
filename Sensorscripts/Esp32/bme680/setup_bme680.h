#include "bsec.h"

struct BMEReading {
    double timeStamp;
    float iaq;
    uint8_t iaqAccuracy;
    float co2Equivalent;
    float breathVocEquivalent;
};

constexpr int sizeof_BMEReading = sizeof(BMEReading::timeStamp) +
                                  sizeof(BMEReading::iaq) +
                                  sizeof(BMEReading::iaqAccuracy) +
                                  sizeof(BMEReading::co2Equivalent) +
                                  sizeof(BMEReading::breathVocEquivalent);

// Initialise BME680
void setup_bme680(void);

// Sample once.
// This function takes some time.
int sample_bme680(BMEReading &reading);

// Convert BMEReading to a string for printing.
String BMEReading_to_string(const BMEReading &bme_reading);

// Convert to byte array for transmission.
// Returns the length of byte array.
// Sequence of bytes are the same as the BMEReading struct.
int to_byte_array(const BMEReading &data, byte *byte_arr);

// Helper function definitions
int checkIaqSensorStatus(void);
