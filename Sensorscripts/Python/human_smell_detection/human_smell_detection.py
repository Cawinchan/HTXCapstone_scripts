# Python script for ROS interface using Serial communication for CO2

# Imports 
from email.mime import base
import numpy as np
import serial

MAX_IAQ = 250
IAQ_base = 50 # Inital IAQ_base value 

class HumanSmellDetector():
    def __init__(self,max_iaq,base_iaq):
        self.max_iaq = max_iaq
        self.base_iaq = base_iaq

    def predict(self,iaq):
        # Returns normalisation of iaq over range of max_iaq and base_iaq (0-100%)
        return (iaq / (self.max_iaq-self.base_iaq))
    
    # Sets new base value when threshold is met to ensure that when the environment changes,
    # the base_IAQ is calibrated correctly
    def set_base(self, new_base_iaq):
        self.base_iaq = new_base_iaq


def setup_human_sound_detection():
    model = HumanSmellDetector(MAX_IAQ,IAQ_base)
    return model

def predict_human_smell_detection(model):
    ser = serial.Serial('COM4', 115200)

    sample = ser.readline()
    sample = sample.decode('utf-8') # decode from byte to string
    sample = sample.split(',')

    arr = np.array(sample)
    iaq = float(arr[1].strip())
    iaq_accuracy = int(arr[2].strip())
    iaq_roc = float(arr[-1].replace('\r\n',''))
    human_likelihood_prob = 0
    if iaq_accuracy < 1 and printer:
        print("IAQ Warming up")
        printer = False
    if iaq_accuracy > 0 and not printer:
        print("IAQ Done Warning up")
        printer = True
    # Signal change: Update IAQ_base value
    if iaq_roc > 32 and iaq_accuracy > 0:
        model.set_base(iaq)
    human_likelihood_prob = model.predict(iaq)
    if human_likelihood_prob > 1:
        human_likelihood_prob = 1
    if human_likelihood_prob < 0:
        human_likelihood_prob = 0
    return human_likelihood_prob
    