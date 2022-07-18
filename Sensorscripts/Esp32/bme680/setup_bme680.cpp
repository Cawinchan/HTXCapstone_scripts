#include "setup_bme680.h"

namespace bme680 {
    // Create an object of the class Bsec
    Bsec iaqSensor;
    boolean disconnected = true;
}

void setup_bme680(void) {
    bme680::iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);

    int rslt = checkIaqSensorStatus();
    if (rslt < 0) {
        bme680::disconnected = true;
        Serial.println("setup_bme680: unable to communicate with sensor - disconnected");
        return ;
    }

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
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY};

    // Note: if the timing of gas measurements of the BME680 is not within 50% of the target values. For example, when running the sensor
    // in low-power mode the intended sample period is 3 s. In this case the difference between two consecutive
    // measurements must not exeed 150% of 3 s which is 4.5s, the IAQ values or equivalent CO2 values will not change.
    // BSEC_SAMPLE_RATE_CONTINUOUS  (1.0f)             1s      !< Sample rate in case of Continuous Mode */
    // BSEC_SAMPLE_RATE_LP          (0.33333f)         3s      !< Sample rate in case of Low Power Mode */
    // BSEC_SAMPLE_RATE_ULP         (0.0033333f)       5s      !< Sample rate in case of Ultra Low Power Mode */

    bme680::iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_CONTINUOUS);
    rslt = checkIaqSensorStatus();
    if (rslt < 0) {
        bme680::disconnected = true;
        Serial.println("setup_bme680: unable to communicate with sensor - update failed");
        return ;
    }

    if (bme680::disconnected) {
        bme680::disconnected = false;
        Serial.println("setup_bme680: BSEC library version " + String(bme680::iaqSensor.version.major) + "." +
                    String(bme680::iaqSensor.version.minor) + "." + String(bme680::iaqSensor.version.major_bugfix) + "." +
                    String(bme680::iaqSensor.version.minor_bugfix));
    }
}

int sample_bme680(BMEReading &reading) {
    if (bme680::disconnected) {
        return -1;
    }
    if (bme680::iaqSensor.run()) {  // If new data is available
        reading.timeStamp = bme680::iaqSensor.outputTimestamp / 1000.0;
        reading.iaq = bme680::iaqSensor.gasResistance;
        reading.iaqAccuracy = bme680::iaqSensor.iaqAccuracy;
        reading.co2Equivalent = bme680::iaqSensor.co2Equivalent;
        reading.breathVocEquivalent = bme680::iaqSensor.breathVocEquivalent;
        return 0;
    } else {
        return -1;
    }
}

String BMEReading_to_string(const BMEReading &bme_reading) {
    String output = String(bme_reading.timeStamp);
    output += ", " + String(1/bme680::iaqSensor.gasResistance);     // IAQ values
    output += ", " + String(bme_reading.iaqAccuracy);             // When IAQ is ready to be used
    output += ", " + String(bme_reading.co2Equivalent);           // Co2Equivalent values
    output += ", " + String(bme_reading.breathVocEquivalent);     // BreathVocEquivalent values
    return output;
}

int to_byte_array(const BMEReading &data, byte *byte_arr)
{
    int data_in = 0;

    // This lambda function is to increment the data_in pointer,
    // so that it writes into the whole data array.
    // Since C++ does not allow multiple type arguments, we need to cast to char* first.
    auto load_data = [&data, &data_in, &byte_arr](const char *element, const int size)
    {
        memcpy(byte_arr + data_in, element, size);
        data_in += size;
    };

    load_data((char *)&data.timeStamp, sizeof(data.timeStamp));
    load_data((char *)&data.iaq, sizeof(data.iaq));
    load_data((char *)&data.iaqAccuracy, sizeof(data.iaqAccuracy));
    load_data((char *)&data.co2Equivalent, sizeof(data.co2Equivalent));
    load_data((char *)&data.breathVocEquivalent, sizeof(data.breathVocEquivalent));

    return (sizeof_BMEReading);
}

int checkIaqSensorStatus(void) {
    if (bme680::iaqSensor.status != BSEC_OK) {
        if (bme680::iaqSensor.status < BSEC_OK) {
            Serial.println("BSEC error code : " + String(bme680::iaqSensor.status));
            return -1;
        } else {
            Serial.println("BSEC warning code : " + String(bme680::iaqSensor.status));
            return 0;
        }
    }

    if (bme680::iaqSensor.bme680Status != BME680_OK) {
        if (bme680::iaqSensor.bme680Status < BME680_OK) {
            Serial.println("BME680 error code : " + String(bme680::iaqSensor.bme680Status));
            return -1;
        } else {
            Serial.println("BME680 warning code : " + String(bme680::iaqSensor.bme680Status));
            return 0;
        }
    }
    return 0;
}
