# Imports 
import numpy as np
import torch
torch.set_num_threads(60)
import torchaudio
torchaudio.set_audio_backend("soundfile")
import pyaudio

# Pyaudio setup
FORMAT = pyaudio.paInt16
CHANNELS = 1
SAMPLE_RATE = 16000
CHUNK = int(SAMPLE_RATE / 10)


def setup():
    model, _ = torch.hub.load(repo_or_dir='snakers4/silero-vad',
                                model='silero_vad',
                                force_reload=True)

    return model

    # Provided by Alexander Veysov
def int2float(sound):
    abs_max = np.abs(sound).max()
    sound = sound.astype('float32')
    if abs_max > 0:
        sound *= 1/abs_max
    sound = sound.squeeze()  # depends on the use case
    return sound

def predict(model):
    audio = pyaudio.PyAudio()
    num_samples = 1536
    stream = audio.open(format=FORMAT,
                        channels=CHANNELS,
                        rate=SAMPLE_RATE,
                        input=True,
                        frames_per_buffer=CHUNK)
    audio_chunk = stream.read(num_samples)
    
    audio_int16 = np.frombuffer(audio_chunk, np.int16)

    audio_float32 = int2float(audio_int16)
    
    # get the confidences and add them to the list to plot them later
    new_confidence = model(torch.from_numpy(audio_float32), 16000).item()

    return new_confidence, audio_chunk
    
