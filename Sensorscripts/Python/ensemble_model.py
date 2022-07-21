# Permutations of weights for testing 
    # weights = [(0.33,0.33,0.34),  # 1-1-1 - weight split evenly between 3 models
    #             (0.25,0.25,0.5),  # 1-1-2 - weighted average, greater importance towards human_body_model 
    #             (0.16,0.34,0.50), # 1-2-3 - weighted average, greater importance towards human_sound_model and human_body_model 
    #             ]
    # weights = []

# Weightage generator  
    # for i in range(1,4+1):
    #     for j in range(1,4+1):
    #         for k in range(1,4+1):
    #             total_weightage = i+j+k
    #             human_smell_weightage = float(i)/total_weightage
    #             human_sound_weightage = float(j)/total_weightage
    #             human_body_weightage = float(k)/total_weightage
    #             weights.append((human_smell_weightage,human_sound_weightage,human_body_weightage))

WEIGHTS = (0.33,0.33,0.34)

class EnsembleModel():
    def __init__(self,weights):
        
        # :type weights: Tuple(human_smell_weightage,human_sound_weightage,human_body_weightage) 

        self.human_smell_weightage, self.human_sound_weightage, self.human_body_weightage = weights

    def predict(self,human_smell_prob,human_sound_prob,human_body_prob):

        # Weighted Average of human detection probabilities
        return (self.human_smell_weightage * human_smell_prob) + \
               (self.human_sound_weightage * human_sound_prob) + \
               (self.human_body_weightage * human_body_prob) 

    def set_weights(self,weights):
        self.weights = weights

def setup_ensemble_model():
    ensemble_model = EnsembleModel(WEIGHTS)
    return ensemble_model

def predict_ensemble_model(model,human_smell_prob,human_sound_prob,human_body_prob):
    return model.predict(human_smell_prob, human_sound_prob, human_body_prob), human_smell_prob, human_sound_prob, human_body_prob

def change_weights(model,weights):
    model.set_weights(weights)
    

