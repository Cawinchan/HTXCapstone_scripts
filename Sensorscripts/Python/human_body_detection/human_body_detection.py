import platform
import numpy as np
import cv2
import serial
import sys
import os
import torch
import matplotlib.pyplot as plt
import torchvision.transforms as T
from PIL import Image
sys.path.append("..")

# For windows systems
import pathlib
temp = pathlib.PosixPath
if platform.system == 'Windows': 
    pathlib.PosixPath = pathlib.WindowsPath

# colors for visualization
COLORS = [[0.000, 0.447, 0.741], [0.850, 0.325, 0.098], [0.929, 0.694, 0.125],
          [0.494, 0.184, 0.556], [0.466, 0.674, 0.188], [0.301, 0.745, 0.933]]
          
# standard PyTorch mean-std input image normalization
transform = T.Compose([
    T.Resize(800),
    T.ToTensor(),
    T.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
])

# for output bounding box post-processing
def box_cxcywh_to_xyxy(x):
    x_c, y_c, w, h = x.unbind(1)
    b = [(x_c - 0.5 * w), (y_c - 0.5 * h),
         (x_c + 0.5 * w), (y_c + 0.5 * h)]
    return torch.stack(b, dim=1)

def rescale_bboxes(out_bbox, size):
    img_w, img_h = size
    b = box_cxcywh_to_xyxy(out_bbox)
    b = b * torch.tensor([img_w, img_h, img_w, img_h], dtype=torch.float32).to('cuda')
    return b

def plot_finetuned_results(pil_img, prob=None, boxes=None, save_dir=False, one_box=True):
    finetuned_classes = ['human']
    plt.figure(figsize=(16,10))
    plt.imshow(pil_img)
    ax = plt.gca()
    colors = COLORS * 100
    if one_box:
      if prob is not None and boxes is not None:
        highest_prob_idx = torch.argmax(prob,axis=1)
        boxes = boxes[highest_prob_idx]
        prob = prob[highest_prob_idx]
        for p, (xmin, ymin, xmax, ymax), c in zip(prob, boxes.tolist(), colors):
            ax.add_patch(plt.Rectangle((xmin, ymin), xmax - xmin, ymax - ymin,
                                      fill=False, color=c, linewidth=3))
            cl = p.argmax()
            text = f'{finetuned_classes[cl]}: {p[cl]:0.2f}'
            ax.text(xmin, ymin, text, fontsize=15,
                    bbox=dict(facecolor='yellow', alpha=0.5))
    else:
      if prob is not None and boxes is not None:
        for p, (xmin, ymin, xmax, ymax), c in zip(prob, boxes.tolist(), colors):
            ax.add_patch(plt.Rectangle((xmin, ymin), xmax - xmin, ymax - ymin,
                                      fill=False, color=c, linewidth=3))
            cl = p.argmax()
            text = f'{finetuned_classes[cl]}: {p[cl]:0.2f}'
            ax.text(xmin, ymin, text, fontsize=15,
                    bbox=dict(facecolor='yellow', alpha=0.5))
    plt.axis('off')
    if save_dir:
      plt.savefig(save_dir)
    plt.show()

def filter_bboxes_from_outputs(pil_image, outputs,
                               threshold=0.7):
  
  # keep only predictions with confidence above threshold
  probas = outputs['pred_logits'].softmax(-1)[0, :, :-1]
  keep = probas.max(-1).values > threshold

  probas_to_keep = probas[keep]

  # convert boxes from [0; 1] to image scales
  bboxes_scaled = rescale_bboxes(outputs['pred_boxes'][0, keep], pil_image.size)
  
  return probas_to_keep, bboxes_scaled

def run_workflow(my_image, my_model, save_dir=False):
  # mean-std normalize the input image (batch-size: 1)
  img = transform(my_image).unsqueeze(0)
  img = img.to('cuda')
  # propagate through the model
  # DETR-RESNET101 takes 0.12s/120ms
  outputs = my_model(img)

  for threshold in [0.9, 0.7]:
    
    probas_to_keep, bboxes_scaled = filter_bboxes_from_outputs(my_image,outputs,
                                                              threshold=threshold)

    plot_finetuned_results(my_image,
                           probas_to_keep, 
                           bboxes_scaled,
                           save_dir=save_dir)
  if len(probas_to_keep) > 1:
    return torch.max(probas_to_keep), bboxes_scaled[torch.argmax(probas_to_keep)]
  if len(probas_to_keep) == 1:
    return probas_to_keep, bboxes_scaled
  else:
    return 0, 0

