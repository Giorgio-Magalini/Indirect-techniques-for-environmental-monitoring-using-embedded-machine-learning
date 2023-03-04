/**
# Thesis project on: Indirect techniques for environmental monitoring using embedded machine learning
# Author: Giorgio Magalini
# Date: 23/02/2023
# Università degli Studi di Padova
# Software - Particolate Mesurement
*/

#include "myhpma115S0.h"
#include <SoftwareSerial.h>

// Sets the size of the moving average window
#define WINDOW_SIZE 10 

//Serial input commands
#define GET_PM2_5 '1'
#define GET_PM10 '2'


int index10 = 0;
int pm10Value = 0;
int sumPm10 = 0;
int readingsPm10[WINDOW_SIZE];
int averagedPm10 = 0;

int index2_5 = 0;
int pm2_5Value = 0;
int sumPm2_5 = 0;
int readingsPm2_5[WINDOW_SIZE];
int averagedPm2_5 = 0;

char pythonInput;

unsigned int pm2_5, pm10;

int i = 0;


//Create an instance of software serial
SoftwareSerial hpmaSerial(10, 11); // Feather TX, Feather RX

//Create an instance of the myhpma115S0 library
MYHPMA115S0 hpma115S0(hpmaSerial);

void setup() {
  Serial.begin(57600);
  hpmaSerial.begin(9600);
  hpma115S0.Init();
  hpma115S0.StartParticleMeasurement();
  MovingAverageInit();
}

void loop() {

  if (Serial.available() > 0) {

    pythonInput = Serial.read();

    if (pythonInput == '1') {
      if (hpma115S0.ReadParticleMeasurement(&pm2_5, &pm10)) {
        Serial.println(MovingAveragePm2_5(pm2_5));
      }
    }
    if (pythonInput == '2') {
      if (hpma115S0.ReadParticleMeasurement(&pm2_5, &pm10)) {
        Serial.println(MovingAveragePm10(pm10));
      }
    }
  }
  if (hpma115S0.ReadParticleMeasurement(&pm2_5, &pm10)) {
    MovingAveragePm2_5(pm2_5);
    MovingAveragePm10(pm10);
    delay(1000);
  }
}

// Moving average function for pm10
unsigned int MovingAveragePm10(unsigned int pm10) {
  sumPm10 -= readingsPm10[index10];       // Remove the oldest entry from the sum
  readingsPm10[index10] = pm10;           // Add the newest reading to the window
  sumPm10 += pm10;                       // Add the newest reading to the sum
  index10 = (index10 + 1) % WINDOW_SIZE;    // Increment the index, and wrap to 0 if it exceeds the window size
  averagedPm10 = sumPm10 / WINDOW_SIZE; // Divide the sum of the window by the window size for the result
  return (unsigned int)averagedPm10;
}

// Moving average function for pm2,5
unsigned int MovingAveragePm2_5(unsigned int pm2_5) {
  sumPm2_5 -= readingsPm2_5[index2_5];       // Remove the oldest entry from the sum
  readingsPm2_5[index2_5] = pm2_5;           // Add the newest reading to the window
  sumPm2_5 += pm2_5;                       // Add the newest reading to the sum
  index2_5 = (index2_5 + 1) % WINDOW_SIZE;    // Increment the index, and wrap to 0 if it exceeds the window size
  averagedPm2_5 = sumPm2_5 / WINDOW_SIZE; // Divide the sum of the window by the window size for the result
  return (unsigned int)averagedPm2_5;
}

// Moving average initializer: prevents initial random or null values
void MovingAverageInit() {
  Serial.println("M.A. init");
  for (int i = 0; i < WINDOW_SIZE; i++) {
    if (hpma115S0.ReadParticleMeasurement(&pm2_5, &pm10)) {
      MovingAveragePm2_5(pm2_5);
      MovingAveragePm10(pm10);
    }
    delay(1000);
  }
  Serial.println("M.A. init done");
}
