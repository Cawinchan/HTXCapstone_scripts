#include "setup_bme680.h"

// Entry point for the example
void setup(void)
{
  Serial.begin(115200);
  while (!Serial) delay(10); // wait for console
  Wire.begin();

  setup_bme680();

  // Print the header
  char* output = "Timestamp [ms], IAQ, IAQ accuracy, CO2 equivalent, breath VOC equivalent, IAQ Rate of Change";

  Serial.println(output);
}

// Function that is looped forever
void loop(void)
{
  BMEReading reading = sample_bme680();
  Serial.println(BMEReading_to_string(reading));
}
