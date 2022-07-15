import numpy as np

def expectation_maximization(arr,maximization_value='ensemble'):
    """
    Takes an array of (ensemble_prediction,human_smell_prediction,human_sound_prediction,human_body_prediction,angle)
    and outputs the best angle of travel 

    :param arr: list
    :param maximization_value: str
    :return best_angle: float/int
    """
    assert isinstance(arr,list), "arr in Expectation Maximization expects a list"
    assert isinstance(maximization_value,str), "maximization_value in Expectation Maximization must be a string"

    if maximization_value == 'ensemble':
        ensemble_prediction = [i[0] for i in arr]
        best_angle_index = np.argmax(ensemble_prediction,axis=0)
    if maximization_value == 'all':
        ensemble_prediction = [i[0] for i in arr]
        human_smell_prediction = [i[1] for i in arr]
        human_sound_prediction = [i[2] for i in arr]
        human_body_prediction = [i[3] for i in arr]
        best_angle_index = np.argmax(ensemble_prediction,axis=0)
    best_angle = arr[best_angle_index][4]
    return best_angle