# Microphone Interface 

In this project directory, microphone_stream.ipynb allows for the streaming of microphone data to our pre-trained Silero VAD model. The audio input available are serial data either ESP32 or arduino and microphone data from source.

### Requirements
- Python 3.8.13
- ESP32/Arduino/Device with microphone
- Jupyter Notebook (pip install notebook)
    - Ubuntu
        -  user@localhost:~$ jupyter notebook \<Project DIR>/HTXCapstone_sensor/Sensorscripts/Python/microphone
    - Windows
        - C:\\User\\USER> jupyter notebook \<Project DIR>/HTXCapstone_sensor/Sensorscripts/Python/microphone

# How to install: Ubuntu
```
    # install requirements
    pip install -r requirements_ubuntu.txt 
```

# How to install: Windows
```
    # install requirements
    pip install -r requirements_windows.txt 

    # installing pyaudio
    # Windows requires a precompiled Windows binaries for pyaudio installation
    pip install pipwin 
    pipwin refresh
    pipwin install pyaudio=0.2.11
```