def setup_human_body_detection(): 
    num_classes = 1

    model = torch.hub.load('facebookresearch/detr',
                        'detr_resnet101',
                        pretrained=False,
                        num_classes=num_classes)

    checkpoint = torch.load(pathlib.Path.cwd() / os.path.dirname(os.path.realpath(__file__))/ 'models/detr_40_epoch.pth',
                            map_location='cuda')

    model.load_state_dict(checkpoint['model'],
                        strict=False)
    model.to('cuda')

    model.eval()
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

def preprocessing_human_body_detection(sample,tmax,tmin):
    np.nan_to_num(0)

    # If the difference between the hottest point and the coolest point is below a threshold value
    # make the image blank as it is highly likely to be ambient data
    if (tmax - tmin <= 3):
        np.full((24, 32), 0)

    # For testing purposes
    # sample = td_to_img(sample,tmin,tmax)

    # Image processing to resize for model
    # Colour grayscale image
    img = cv2.applyColorMap(cv2.bitwise_not(np.array(sample).astype(np.uint8)), cv2.COLORMAP_JET)
    # Upscale Image
    img = cv2.resize(img, (640,480), interpolation = cv2.INTER_CUBIC)
    return img
    
def predict_human_body_detection(model,img,tmax=255,tmin=0):
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
    pil_image = Image.fromarray(img)

    # Model prediction

    # TO-DO: Pass prediction box to JK
    human_likelihood_prob, box = run_workflow(pil_image, model)
    return human_likelihood_prob

