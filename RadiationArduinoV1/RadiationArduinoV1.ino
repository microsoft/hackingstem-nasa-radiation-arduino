// --------------------HackingSTEM NASA Radiation-------------------------------
// Measuring Radiation for use with the NASA Radiation lesson plan 
// available from Microsoft Education Workshop at http://aka.ms/hackingSTEM 
// 
// Overview: 
// This sketch uses LEDs as light sensors (photodiode) to detect and measure 
// different wavelengths in various types of light sources.
//
// This project uses an Arduino UNO microcontroller board. More information can
// be found by visiting the Arduino website: 
// https://www.arduino.cc/en/main/arduinoBoardUno 
//
// Comments, contributions, suggestions, bug reports, and feature requests 
// are welcome! For source code and bug reports see: 
// http://github.com/[TODO github path to Hacking STEM] 
// 
// Copyright 2019, Jen Fox (jenfoxbot) Microsoft EDU Workshop - HackingSTEM 
// MIT License terms detailed in LICENSE.txt 
// ----------------------------------------------------------------------------

// Program variables ----------------------------------------------------------
int ledPins[6] = {A0, A1, A2, A3, A4, A5};
const int kNumberOfLeds = 6;

// Excel variables ------------------------------------------------------------
int colorSensors[6] = {1, 2, 3, 4, 5, 6};


// Serial data variables ------------------------------------------------------
const byte kNumberOfChannelsFromExcel = 6; //Incoming Serial Data Array
// IMPORTANT: This must be equal to number of channels set in Data Streamer

const char kDelimiter = ',';    // Data Streamer expects a comma delimeter
const int kSerialInterval = 50;   // Interval between serial writes
unsigned long serialPreviousTime; // Timestamp to track serial interval

char* arr[kNumberOfChannelsFromExcel];

// SETUP ----------------------------------------------------------------------
void setup() {
  // Initializations occur here
  Serial.begin(9600);    
  _SFR_IO8(0x35) |= 0x10;   // global disable pull up resistors

}

// START OF MAIN LOOP --------------------------------------------------------- 
void loop()
{
  // Process sensors
  processSensors();

  // Read Excel variables from serial port (Data Streamer)
  processIncomingSerial();

  // Process and send data to Excel via serial port (Data Streamer)
  processOutgoingSerial();

  if ( strcmp ("Apple", arr[0]) == 0){ // Compares STR1 to STR2 returns 0 if true.
      Serial.println("working");
  }
}

// SENSOR INPUT CODE-----------------------------------------------------------
void processSensors() 
{
  // Read analog value of LED inputs
  for(int i = 0; i < kNumberOfLeds; i++){
    colorSensors[i] = analogRead(ledPins[i]);
  }
  
  delay(10); //short delay to prevent overclocking
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

// INCOMING SERIAL DATA PROCESSING CODE----------------------------------------
void processIncomingSerial()
{
  if(Serial.available()){
    parseData(GetSerialData());
  }
}

// Gathers bytes from serial port to build inputString
char* GetSerialData()
{
  static char inputString[64]; // Create a char array to store incoming data
  memset(inputString, 0, sizeof(inputString)); // Clear the memory from a pervious reading
  while (Serial.available()){
    Serial.readBytesUntil('\n', inputString, 64); //Read every byte in Serial buffer until line end or 64 bytes
  }
  //TODO reject string if comma count isn't >= to kNumberOfChannelsFromExcel -1 
  return inputString;
}

// Seperate the data at each delimeter
void parseData(char data[])
{
    char *token = strtok(data, ","); // Find the first delimeter and return the token before it
    int index = 0; // Index to track storage in the array
    while (token != NULL){ // Char* strings terminate w/ a Null character. We'll keep running the command until we hit it
      arr[index] = token; // Assign the token to an array
      token = strtok(NULL, ","); // Conintue to the next delimeter
      index++; // incremenet index to store next value
    }
}
