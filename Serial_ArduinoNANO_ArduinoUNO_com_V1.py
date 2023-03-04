import serial
import serial.tools.list_ports
import wave
import struct
import os
import calendar
import time
from datetime import datetime



# VARIABLES DEFINITIONS

sampleRate = 16000 #Hz

fileAudioLength = int(input("\nSet file audio duration (seconds): ")) #seconds

fileAudioAmount = int(input("\nSet number of total files you want to record: ")) #files

fileAudioRecorded = 0 #count file audio recorded

samples = fileAudioLength * sampleRate #total samples in a file

fileNameCounter = 1 #counter for file audio name

dataList = [0] * 2



# SERIAL PORT COMMUNICATION SETTINGS NANO ***************************************************************************************************

ports = serial.tools.list_ports.comports()
nano = serial.Serial()

portsList = []
portVar = ""

for onePort in ports:
    portsList.append(str(onePort))
    print(str(onePort))

val = input("\nSelect Arduino NANO Port: /dev/cu.usbmodem")

for x in range(0,len(portsList)):
    if portsList[x].startswith("/dev/cu.usbmodem" + str(val)):
        portVar = "/dev/cu.usbmodem" + str(val)
        print("\n" + portVar + "\n")

nano.port = portVar

# nano.port = "/dev/cu.usbmodem14131" # SET directly your Arduino NANO port

nano.baudrate = 115200

try:
    nano.open()
except Exception as e:
    print("error open serial port: " + str(e))
    exit()

print("Initializing Arduino Nano...")

time.sleep(1.5)

print("Done")

nano.write(b'2')



# SERIAL PORT COMMUNICATION SETTINGS UNO ***************************************************************************************************

ports = serial.tools.list_ports.comports()
uno = serial.Serial()

portsList = []

portVar = ""

for onePort in ports:
    portsList.append(str(onePort))
    print(str(onePort))

val = input("\nSelect Arduino UNO Port: /dev/cu.usbmodem")

for x in range(0,len(portsList)):
    if portsList[x].startswith("/dev/cu.usbmodem" + str(val)):
        portVar = "/dev/cu.usbmodem" + str(val)
        print("\n" + portVar + "\n")

uno.port = portVar

# uno.port = "/dev/cu.usbmodem14121"

uno.baudrate = 57600
uno.timeout = 5
try:
    uno.open()
except Exception as e:
    print("error open serial port: " + str(e))
    exit()

print("Initializing Arduino Uno...")

time.sleep(20)

print("Done")

audioRecorded = False




def getPmValues(i):  # Read UNO serial port for pm values
    uno.reset_input_buffer()
    arduinoData = 0
    if i == 0:
        uno.write(b'1') # request pm2.5
        uno.reset_input_buffer() 
        arduinoData = uno.readline().decode("utf-8") # decode
    if i == 1:
        uno.write(b'2') # request pm10
        uno.reset_input_buffer()
        arduinoData = uno.readline().decode("utf-8") # decode
    return int(arduinoData)



while fileAudioRecorded < fileAudioAmount: # Cycle up until #fileAudioAmount recordings have made

    wavefile = wave.open("sound.wav",'wb')
    wavefile.setnchannels(1) # 1 = mono channel, 2 = stereo channel
    wavefile.setsampwidth(2) # samplewidth in bytes (16 bits = 2 bytes)
    wavefile.setframerate(sampleRate)

    nano.write(b'2')
    nano.reset_input_buffer()
    nano.write(b'1')
    
    time.sleep(5)

    fileAudioLeft = fileAudioAmount - fileAudioRecorded

    print("\n rec number: " + str(fileAudioRecorded + 1) + "started. " + str(fileAudioLeft - 1) + " left.\n")

    for x in range(samples):
        nano.read_until()
        cc1 = nano.read(2)
        value = int.from_bytes(cc1, "big", signed = True)
        data = struct.pack("<h", value)
        wavefile.writeframesraw(data)

    wavefile.close()

    print("\nEnd recording!\n")

    
    print("\nGetting Pm values...\n")

    for i in range (0, 2):
        data = getPmValues(i)
        dataList[i] = data

    print("\nPm values collected!\n")

    pm2_5 = dataList[0]
    pm10 = dataList[1]

    timeStamp = str(datetime.fromtimestamp(calendar.timegm(time.gmtime())))
    pmString = str(pm2_5) + ", " + str(pm10)
    newFile_Name = timeStamp + "; " + pmString + ".wav" # timestamp necessary: avoid same name audio files 

    old_name = r"YOUR/PATH/sound.wav" # SET path to your file: "sound.wav" previously generated
    new_name = r"YOUR/RECORDINGS/DIRECTORY/PATH" + newFile_Name # SET path to your desired recordings directory

    os.rename(old_name, new_name)

    fileNameCounter += 1
    fileAudioRecorded += 1
    
