# Python script for ROS interface using Serial communication for CO2

# Imports 
from decimal import DivisionByZero
from email.mime import base
import numpy as np
import serial

MAX_IAQ = 20
IAQ_base = 0 # Inital IAQ_base value 

class HumanSmellDetector():
    def __init__(self,max_iaq,base_iaq):
        self.max_iaq = max_iaq
        self.base_iaq = base_iaq

    def predict(self,iaq):
        # Returns normalisation of iaq over range of max_iaq and base_iaq (0-100%)
        return (iaq -  self.base_iaq/ (self.max_iaq - self.base_iaq))
    
    # Sets new base value when threshold is met to ensure that when the environment changes,
    # the base_IAQ is calibrated correctly
    def set_base(self, new_base_iaq):
        self.base_iaq = new_base_iaq

def setup_human_smell_detection():
    model = HumanSmellDetector(MAX_IAQ,IAQ_base)
    return model

def predict_human_smell_detection_serial(model):
    '''
        For local testing: Takes a human smell model, reads serial port and outputs a human likelihood probability
        :param model: A human smell model
        :return human_likelihood_prob: float

        serial format ("timestamp","IAQ","iaqAccuracy","co2Equivalent","breathVocEquivalent")
    '''
    ser = serial.Serial('COM4', 115200)

    sample = ser.readline()
    sample = sample.decode('utf-8') # decode from byte to string
    sample = sample.split(',')

    arr = np.array(sample)
    iaq = float(arr[1].strip())
    try:
        iaq_roc = ((iaq / previous_IAQ) - 1) * 1000
    except:
        iaq_roc = 0
    human_likelihood_prob = 0
    # Signal change: Update IAQ_base value
    # if iaq_roc > 32:
    #     model.set_base(iaq)
    human_likelihood_prob = model.predict(iaq)
    if human_likelihood_prob > 1:
        human_likelihood_prob = 1
    if human_likelihood_prob < 0:
        human_likelihood_prob = 0
    previous_IAQ = iaq
    return human_likelihood_prob
    
def predict_human_smell_detection(model, previous_iaq, current_iaq):
    '''
        For ROS: Takes a human smell model and iaq (equivalentCO2) readings and outputs a human likelihood probability
        :param model: A human smell model 
        :param previous_iaq: float (previous equivalentCO2 value read)
        :param current_iaq: float (current equivalentCO2 value read)
        :return human_likelihood_prob: float
    '''
    iaq = float(current_iaq)
    iaq_roc = abs(((iaq / previous_iaq) - 1) * 1000)
    human_likelihood_prob = 0
    human_likelihood_prob = model.predict(iaq_roc)
    if human_likelihood_prob > 1:
        human_likelihood_prob = 1
    if human_likelihood_prob < 0:
        human_likelihood_prob = 0
    return human_likelihood_prob