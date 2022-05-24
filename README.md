# HTXCapstone
> Capstone Project S15 HTX: Sensor subsystem for Urban Search and Rescue. 

Code should contain the following functions as API:

* `setup_<sensor_name>` - initializes the sensor, called in `setup()`.
* `sample_<sensor_name>` - reads once from the sensor, called in `loop()`.
* `<sensor_data>_to_string` - converts it to Arduino `String` for `Serial.print()`.
* `to_byte_array` - converts it to byte array for `Serial.write()`.


## Tip

Do not use `stdlib`, use Arduino library instead. (better on memory)
