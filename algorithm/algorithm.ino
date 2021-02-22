#include "Arduino.h"

/**

   MTE 482 - Final Design Project
   iCross, An Automated Pedestrian Crossing System
   By: Sean Miller, Thomas ten Kortenaar, Taylor Robertson
   2021-02-15

*/

// TODO - put in the proper thermopile pin once the electronics have been configured
//const int THERMO_IN = 13; // Input analog pin for the thermopile input sensor

const int PRESSURE_IN_1 = A0; // Input analog pin for pressure input sensor 1/4
const int PRESSURE_IN_2 = A1; // Input analog pin for pressure input sensor 2/4
const int PRESSURE_IN_3 = A2; // Input analog pin for pressure input sensor 3/4
const int PRESSURE_IN_4 = A3; // Input analog pin for pressure input sensor 4/4
const int LED_OUT = 8; // Output digital pin for whether a pedestrian was detected or not
const int BUZZER_OUT = 9; // Output digital pin for whether a pedestrian was detected or not

// TODO - decide on which weights/thresholds work best for this application
const float PRESSURE_WEIGHT = 0.75;
const float THERMO_WEIGHT = 1.0 - PRESSURE_WEIGHT;
const float SENSOR_THRESHOLD = 0.25;


// Boolean value representing the state of whether a pedestrian has been detected or not
bool pedestrianDetected = LOW;

// State variables for all the input sensor values
int thermo = 0;
int pressure[4];

// Intermediate values used in algorithm
int pressure1st = 0;
int pressure2nd = 0;
float pressureAvg = 0.0;
float sensorAvg = 0.0;

// Set up the pins within setup()
void setup()
{
  SerialUSB.begin(9600); // Initialize Serial Monitor USB
  Serial1.begin(9600); // Initialize hardware serial port, pins 0/1

  while (!SerialUSB) ; // Wait for Serial monitor to open

  // Send a welcome message to the serial monitor:
  SerialUSB.println("Beginning Operations!");


  // Set all the input pins to be inputs
  //  pinMode(THERMO_IN, INPUT);
  pinMode(PRESSURE_IN_1, INPUT);
  pinMode(PRESSURE_IN_2, INPUT);
  pinMode(PRESSURE_IN_3, INPUT);
  pinMode(PRESSURE_IN_4, INPUT);
  
    // Set the output pins to be an output
  pinMode(LED_OUT, OUTPUT);
  pinMode(BUZZER_OUT, OUTPUT);
}


// Continous loop that will run detecting pedestrians 24/7
void loop()
{
  // Read in the sensor values from the input pins
  //  thermo = analogRead(THERMO_IN);
    pressure[0] = analogRead(PRESSURE_IN_1) / 1024.0;
    pressure[1] = analogRead(PRESSURE_IN_2)/ 1024.0;
    pressure[2] = analogRead(PRESSURE_IN_3)/ 1024.0;
    pressure[3] = analogRead(PRESSURE_IN_4)/ 1024.0;

//  int val = analogRead(A0);
//  SerialUSB.println("The value of the pressure sensor is:");
//  SerialUSB.println(val);

  // Only use the 2 highest readings from the pressure sensor readings
  for (int i = 0; i < 5; i++) {
    if (pressure[i] >= pressure1st) {
      pressure1st = pressure[i];
    }
  }

  for (int i = 0; i < 5; i++) {
    if (pressure[i] < pressure1st) {
      if (pressure[i] > pressure2nd) {
        pressure2nd = pressure[i];
      }
    }
  }

  // Take the average of the 2 highest pressure sensor values
  pressureAvg = (pressure1st + pressure2nd) / 2.0;

  // The finalized sensor average wil be the weighted average of the 2 sensor types
  sensorAvg = PRESSURE_WEIGHT * pressureAvg + THERMO_WEIGHT * thermo;

  if (sensorAvg >= SENSOR_THRESHOLD) {
    pedestrianDetected = HIGH;
  } else {
    pedestrianDetected = LOW;
  }

  // write the value to the output pin
  //  digitalWrite(SIGNAL_OUT, pedestrianDetected);
  SerialUSB.println("Pedestrian Detected?");
  SerialUSB.println(pedestrianDetected);
  digitalWrite(LED_OUT, HIGH);
  tone(BUZZER_OUT, 250);
  digitalWrite(BUZZER_OUT, HIGH);
  
  // run a small delay to preserve processor power
//  delay(200);
}
