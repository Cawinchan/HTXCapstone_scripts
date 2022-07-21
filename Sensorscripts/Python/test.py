import unittest

from human_body_detection.human_body_detection import setup_human_body_detection, predict_human_body_detection
from human_smell_detection.human_smell_detection import setup_human_smell_detection, predict_human_smell_detection
from human_sound_detection.human_sound_detection import setup_human_sound_detection, predict_human_sound_detection
from ensemble_model import predict_ensemble_model
from human_search_protocol import  expectation_maximization


class HumanDetectionSetup(unittest.TestCase):
    def testhuman_body_detection_process(self):
        """
        Test human body detection can be loaded and predicted 
        """

        model = setup_human_body_detection()
        self.assertIsNotNone(model)

    def testhuman_smell_detection_process(self):
        """
        Test human smell detection can be loaded and predicted 
        """
        model = setup_human_smell_detection()
        self.assertIsNotNone(model)

    def testhuman_sound_detection_process(self):
        """
        Test human sound detection can be loaded and predicted 
        """
        model = setup_human_sound_detection()
        self.assertIsNotNone(model)

if __name__ == '__main__':
    unittest.main()
