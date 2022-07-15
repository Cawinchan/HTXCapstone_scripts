## How to use 

Using Platform.io for sensor testing
In src/main.cpp comment out.
```bash
    #include <Sensorscripts/Esp32/microphone/src/I2SMEMSSampler.h>
    #include <Sensorscripts/Esp32/microphone/src/ADCSampler.h>
```
and uncomment.
```bash
    #include <I2SMEMSSampler.h>
    #include <ADCSampler.h>
```