# Python Interfacing 

### How to write using Pyserial to trigger the pan servo
```python
import serial
ser = serial.Serial('COM4', 115200)
while True:
    turn = input("Press Enter to stop the recording:")
    if int(turn) == 1:
        ser.write("esp pan 1".encode('utf-8'))
    if int(turn) == -1:
        ser.write("esp pan -1".encode('utf-8'))
```
