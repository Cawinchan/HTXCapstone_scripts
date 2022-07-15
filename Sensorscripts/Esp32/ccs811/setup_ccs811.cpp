#include "setup_ccs811.h"

Adafruit_CCS811 ccs;

void setup_ccs811(void) {
    while(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    delay(1000);
    };
    ccs.setDriveMode(0x04);
    // Wait for the sensor to be ready
    while(!ccs.available());
    Serial.println("CCS811 sensor ready!");

}

CCSReading sample_ccs811(void) {
    if(ccs.available()){
        if(!ccs.readData()){
        CCSReading output{
            ccs.geteCO2(),
            ccs.getTVOC()
        };
        return output;
        }
    }
    else{
      Serial.println("ERROR!");
      while(1);
  }
}

String CCSReading_to_string(const CCSReading &ccs_reading) {
    String output = String(ccs_reading.co2Equivalent);  // Co2Equivalent values
    output += ", " + String(ccs_reading.totalVoc);     // totalVoc values
    return output;
}

int to_byte_array(const CCSReading &data, byte *byte_arr) {
    int data_in = 0;

    // This lambda function is to increment the data_in pointer,
    // so that it writes into the whole data array.
    // Since C++ does not allow multiple type arguments, we need to cast to char* first.
    auto load_data = [&data, &data_in, &byte_arr] (const char* element, const int size) {
        memcpy(byte_arr + data_in, element, size);
        data_in += size;
    };

    load_data((char *) &data.co2Equivalent, sizeof(data.co2Equivalent));
    load_data((char *) &data.totalVoc, sizeof(data.totalVoc));

    return (sizeof_CCSReading);
}