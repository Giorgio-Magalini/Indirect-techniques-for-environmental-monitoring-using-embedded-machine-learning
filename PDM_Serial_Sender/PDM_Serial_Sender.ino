/**
# Thesis project on: Indirect techniques for environmental monitoring using embedded machine learning
# Author: Giorgio Magalini
# Date: 21/02/2023
# Università degli Studi di Padova
# Software - Audio acquisition
*/

#include <PDM.h>

// Serial input commands
#define START_REC '1'
#define STOP_REC '2'

// default PCM output frequency
static const int frequency = 16000;

// one channel (mono mode)
static const int channels = 1;

// Buffer to read samples into, each sample is 16-bits
short sampleBuffer[512];

// 8-bit buffer for serial writing
byte sampleBuffer_8bit[1536];

// Number of audio samples read
volatile int samplesRead;

int sb_index = 0;

int i = 0;

char pythonInput;

void setup() {

  SerialUSB.begin(115200);
  while (!SerialUSB);

  // Configure the data receive callback
  PDM.onReceive(onPDMdata);

  // Optionally set the gain
  // Defaults to 20 on the BLE Sense and 24 on the Portenta Vision Shield
  PDM.setGain(127);

  // Initialize PDM with:
  // - one channel (mono mode)
  // - a 16 kHz sample rate for the Arduino Nano 33 BLE Sense
  // - a 32 kHz or 64 kHz sample rate for the Arduino Portenta Vision Shield
  if (!PDM.begin(channels, frequency)) {
    SerialUSB.println("Failed to start PDM!");
    while (1);
  }
}

void loop() {
  if (Serial.available() > 0) {
    
    pythonInput = Serial.read();

    if (pythonInput == START_REC) {
      if (!PDM.begin(channels, frequency)) {
        SerialUSB.println("Failed to start PDM!");
        while (1);
      }
      else {
        delay(500);
      }
    }
    if (pythonInput == STOP_REC) {
      PDM.end();
    }
  }

  // Wait for samples to be read
  if (samplesRead) {

    // Print samples to the serial monitor or plotter
    for (int i = 0; i < samplesRead; i++) {

      sampleBuffer_8bit[sb_index] = '\n';
      sb_index ++;
      sampleBuffer_8bit[sb_index] = (sampleBuffer[i] >> 8) & 0xFF;
      sb_index ++;
      sampleBuffer_8bit[sb_index] = (sampleBuffer[i] & 0xFF);
      sb_index ++;


      //Sending data via SerialUSB
      if (sb_index >= 1536) {
        SerialUSB.write(sampleBuffer_8bit, sb_index);
        sb_index = 0;
      }
    }

    // Clear the read count
    samplesRead = 0;
  }
}
/**
   Callback function to process the data from the PDM microphone.
   NOTE: This callback is executed as part of an ISR.
   Therefore using `Serial` to print messages inside this function isn't supported.
 * */
void onPDMdata() {
  // Query the number of available bytes
  int bytesAvailable = PDM.available();

  // Read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}
