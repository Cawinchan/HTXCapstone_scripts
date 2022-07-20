# import fastai library
from fastai.vision.all import *
from collections import Counter
import glob 
import matplotlib.pyplot as plt
import torch
import gc; 
import path

model = 'resnet34'
model_name = 'person_classifier_v3'

if __name__ == '__main__':
    # Load the data path to images
    path = os.getcwd()
    thermal_path = os.path.join('C:/Users/cawin/CodeProjects/SUTD/HTXCapstone_script/Sensorscripts/Python/IR/img/complete_thermal_image')
    # initalise list to store the total number of each class
    distribution = []
    for i in glob.glob(os.path.join(thermal_path+"/*")):
        if 'nusiance' in i:
            distribution.append('Nusiance')
        else:
            distribution.append('Person')
    c = dict(Counter(distribution))
    print(c)

    # Define the function to return the label
    def is_person(x):
        if 'nusiance' in x.name:
            return 'Nusiance'
        else:
            return 'Person'
            
    # Define the datablock called persons
    persons = DataBlock(
        blocks = (ImageBlock, CategoryBlock),
        get_items = get_image_files,
        get_y = is_person,
        splitter = RandomSplitter(valid_pct=0.25, seed=42),
        item_tfms = Resize(420),
        batch_tfms = aug_transforms(size = 244, mult=1.5),
    )

    # Define the dataloaders from the persons datablock
    dls = persons.dataloaders(thermal_path,bs=4)

    if not glob.glob('models/{}_{}.pkl'.format(model,model_name)):
        print(model,"training starts")
        # Clear cache
        gc.collect()
        torch.cuda.empty_cache()
        
        # Define the model using cnn_learner
        learn = vision_learner(dls, model, metrics=[accuracy, error_rate])

        # Find the optimal lr using lr_find()
        lr = learn.lr_find()

        learn.fine_tune(10, base_lr=lr.valley, freeze_epochs=3) 

        # Export our trained model in form of pickle file
        learn.save(fname='models/{}_{}.pkl'.format(model,model_name))
        print(model,"is done training")