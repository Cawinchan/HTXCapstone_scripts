#include "setup_bme680.h"

Bsec iaqSensor;
float current_iaq;
float past_iaq;

void setup_bme680(void) {
    iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
    Serial.println("\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix));
    checkIaqSensorStatus();

    bsec_virtual_sensor_t sensorList[10] = {
        BSEC_OUTPUT_RAW_TEMPERATURE,
        BSEC_OUTPUT_RAW_PRESSURE,
        BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_IAQ,
        BSEC_OUTPUT_STATIC_IAQ,
        BSEC_OUTPUT_CO2_EQUIVALENT,
        BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY
    };

    // Note: if the timing of gas measurements of the BME680 is not within 50% of the target values. For example, when running the sensor
    // in low-power mode the intended sample period is 3 s. In this case the difference between two consecutive
    // measurements must not exeed 150% of 3 s which is 4.5s, the IAQ values or equivalent CO2 values will not change.
    iaqSensor.updateSubscription(sensorList, 10, 0.66666f);
    checkIaqSensorStatus();
    past_iaq = 350;
    }

BMEReading sample_bme680(void) {
    if (iaqSensor.run()) {  // If new data is available
        BMEReading output{
            iaqSensor.outputTimestamp,
            iaqSensor.iaq,
            iaqSensor.iaqAccuracy,
            iaqSensor.co2Equivalent,
            iaqSensor.breathVocEquivalent,
        };
        return output;
    } else {
        checkIaqSensorStatus();
    }
}

String BMEReading_to_string(const BMEReading &bme_reading) {
    String output = String((unsigned long) float(bme_reading.timeStamp)/ 1000.0);
    current_iaq = bme_reading.iaq;
    output += ", " + String(current_iaq);                         // IAQ values
    output += ", " + String(bme_reading.iaqAccuracy);             // When IAQ is ready to be used
    output += ", " + String(bme_reading.co2Equivalent);           // Co2Equivalent values
    output += ", " + String(bme_reading.breathVocEquivalent);     // BreathVocEquivalent values
    past_iaq = current_iaq;
    return output;
}

int to_byte_array(const BMEReading &data, byte *byte_arr) {
    int data_in = 0;

    // This lambda function is to increment the data_in pointer,
    // so that it writes into the whole data array.
    // Since C++ does not allow multiple type arguments, we need to cast to char* first.
    auto load_data = [&data, &data_in, &byte_arr] (const char* element, const int size) {
        memcpy(byte_arr + data_in, element, size);
        data_in += size;
    };
    
    load_data((char *) &data.timeStamp, sizeof(data.timeStamp));
    load_data((char *) &data.iaq, sizeof(data.iaq));
    load_data((char *) &data.iaqAccuracy, sizeof(data.iaqAccuracy));
    load_data((char *) &data.co2Equivalent, sizeof(data.co2Equivalent));
    load_data((char *) &data.breathVocEquivalent, sizeof(data.breathVocEquivalent));

    return (sizeof_BMEReading);
}

void checkIaqSensorStatus(void) {
    if (iaqSensor.status != BSEC_OK) {
        if (iaqSensor.status < BSEC_OK) {
            Serial.println("BSEC error code : " + String(iaqSensor.status));
            for (;;)
                errLeds(); /* Halt in case of failure */
        } else {
            Serial.println("BSEC warning code : " + String(iaqSensor.status));
        }
    }

    if (iaqSensor.bme680Status != BME680_OK) {
        if (iaqSensor.bme680Status < BME680_OK) {
            Serial.println("BME680 error code : " + String(iaqSensor.bme680Status));
            for (;;)
                errLeds(); /* Halt in case of failure */
        } else {
            Serial.println("BME680 warning code : " + String(iaqSensor.bme680Status));
        }
    }
}

void errLeds(void) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}
