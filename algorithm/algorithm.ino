/**

   MTE 482 - Final Design Project
   iCross, An Automated Pedestrian Crossing System
   By: Sean Miller, Thomas ten Kortenaar, Taylor Robertson
   2021-02-15

*/

#include "Arduino.h"
#include <cmath>
#include <SparkFun_GridEYE_Arduino_Library.h>
#include <Wire.h>

const int PRESSURE_IN_1 = A0; // Input analog pin for pressure input sensor 1/4
const int PRESSURE_IN_2 = A1; // Input analog pin for pressure input sensor 2/4
const int PRESSURE_IN_3 = A2; // Input analog pin for pressure input sensor 3/4
const int PRESSURE_IN_4 = A3; // Input analog pin for pressure input sensor 4/4
const int LED_OUT = 8; // Output digital pin for whether a pedestrian was detected or not
const int BUZZER_OUT = 9; // Output digital pin for whether a pedestrian was detected or not

// TODO - decide on which weights/thresholds work best for this application
const float PRESSURE_WEIGHT = 1.0;
const float THERMO_WEIGHT = 1.0 - PRESSURE_WEIGHT;
const float SENSOR_THRESHOLD = 0.25;

// TODO = define the average environment temperature here
float ENVIRO_TEMP = 17.5;

// TODO - define the average pressure reading at ambient
float PRESSURE_CONST = 800.0;

// Boolean value representing the state of whether a pedestrian has been detected or not
bool pedestrianDetected = LOW;

// State variables for all the input sensor values
float pressure[4];
float thermoPixels[64];
GridEYE grideye;

// Intermediate values used in algorithm
float pressureProb = 0.0;
float thermoProb = 0.0;
float totalProb = 0.0;


// Set up the pins and comms within setup()
void setup()
{
  // Start the thermopile
  Wire.begin();
  grideye.begin();

  SerialUSB.begin(9600); // Initialize Serial Monitor USB
  Serial1.begin(9600); // Initialize hardware serial port, pins 0/1

  while (!SerialUSB) ; // Wait for Serial monitor to open

  // Send a welcome message to the serial monitor:
  SerialUSB.println("Beginning Operations!");

  // Set all the input pins to be inputs
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
  // Read in the data from the pressure sensors
  pressure[0] = analogRead(PRESSURE_IN_1);
  pressure[1] = analogRead(PRESSURE_IN_2);
  pressure[2] = analogRead(PRESSURE_IN_3);
  pressure[3] = analogRead(PRESSURE_IN_4);

  SerialUSB.println("Pressure readings");
  SerialUSB.println(pressure[0]);
  SerialUSB.println(pressure[1]);
  SerialUSB.println(pressure[2]);
  SerialUSB.println(pressure[3]);

  // Extract the data from the thermopile sensors
  // by looping through all 64 pixels on the device
  for(unsigned char i = 0; i < 64; i++) {
    thermoPixels[i] = grideye.getPixelTemperature(i);
  }

  
//  SerialUSB.println("Current temperature is:");
//  SerialUSB.println(grideye.getDeviceTemperature());

  // TODO - make this more robust. This is a good approximation but not perfect
  ENVIRO_TEMP = grideye.getDeviceTemperature() - 4.0;

  // calculate the probabilities
  pressureProb = pressureProbability(pressure);
  thermoProb = thermoProbability(thermoPixels);

  SerialUSB.println("Temperature probability:");
  SerialUSB.println(thermoProb);

  SerialUSB.println("Pressure probability:");
  SerialUSB.println(pressureProb);

  // The finalized sensor average wil be the weighted average of the 2 sensor types
  totalProb = PRESSURE_WEIGHT*pressureProb + THERMO_WEIGHT*thermoProb;

  if (totalProb >= SENSOR_THRESHOLD) {
    pedestrianDetected = HIGH;
  } else {
    pedestrianDetected = LOW;
  }

  // Output to console whether it detected a pedestrian or not
  SerialUSB.println("Pedestrian Detected?");
  SerialUSB.println(pedestrianDetected);

  // write the value to the output pin
  if (pedestrianDetected) {
    tone(BUZZER_OUT, 250);
    // digitalWrite(BUZZER_OUT, HIGH);
    digitalWrite(LED_OUT, HIGH);
  } else {
    digitalWrite(LED_OUT, LOW);
  }


  // add some new lines for console cosmetics
  SerialUSB.println();
  
  // run a small delay to preserve processor power
  delay(200);
}

// Function which outputs the probability that a pedestrian
// is present as determined by the thermopile
float thermoProbability(float pixels[]) {

  float totalTempHottestPixels = 0.0;
  float avgTempHottestPixels = 0.0;

  bubbleSort(pixels, 64);

  const int NUM_PIXELS_HOTTEST = 4;

  // take the average of the top n% of the hottest pixels
  for (int i = 0; i < NUM_PIXELS_HOTTEST; i++) {
    totalTempHottestPixels += pixels[i];
  }

  avgTempHottestPixels = totalTempHottestPixels / NUM_PIXELS_HOTTEST;

  // now, we can apply a simple probability density function on the average temperature of the hottest
  // pixels in comparison with the environment temperature to see if a pedestrian is indeed
  // present at the intersection
  float tempDelta = (avgTempHottestPixels - ENVIRO_TEMP) / ENVIRO_TEMP;
  float tempProb = probability(tempDelta);
  return tempProb;
}

float probability(float ratio) {
  return 1 - std::exp(-12*ratio);
}


// Function which outputs the probability that a pedestrian
// is present as determined by the pressure sensors
float pressureProbability(float pressure[]) {

  float pressure1st = 0;
//  float pressure2nd = 0;

  // Only use the 2 highest readings from the pressure sensor readings
  for (int i = 0; i < 5; i++) {
    if (pressure[i] >= pressure1st) {
      pressure1st = pressure[i];
    }
  }

//  for (int i = 0; i < 5; i++) {
//    if (pressure[i] < pressure1st) {
//      if (pressure[i] > pressure2nd) {
//        pressure2nd = pressure[i];
//      }
//    }
//  }

  // Take the average of the 2 highest pressure sensor values
//  float pressureAvg = (pressure1st + pressure2nd) / 2.0;

  // Scale this to match the 0-1024 range of the pressure sensors
//  pressureAvg /= 300;
//  pressure1st /= 800;

  float pressureDelta = (pressure1st - PRESSURE_CONST) / PRESSURE_CONST;


  return probability(pressureDelta);  
}



// An optimized version of Bubble Sort 
void bubbleSort(float arr[], int n) 
{ 
   int i, j; 
   bool swapped; 
   for (i = 0; i < n-1; i++) 
   { 
     swapped = false; 
     for (j = 0; j < n-i-1; j++) 
     { 
        if (arr[j] > arr[j+1]) 
        { 
           swap(&arr[j], &arr[j+1]); 
           swapped = true; 
        } 
     } 
  
     // IF no two elements were swapped by inner loop, then break 
     if (swapped == false) 
        break; 
   } 
}

 void swap(float *xp, float *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
