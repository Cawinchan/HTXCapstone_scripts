#include "setup_bme680.h"

Bsec iaqSensor;

void setup_bme680(void) {
    iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
    Serial.println("\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix));
    checkIaqSensorStatus();

    bsec_virtual_sensor_t sensorList[8] = {
        BSEC_OUTPUT_RAW_TEMPERATURE,
        BSEC_OUTPUT_RAW_PRESSURE,
        BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_IAQ,
        BSEC_OUTPUT_STATIC_IAQ,
        BSEC_OUTPUT_CO2_EQUIVALENT,
        BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    };

    iaqSensor.updateSubscription(sensorList, 8, 1);
    checkIaqSensorStatus();
}

BMEReading sample_bme680(void) {
    if (iaqSensor.run()) {  // If new data is available
        BMEReading output{
            iaqSensor.outputTimestamp,
            iaqSensor.rawTemperature,
            iaqSensor.pressure,
            iaqSensor.rawHumidity,
            iaqSensor.gasResistance,
            iaqSensor.iaq,
            iaqSensor.iaqAccuracy,
            iaqSensor.temperature,
            iaqSensor.humidity,
            iaqSensor.staticIaq,
            iaqSensor.co2Equivalent,
            iaqSensor.breathVocEquivalent,
        };

        return output;
    } else {
        checkIaqSensorStatus();
    }
}

String BMEReading_to_string(const BMEReading &bme_reading) {
    String output = String((unsigned long) bme_reading.timeStamp);
    output += ", " + String(bme_reading.rawTemperature);
    output += ", " + String(bme_reading.pressure);
    output += ", " + String(bme_reading.rawHumidity);
    output += ", " + String(bme_reading.gasResistance);
    output += ", " + String(bme_reading.iaq);
    output += ", " + String(bme_reading.iaqAccuracy);
    output += ", " + String(bme_reading.temperature);
    output += ", " + String(bme_reading.humidity);
    output += ", " + String(bme_reading.staticIaq);
    output += ", " + String(bme_reading.co2Equivalent);
    output += ", " + String(bme_reading.breathVocEquivalent);
    return output;
}

int to_byte_array(const BMEReading &data, byte *byte_arr) {
    memcpy(byte_arr, &data, sizeof(data));
    return (sizeof(data));
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