# if __name__ == "__main__":  
#     model = setup_human_body_detection()
    # predict_human_body_detection(model,[27.66,27.72,26.99,27.47,28.02,28.71,27.82,28.26,28.37,28.42,28.48,29.03,29.04,28.84,28.83,29.47,28.79,28.96,28.79,28.93,28.88,28.85,28.81,28.93,27.99,28.07,28.17,28.51,28.07,28.67,27.46,28.34,27.39,27.75,27.45,27.33,27.61,27.80,28.54,28.83,28.24,28.41,28.58,28.49,29.02,29.00,28.91,29.15,29.05,29.23,28.58,28.79,28.65,28.94,28.45,28.68,28.66,28.42,28.40,28.05,28.73,28.22,28.41,28.31,27.87,27.50,27.21,27.56,27.78,28.09,27.83,28.42,28.92,29.13,28.59,28.77,28.88,29.15,29.14,29.25,29.42,29.19,29.04,28.70,29.56,29.17,28.93,28.83,28.57,28.38,28.16,28.43,28.84,28.57,27.97,28.11,27.57,27.79,27.22,27.12,27.53,27.49,28.48,28.08,28.50,28.62,28.36,28.55,29.29,29.14,29.54,29.23,29.39,29.07,29.35,28.95,29.16,29.15,28.91,29.00,28.85,28.66,28.87,28.74,28.79,28.66,28.56,28.37,27.89,27.86,27.43,27.55,27.37,27.55,27.68,28.21,28.48,28.46,28.18,28.08,28.86,29.28,29.27,29.47,29.38,29.17,29.43,29.27,29.21,29.38,29.01,29.45,29.17,29.14,28.75,29.30,29.02,29.23,28.91,28.41,28.27,27.70,27.53,27.17,27.36,27.44,27.78,28.68,28.57,28.56,27.75,28.15,28.92,29.25,29.48,29.35,29.27,29.15,29.48,29.48,29.18,29.27,29.07,29.31,28.82,28.92,28.94,29.02,28.73,29.05,28.55,28.37,27.60,28.20,27.22,27.37,27.41,27.43,28.44,28.66,28.40,28.02,27.70,28.00,28.93,29.39,31.58,31.97,31.14,30.85,29.92,29.47,29.27,29.33,28.90,29.02,28.90,28.87,28.68,28.94,28.78,29.12,28.55,28.59,27.84,27.89,27.47,26.66,27.09,27.51,28.23,28.26,28.19,27.92,27.95,28.14,28.60,28.92,31.24,32.12,31.09,30.36,29.90,29.28,28.84,29.23,28.96,28.72,28.58,28.75,28.84,28.70,29.16,28.59,28.51,28.48,28.61,28.34,26.99,27.24,27.22,27.55,28.00,28.31,28.03,27.74,27.33,27.62,28.58,28.85,30.11,31.35,30.78,30.71,31.36,30.19,28.88,28.60,28.65,28.85,28.38,28.72,28.47,28.47,28.67,28.65,28.65,28.67,28.57,28.40,27.30,26.96,27.51,27.52,28.17,28.10,27.75,27.65,27.31,27.44,28.32,29.56,31.93,32.16,32.06,32.21,31.81,30.29,29.00,28.89,28.62,28.81,28.60,28.68,28.73,28.31,28.38,28.57,28.60,28.44,28.53,28.34,27.74,27.41,27.28,27.78,27.93,28.04,27.73,27.53,27.46,27.85,28.67,29.50,33.07,32.88,32.78,33.07,31.49,29.92,28.43,28.76,28.31,28.39,28.80,28.39,28.84,28.63,28.93,28.70,28.49,28.50,28.63,28.42,28.03,27.94,27.35,27.64,28.16,27.99,27.60,27.50,27.50,27.51,27.98,28.96,33.24,33.24,33.40,33.23,30.66,29.14,28.24,28.58,28.59,28.68,28.31,28.51,28.40,28.38,28.66,28.97,28.40,28.54,29.10,29.61,28.32,27.82,27.38,28.36,27.62,27.80,27.50,27.42,27.43,27.75,26.98,27.21,31.84,33.37,33.32,33.31,28.40,27.59,27.89,28.08,28.61,28.58,28.53,28.74,28.62,28.13,28.62,28.90,28.51,29.00,29.43,28.96,28.71,27.97,27.53,27.71,27.67,27.66,27.29,27.13,27.53,27.15,27.44,27.16,31.66,33.41,33.42,33.29,28.50,27.09,27.67,27.89,28.81,28.61,28.40,28.94,28.54,28.54,28.62,29.04,29.03,29.05,29.86,29.72,28.71,27.77,27.80,28.08,27.81,27.80,27.45,27.02,27.46,27.97,28.15,28.35,31.92,33.50,33.57,33.55,30.93,29.94,29.49,29.50,29.65,29.42,28.68,28.84,29.11,29.07,29.13,29.12,28.96,29.02,30.18,29.79,28.66,27.79,27.52,27.73,27.76,27.73,27.55,27.28,28.74,29.29,30.09,30.39,32.80,33.48,33.69,33.87,31.63,30.75,31.34,30.90,30.81,30.65,29.23,29.48,28.62,29.38,28.73,28.69,29.23,29.02,29.58,29.69,28.79,28.59,28.12,27.98,27.61,28.03,28.41,28.87,30.88,31.25,31.01,31.22,31.23,31.47,31.23,31.08,30.64,30.63,30.86,31.11,31.53,31.05,31.13,30.60,30.07,29.86,29.65,29.31,29.68,30.14,29.89,29.30,29.54,28.64,27.81,27.32,27.34,27.55,28.69,29.58,30.91,31.14,31.27,31.05,30.77,31.05,30.54,30.76,30.90,30.90,30.59,30.74,31.15,31.01,31.51,31.55,31.49,31.01,30.79,30.69,31.16,30.95,29.98,29.79,29.79,29.66,27.71,27.73,27.65,28.16,29.82,30.52,30.37,30.77,30.88,30.94,31.19,31.26,30.95,31.11,31.12,31.29,30.37,30.37,29.23,29.42,30.68,31.35,32.00,32.06,32.15,31.82,31.67,31.47,29.90,30.14,29.89,29.00,27.41,27.54,27.76,28.57,30.85,31.44,30.33,30.48,30.91,30.92,31.47,31.15,30.45,30.37,30.76,30.74,30.33,30.23,28.69,28.62,28.97,29.57,30.81,31.37,32.32,31.52,31.17,31.65,30.65,30.02,29.49,29.60,28.25,27.81,29.53,30.66,31.65,31.71,29.62,29.32,30.56,30.71,30.72,30.53,30.06,29.88,29.86,30.09,30.34,29.35,27.41,27.26,28.36,28.48,28.86,29.48,31.00,30.89,31.00,31.05,30.61,30.14,29.83,29.94,28.88,29.00,30.32,31.58,31.73,31.12,29.27,29.00,30.33,30.16,30.77,30.49,30.08,30.16,30.24,30.21,29.64,29.28,27.08,27.35,27.95,28.38,27.94,27.96,27.76,28.32,28.81,29.44,30.51,29.94,29.64,29.61,29.18,30.38,31.14,31.55,30.81,29.67,27.89,28.42,29.48,29.55,30.15,29.96,30.32,30.52,30.05,30.01,29.13,28.31,26.90,27.43,28.12,28.27,27.22,27.04,27.33,26.48,27.25,26.95,29.97,29.48,29.77,29.50,29.18,29.65,30.81,30.99,29.96,28.18,27.64,27.69,29.24,29.44,29.93,30.02,30.22,30.41,30.51,30.09,29.15,28.26,27.24,27.68,28.14,28.31,27.54,26.67,27.20,26.70,27.52,26.81],(24,32),26,1)
