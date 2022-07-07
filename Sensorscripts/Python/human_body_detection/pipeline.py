# import fastai library
from fastai.vision.all import *
from collections import Counter
import glob 
import matplotlib.pyplot as plt
import torch
import gc; 

models = ['resnet18', 'resnet34', 'resnet50', 'resnet101', 'resnet152',
        #   'squeezenet1_0', 'squeezenet1_1',
          'densenet121', 'densenet169', 'densenet201', 'densenet161',
           'vgg16_bn', 'vgg19_bn',
        #    'alexnet'
         ]


# Load the data path to images
path = os.getcwd()
thermal_path = os.path.join(path , 'img/thermal/')

# initalise list to store the total number of each class
distribution = []

for i in glob.glob(thermal_path+"/*"):
    if i.split('/')[-1].split('_')[1] == 'nusiance':
        distribution.append('Nusiance')
    else:
        distribution.append('Person')
c = dict(Counter(distribution))
print(c)

# Define the function to return the label
def is_person(x):
    if x.name.split('_')[1] == 'nusiance':
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

for model in models:
    # check if model exists
    if not glob.glob('models/{}_person_classifier_v2.pkl'.format(model)):
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
        learn.export(fname='models/{}_person_classifier_v2.pkl'.format(model))
        print(model,"is done training")