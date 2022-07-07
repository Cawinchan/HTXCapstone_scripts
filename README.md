# HTXCapstone

## Sensor API
Code should contain the following functions as API:

* `setup_<sensor_name>` - initializes the sensor, called in `setup()`.
* `sample_<sensor_name>` - reads once from the sensor, called in `loop()`.
* `<sensor_data>_to_string` - converts it to Arduino `String` for `Serial.print()`.
* `to_byte_array` - converts it to byte array for `Serial.write()`.

### Things to note
If the sensor uses a struct to keep track of the data received, be sure to do the following:
* Initialize the struct in the header file.
* Calculate the size of individual elements.
  * `constexpr int sizeof_<sensor_name> = sizeof(<individual elements>) + ...`
  * This is because `sizeof(<struct>)` will include unwanted padding.
* In `to_byte_array` be sure manually call `memcpy` for each element, in the **sequence it is initialised** to be consistent. Additionally, do specify the index of the byte array to copy into for `memcpy`, else data will just be overwritten.

## Motor API
Code should contain the following functions as API:

* `setup_<motor_name>_servo` - initializes the servo, call in `setup()`.
* `actuate_<motor_name>_servo(float degrees)` - actuates the servo by a specified amount, called in `loop()`.
* `get_<motor_name>_servo_degrees` - returns the degrees the servo is at currently.

## Tip

* Do not use `stdlib`, use Arduino library instead. (better on memory)
* To optimise the use of `String` from Arduino library, use `sprinf` instead. ([tutorial](https://cpp4arduino.com/2020/02/07/how-to-format-strings-without-the-string-class.html)) 
