// --------------------HackingSTEM EM Spectrum-------------------------------
// This project is for use with the EM spectrum lesson plan 
// available from Microsoft Education Workshop at http://aka.ms/hackingSTEM 
// 
// Overview: 
// This sketch uses LEDs as light sensors (photodiode) to detect and measure 
// different wavelengths in various types of light sources.
//
// Pins:
// Pin A0: IR LED
// Pin A1: Red LED
// Pin A2: Green LED
// Pin A3: Blue LED
// Pin A4: UV LED
//
// This project uses an Arduino UNO microcontroller board. More information can
// be found by visiting the Arduino website: 
// https://www.arduino.cc/en/main/arduinoBoardUno 
//
// Comments, contributions, suggestions, bug reports, and feature requests 
// are welcome! For source code and bug reports see: 
// http://github.com/[TODO github path to Hacking STEM] 
// 
// Copyright 2019, Jen Fox, Microsoft EDU Workshop - HackingSTEM 
// MIT License terms detailed in LICENSE.txt 
// ----------------------------------------------------------------------------

// Program variables for project sensors --------------------------------------
const int kNumberOfLeds = 5;
int kledPins[kNumberOfLeds] = {A0, A1, A2, A3, A4};

// Excel variables for storing data sent to Excel -----------------------------
int colorSensors[kNumberOfLeds] = {1, 2, 3, 4, 5};

//Serial data variables
const char kDelimiter = ',';
const int kSerialInterval = 50;
unsigned long serialPreviousTime;


// SETUP ----------------------------------------------------------------------
void setup() {
  // Initializations occur here
  Serial.begin(9600);    
  _SFR_IO8(0x35) |= 0x10;   // A condensed method for disabling the internal 
                            // pull up resistors in the Arduino.

// This function changes the reference voltage for measuring the analog voltage
// of the LEDs to a 3.3V reference. This is why we added the jumper cable 
// between 3.3V to AREF!
  analogReference(EXTERNAL); 

}

// START OF MAIN LOOP --------------------------------------------------------- 
void loop()
{
  // Process sensors
  processSensors();

  // Process and send data to Excel via serial port (Data Streamer)
  processOutgoingSerial();

}

// SENSOR INPUT CODE-----------------------------------------------------------
void processSensors() 
{
  // Read analog value of LED inputs
  for(int i = 0; i < kNumberOfLeds; i++){
    colorSensors[i] = analogRead(kledPins[i]);
  }
  
  delay(10); //short delay to prevent the Arduino from overclocking
}

// Add any specialized methods and processing code here

// OUTGOING SERIAL DATA PROCESSING CODE----------------------------------------
void sendDataToSerial()
{
  // Send data out separated by a comma (kDelimiter)
  for(int i = 0; i < kNumberOfLeds; i++){
      Serial.print(colorSensors[i]);
      Serial.print(kDelimiter);
  }

  Serial.println(); // Add final line ending character only once
}

//-----------------------------------------------------------------------------
// DO NOT EDIT ANYTHING BELOW THIS LINE
//-----------------------------------------------------------------------------

// OUTGOING SERIAL DATA PROCESSING CODE----------------------------------------
void processOutgoingSerial()
{
   // Enter into this only when serial interval has elapsed
  if((millis() - serialPreviousTime) > kSerialInterval) 
  {
    serialPreviousTime = millis(); // Reset serial interval timestamp
    sendDataToSerial(); 
  }
}
