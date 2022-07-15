#include <Arduino.h>
#include <stdio.h>
#include <Sensorscripts/Esp32/microphone/src/I2SMEMSSampler.h>
#include <Sensorscripts/Esp32/microphone/src/ADCSampler.h>
// For usage of script without ROS
// #include <I2SMEMSSampler.h>
// #include <ADCSampler.h>

#include "config.h"


int NUM_OF_SAMPLES = 1536;

void record(I2SSampler *input, const char *fname)
{
  int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * NUM_OF_SAMPLES);
  // Start recording
  input->start();
  while (true)
  {
    int samples_read = input->read(samples, NUM_OF_SAMPLES);
    for (int i=0 ;i < NUM_OF_SAMPLES; i++){ 
      Serial.print(samples[i]);
      if (i != NUM_OF_SAMPLES-1){
        Serial.print(",");
      }
    }
    Serial.print("\n");
  }
  // stop the input
  input->stop();
  // and finish the writing
  free(samples);
}

void main_task(void *param)
{
  // Starting up

  // Creating microphone
  I2SSampler *input = new ADCSampler(ADC_UNIT_1, ADC1_CHANNEL_7, i2s_adc_config);

  while (true)
  {
    record(input, "/sdcard/test.wav");
  }
}

void setup()
{
  Serial.begin(115200);
  xTaskCreate(main_task, "Main", 4096, NULL, 0, NULL);
}

void loop()
{
}