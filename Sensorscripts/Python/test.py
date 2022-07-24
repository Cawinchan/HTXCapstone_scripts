import unittest

from human_body_detection.human_body_detection import preprocessing_human_body_detection, setup_human_body_detection, predict_human_body_detection
from human_smell_detection.human_smell_detection import setup_human_smell_detection, predict_human_smell_detection
from human_sound_detection.human_sound_detection import setup_human_sound_detection, predict_human_sound_detection
from ensemble_model import predict_ensemble_model
from human_search_protocol import  expectation_maximization
from human_body_detection.sample import SAMPLE


class HumanDetectionSetup(unittest.TestCase):
    def test_setup_human_body_detection(self):
        """
        Test human body detection can be loaded
        """

        model = setup_human_body_detection()
        self.assertIsNotNone(model)

    def test_setup_human_smell_detection(self):
        """
        Test human smell detection can be loaded
        """
        model = setup_human_smell_detection()
        self.assertIsNotNone(model)

    def test_setup_human_sound_detection(self):
        """
        Test human sound detection can be loaded
        """
        model = setup_human_sound_detection()
        self.assertIsNotNone(model)

    def test_prediction_human_body_detection(self):
        """
        Test human body detection can be loaded and predicted
        """

        model = setup_human_body_detection()
        self.assertIsNotNone(model)
        img = preprocessing_human_body_detection(SAMPLE,255,0)
        human_prob = predict_human_body_detection(model,img,255,0)
        self.assertTrue(human_prob)

    # def test_prediction_human_smell_detection(self):
    #     """
    #     Test human smell detection can be loaded and predicted
    #     """
    #     model = setup_human_smell_detection()
    #     self.assertIsNotNone(model)

    # def test_prediction_human_sound_detection(self):
    #     """
    #     Test human sound detection can be loaded and predicted
    #     """
    #     model = setup_human_sound_detection()
    #     self.assertIsNotNone(model)


    #  predict_human_smell_detection(model, previous_iaq, current_iaq)



if __name__ == '__main__':
    unittest.main()
