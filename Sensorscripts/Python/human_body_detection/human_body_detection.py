import numpy as np
import cv2
import serial
from fastai.vision.all import load_learner

# For windows systems
import pathlib
temp = pathlib.PosixPath
pathlib.PosixPath = pathlib.WindowsPath

def td_to_img(f,tmax,tmin):
    norm = np.uint8((f - tmin)*255/(tmax-tmin))
    return norm

def setup_human_body_detection():
    model = load_learner('models/resnet34_person_classifier_2022.1.pkl')
    return model

def predict_human_body_detection_serial(model):
    ser = serial.Serial('COM4', 115200)
    mlx_shape = (24,32)

    # Read line
    sample = ser.readline()
    sample = sample.decode('utf-8') # decode from byte to string
    sample = sample.split(',')
    sample = np.array(sample).astype(np.float)

    # Find the min and max values of image
    tmax = sample.max()
    tmin = sample.min()
    np.nan_to_num(0)
    sample = np.reshape(sample,mlx_shape)

    # If the difference between the hottest point and the coolest point is below a threshold value
    # make the image blank as it is highly likely to be ambient data
    if (tmax - tmin <= 3):
        np.full((24, 32), 0)

    # Normalise image
    tg_img = td_to_img(sample,tmin,tmax)

    # Image processing to resize for model
    # Colour grayscale image
    img = cv2.applyColorMap(tg_img, cv2.COLORMAP_JET)
    # Upscale Image
    img = cv2.resize(img, (640,480), interpolation = cv2.INTER_CUBIC)

    # Model prediction
    with model.no_bar():
        if (tmax - tmin > 3):
            pred_class , _ , human_likelihood_prob = model.predict(img)
        else: 
            human_likelihood_prob = 0
    return human_likelihood_prob

def predict_human_body_detection(model,sample,mlx_shape,tmax,tmin):
    """
    Takes the model and sample given by ROS to do human body prediction. 
    The current shape of the model is mlx_shape, tmax and tmin

    :param model: fastai.vision.all()
    :param sample: list(int16)
    :param mlx_shape: (int,int)
    :param tmax: int
    :param tmin: int
    :return human_likelihood_prob: float
    
    """
    np.nan_to_num(0)
    sample = np.array(sample)
    sample = np.reshape(sample,mlx_shape)

    # If the difference between the hottest point and the coolest point is below a threshold value
    # make the image blank as it is highly likely to be ambient data
    if (tmax - tmin <= 3):
        np.full((24, 32), 0)

    # Image processing to resize for model
    # Colour grayscale image
    img = cv2.applyColorMap(sample, cv2.COLORMAP_JET)
    # Upscale Image
    img = cv2.resize(img, (640,480), interpolation = cv2.INTER_CUBIC)

    # Model prediction
    with model.no_bar():
        if (tmax - tmin > 3):
            pred_class , _ , human_likelihood_prob = model.predict(img)
        else: 
            human_likelihood_prob = 0
    return human_likelihood_prob
        