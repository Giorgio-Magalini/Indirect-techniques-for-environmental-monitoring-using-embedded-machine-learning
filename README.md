Three-year thesis project on: Indirect techniques for environmental monitoring using embedded machine learning

Author: Giorgio Magalini

Università degli Studi di Padova, Ingegneria dell'Informazione.


## Before starting
Attention!

The rerequirements.txt is not 100% tested, there may be some compatibility problem.

```
pip install -r rerequirements.txt
```

## **Recording Examples folder**
This folder contains a series of recordings in wav format to let you check recording quality and check some pm samples stored in file name as: 
"yyyy-mm-dd HH:MM:SS; pm2.5, pm10.wav"



## Scripts Usage

> **Warning**
> Follow the instructions in all the scripts!

#### HPMA115S0 Sensor
 *MyHPMA_Serial_Sender.ino* is supposed to be uploaded onto an Arduino Uno correctly connected to the HPMA115S0 sensor.
 > **Warning**
 > HPMA115S0 sensor needs a logic levels converter from 5V to 3.3V to establish e serial communication with Arduino Uno!

#### PDM Microphone
 *PDM_Serial_Sender.ino* is supposed to be uploaded onto an Arduino Nano 33 BLE Sense.

#### Audio and Particulate recordings
 Recordings are made and saved with the correct timestamp and pm values by executing *Serial_ArduinoNANO_ArduinoUNO_com_V1.py* 
 with Arduino Nano & Arduino Uno connected to your computer with USB cables.

#### Upload

The upload to EdgeImpulse is made by executing *UploadToEI.py*.