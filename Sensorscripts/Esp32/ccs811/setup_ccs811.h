#include "Adafruit_CCS811.h"

// Create an object of the class Adafruit_CCS811
extern Adafruit_CCS811 ccs;

struct CCSReading {
    uint16_t co2Equivalent;
    uint16_t totalVoc;
};

constexpr int sizeof_CCSReading = sizeof(CCSReading::co2Equivalent) +
                                  sizeof(CCSReading::totalVoc);


// Initialise CCS811
void setup_ccs811(void);

// Sample once.
// This function takes some time.
CCSReading sample_ccs811(void);

// Convert CCSReading to a string for printing.
String CCSReading_to_string(const CCSReading &ccs_reading);

// Convert to byte array for transmission.
// Returns the length of byte array.
// Sequence of bytes are the same as the BMEReading struct.
int to_byte_array(const CCSReading &data, byte *byte_arr);

