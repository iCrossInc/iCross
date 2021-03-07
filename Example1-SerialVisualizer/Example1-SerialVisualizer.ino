/*
  Using the Panasonic Grid-EYE Sensor
  By: Nick Poole
  SparkFun Electronics
  Date: January 12th, 2018
  
  MIT License: Permission is hereby granted, free of charge, to any person obtaining a copy of this 
  software and associated documentation files (the "Software"), to deal in the Software without 
  restriction, including without limitation the rights to use, copy, modify, merge, publish, 
  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or 
  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14568
  
  This example implements a rudimentary heat camera in the serial terminal, using a matrix of ascii
  characters to represent the temperature of each pixel in the device. Start your terminal at 115200 
  and make the window as small as possible for best effect.
  
  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
*/

#include <SparkFun_GridEYE_Arduino_Library.h>
#include <Wire.h>

// Use these values (in degrees C) to adjust the contrast
#define HOT 40
#define COLD 20

// This table can be of type int because we map the pixel 
// temperature to 0-3. Temperatures are reported by the 
// library as floats
int pixelTable[64];

GridEYE grideye;

void setup() {

  // Start your preferred I2C object 
  Wire.begin();
  // Library assumes "Wire" for I2C but you can pass something else with begin() if you like
  grideye.begin();
  // Pour a bowl of serial
  SerialUSB.begin(9600);
  Serial1.begin(9600); // Initialize hardware serial port, pins 0/1 

  while (!SerialUSB) ; // Wait for Serial monitor to open

  // Send a welcome message to the serial monitor:
  SerialUSB.println("Beginning Operations!");
}

float temp = 0.0;

void loop() {

//  temp = grideye.getDeviceTemperature();
//  SerialUSB.print("The temperature of the device is:");
//  SerialUSB.println(temp);

//  // loop through all 64 pixels on the device and map each float value to a number
//  // between 0 and 3 using the HOT and COLD values we set at the top of the sketch
  for(unsigned char i = 0; i < 64; i++){
//    SerialUSB.println(i);
//    SerialUSB.println("The temperature at this pixel is:");
    pixelTable[i] = grideye.getPixelTemperature(i);
//    SerialUSB.println(pixelTable[i]);

//    pixelTable[i] = map(grideye.getPixelTemperature(i), COLD, HOT, 0, 3);
  }

  SerialUSB.println("New temp readings:");
//  // loop through the table of mapped values and print a character corresponding to each
//  // pixel's temperature. Add a space between each. Start a new line every 8 in order to 
//  // create an 8x8 grid
  for(unsigned char i = 0; i < 64; i++){
//    if(pixelTable[i]==0){SerialUSB.print(".");}
//    else if(pixelTable[i]==1){SerialUSB.print("o");}
//    else if(pixelTable[i]==2){SerialUSB.print("0");}
//    else if(pixelTable[i]==3){SerialUSB.print("O");}
//    SerialUSB.print(" ");
//    if((i+1)%8==0){
//      SerialUSB.println();
//    }
    SerialUSB.println(pixelTable[i]);
  }
//
//  // in between updates, throw a few linefeeds to visually separate the grids. If you're using
//  // a serial terminal outside the Arduino IDE, you can replace these linefeeds with a clearscreen
//  // command
//  SerialUSB.println();
//  SerialUSB.println();

//  SerialUSB.println("We in the loop I promise....");


  // toss in a delay because we don't need to run all out
  delay(200);

}
