#include <Arduino.h>
#include <stdio.h>
#include <I2SMEMSSampler.h>
#include <ADCSampler.h>
#include <I2SOutput.h>
#include <DACOutput.h>
#include <SDCard.h>
#include "SPIFFS.h"
#include <WAVFileReader.h>
#include <WAVFileWriter.h>
#include "config.h"

int NUM_OF_SAMPLES = 1536;

void record(I2SSampler *input, const char *fname)
{
  int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * NUM_OF_SAMPLES);
  // Start recording
  input->start();
  // open the file on the sdcard
  // FILE *fp = fopen(fname, "wb");
  // create a new wave file writer
  // WAVFileWriter *writer = new WAVFileWriter(fp, input->sample_rate());
  while (true)
  {
    // int64_t start = esp_timer_get_time();
    int samples_read = input->read(samples, NUM_OF_SAMPLES);
    for (int i=0 ;i < NUM_OF_SAMPLES; i++){ 
      Serial.print(samples[i]);
      if (i != NUM_OF_SAMPLES-1){
        Serial.print(",");
      }
    }
    Serial.print("\n");
    // int64_t end = esp_timer_get_time();
    // ESP_LOGI(TAG, "Wrote %d samples in %lld microseconds", samples_read, end - start);
  }
  // stop the input
  input->stop();
  // and finish the writing
  // fclose(fp);
  // delete writer;
  free(samples);
  // ESP_LOGI(TAG, "Finished recording");
}

void main_task(void *param)
{
  // Starting up

  // ESP_LOGI(TAG, "Mounting SDCard on /sdcard");
  // new SDCard("/sdcard", PIN_NUM_MISO, PIN_NUM_MOSI, PIN_NUM_CLK, PIN_NUM_CS);

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