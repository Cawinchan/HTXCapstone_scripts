# Python script for ROS interface using Serial communication for CO2

# Imports 
import numpy as np
import serial


# Model setup
def human_breath_detector(iaq,max_iaq,base_iaq):
    return (iaq / (max_iaq-base_iaq))

def predict(model):
    MAX_IAQ = 250
    IAQ_base = 50
    
    ser = serial.Serial('COM4', 115200)

    sample = ser.readline()
    sample = sample.decode('utf-8') # decode from byte to string
    sample = sample.split(',')

    arr = np.array(sample)
    IAQ = float(arr[1].strip())
    IAQ_accuracy = int(arr[2].strip())
    IAQ_roc = float(arr[-1].replace('\r\n',''))
    probability = 0
    if IAQ_accuracy < 1 and printer:
        print("IAQ Warming up")
        printer = False
    if IAQ_accuracy > 0 and not printer:
        print("IAQ Done Warning up")
        printer = True
    # Signal change: Update IAQ_base value
    if IAQ_roc > 32 and IAQ_accuracy > 0:
        IAQ_base = IAQ
    probability = (IAQ / (MAX_IAQ-IAQ_base))
    if probability > 1:
        probability = 1
    if probability < 0:
        probability = 0
    return probability
    