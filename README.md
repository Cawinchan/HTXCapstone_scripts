# HTXCapstone Scripts

# Quick overview
Submodule of [HTXCapstone ESP32](https://github.com/LoJunKai/HTXCapstone_esp32) that handles all API's of Sensor and Motors in SLEEK's robotic movement and human detection sensing. 

# How to Run 

## Prerequisites
- ArduinoIDE
- ESP32 (Doit ESP32 Devkit V1)

Each indiviudal script can be opened by Arduino IDE, however the entire project should be accessed through PlatformIO in [HTXCapstone ESP32](https://github.com/LoJunKai/HTXCapstone_esp32). 

# Repository Directiory
```
  ├── Motorscripts        <-- All servo motor scripts
  |   └──Esp32            <-- All ESP32 servo motor scripts
  │        └── examples   <-- Example scripts for the usage of servos with different user inptus
  |             ├── pan_servo_button_input
  |             └── reel_servo_user_input
  |        ├── pan_servo              <-- Source code for the usage of pan servos 
  |        └── reel_servo             <-- Source code for the usage of reeling servos
  |       
  |        
  ├── Sensorscripts       <-- All life detecting sensor scripts
  |   └── Esp32           <-- All ESP32 life detecting sensor scripts
  |        ├── bme680                 <-- Source code for the usage of bme680 (equivalent) CO2 gas sensor
  |        ├── bmi160                 <-- Source code for the usage of bmi680 Inertial Measurement Unit (IMU) sensor
  |        ├── microphone             <-- Source code for the usage of microphone sensor 
  |        └── mlx90640               <-- Source code for the usage of mlx90640 Thermal IR sensor
  |   └── Python          <-- All ESP32 life detecting sensor scripts
  |         ├── human smell detection <-- Script for prediction of human likelihood probability through CO2 (BME680) sensor
  |         ├── human sound detection <-- Script for prediction of human likelihood probability through microphone sensor
  |         ├── human body detection  <-- Script for prediction of human likelihood probability through Thermal IR (MLX90640) sensor
  |         ├── ensemble_model.py     <-- Script for prediction of human presence score through human smell, sound and body detection
  |         └── human_search_protocol.py <-- Script for locating the best direction to face towards potential survivors through expectation maximisation. 
  |
  ├── .gitignore
  ├── README.md
  └── requirements.txt
```

# Script

## [ESP32 Motor API](https://github.com/Cawinchan/HTXCapstone_scripts/tree/main/Motorscripts/Esp32)
### Access the pan servo motor (MG995 180 Servo) and reel servo motor (MG995 360 Servo).

Allows for the control and readings of each of the servo motors.

Code should contain the following functions as API:

* `setup_<motor_name>_servo` - initializes the servo, call in `setup()`.
* `actuate_<motor_name>_servo(float degrees)` - actuates the servo by a specified amount, called in `loop()`.
* `get_<motor_name>_servo_degrees` - returns the degrees the servo is at currently.

`<motor_name>` : pan_servo, reel_servo


## Tip

* Do not use `stdlib`, use Arduino library instead. (better on memory)
* To optimise the use of `String` from Arduino library, use `sprinf` instead. ([tutorial](https://cpp4arduino.com/2020/02/07/how-to-format-strings-without-the-string-class.html)) 

## [ESP32 Sensor API](https://github.com/Cawinchan/HTXCapstone_scripts/tree/main/Sensorscripts/Esp32)
### Access the bme680 (CO2 sensor for Human Smell Detection), bmi160 (IMU sensor for localisation), microphone (Microphone sensor for Human Sound Detection), mlx90640 (Thermal Infrared-Red(IR) Camera for Human Body Detection). 

Allows for the reading of sensor values from their respective sensors. 

Code should contain the following functions as API:

* `setup_<sensor_name>` - initializes the sensor, called in `setup()`.
* `sample_<sensor_name>` - reads once from the sensor, called in `loop()`.
* `<sensor_data>_to_string` - converts it to Arduino `String` for `Serial.print()`.
* `to_byte_array` - converts it to byte array for `Serial.write()`.

`<sensor_name>` : bme680, bmi160, microphone, mlx90640


### Things to note
For reference on how the APIs are implemented, can take a look at:

* `struct`: bme680, bmi160, ccs811
* `array`: mlx90640, microphone

If the sensor uses a struct to keep track of the data received, be sure to do the following:
* Initialize the struct in the header file.
* Calculate the size of individual elements.
  * `constexpr int sizeof_<sensor_name> = sizeof(<individual elements>) + ...`
  * This is because `sizeof(<struct>)` will include unwanted padding.
* In `to_byte_array` be sure manually call `memcpy` for each element, in the **sequence it is initialised** to be consistent. Additionally, do specify the index of the byte array to copy into for `memcpy`, else data will just be overwritten.

## [Python Sensor API](https://github.com/Cawinchan/HTXCapstone_scripts/tree/main/Sensorscripts/Python)
### Access the Human Smell Detection (CO2 Model), Human Sound Detection (Microphone Model), Human Body Detection (Thermal IR Model) 

Allows for the access of human detection models to predict a human likelihood score based on sensor values given. 

Code should contain the following functions as API:

* `setup_<model_name>` - initializes the sensor, called in `setup()`.
* `predict_<sensor_name>` - predict once from sensor values, called in `loop()`.

`<model_name>` : human_smell_detection, human_sound_detection, human_body_detection


### Things to note
Each model detection directory contains a jupyter notebook called `<model_name>_stream.ipynb` that allows for real-time sensor reading and prediction values for testing purposes.
