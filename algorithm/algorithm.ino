/**
 *
 * MTE 482 - Final Design Project
 * iCross, An Automated Pedestrian Crossing System
 * By: Sean Miller, Thomas ten Kortenaar, Taylor Robertson
 * 2021-02-15
 * 
 */

// TODO - put in the proper thermopile pin once the electronics have been configured
const int THERMO_IN = 13; // Input analog pin for the thermopile input sensor

// TODO - put in the proper pressure pins once the electronics have been configured
const int PRESSURE_IN_1 = 14; // Input analog pin for pressure input sensor 1/5
const int PRESSURE_IN_2 = 15; // Input analog pin for pressure input sensor 2/5
const int PRESSURE_IN_3 = 16; // Input analog pin for pressure input sensor 3/5
const int PRESSURE_IN_4 = 17; // Input analog pin for pressure input sensor 4/5
const int PRESSURE_IN_5 = 18; // Input analog pin for pressure input sensor 5/5

// TODO - put the proper output pin once the electronics have been configured
const int SIGNAL_OUT = 19 // Output digital pin for whether a pedestrian was detected or not

// TODO - decide on which weights/thresholds work best for this application
const float PRESSURE_WEIGHT = 0.3;
const float THERMO_WEIGHT = 1.0 - PRESSURE_WEIGHT;
const float SENSOR_THRESHOLD = 0.3;


// Boolean value representing the state of whether a pedestrian has been detected or not
bool pedestrianDetected = LOW;

// State variables for all the input sensor values
int thermo = 0;
int pressure[5] = [];

// Intermediate values used in algorithm
int pressure1st = 0;
int pressure2nd = 0;
float pressureAvg = 0.0;
float sensorAvg = 0.0;

// Set up the pins within setup()
void setup() 
{
  // Set all the input pins to be inputs
  pinMode(THERMO_IN, INPUT);
  pinMode(PRESSURE_IN_1, INPUT);
  pinMode(PRESSURE_IN_2, INPUT);
  pinMode(PRESSURE_IN_3, INPUT);
  pinMode(PRESSURE_IN_4, INPUT);
  pinMode(PRESSURE_IN_5, INPUT);

  // Set the output pin to be an output
  pinMode(SIGNAL_OUT, OUTPUT);
}


// Continous loop that will run detecting pedestrians 24/7
void loop() 
{
  // Read in the sensor values from the input pins
  thermo = analogRead(THERMO_IN);
  pressure[0] = analogRead(PRESSURE_IN_1);
  pressure[1] = analogRead(PRESSURE_IN_2);
  pressure[2] = analogRead(PRESSURE_IN_3);
  pressure[3] = analogRead(PRESSURE_IN_4);
  pressure[4] = analogRead(PRESSURE_IN_5);

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
  pressureAvg = (pressure1 + pressure2) / 2.0;

  // The finalized sensor average wil be the weighted average of the 2 sensor types
  sensorAvg = PRESSURE_wEIGHT*pressureAvg + THERMO_WEIGHT*thermo;

  if (sensorAvg >= SENSOR_THRESHOLD) {
    pedestrianDetected = HIGH;
  } else {
    pedestrianDetected = LOW;
  }

  // write the value to the output pin
  digitalWrite(SIGNAL_OUT, pedestrianDetected);

  // run a small delay to preserve processor power
  delay(200);
}
