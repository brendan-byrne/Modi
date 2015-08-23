#include "Arduino.h"
#include "Modi.h"

Modi::Modi(byte _selPinA, byte _selPinB, byte _selPinC, byte _smoothSel) {
  numMux = 0;                                           // This number increases by 1 with each attach() method used        
  smoothAmount = .5;                                    // Default amount of smoothing
  smoothSel = _smoothSel;                               // Toggles smoothing
  if (smoothSel > 1 || smoothSel < 0) {                 // Set smoothSel to 0 if improper input received
    smoothSel = 0;
  }
  
  selPins[0] = _selPinA;                                // Assign selection pins to variables
  selPins[1] = _selPinB;
  selPins[2] = _selPinC; 
  
  for (int i=0; i<sizeof(selPins); i++) {               // Each selPins[] designated as OUTPUT
    pinMode(selPins[i], OUTPUT); 
  }
}

void Modi::attach(byte _pin, String _anaType) {
  if (_anaType == "slide") slideType[numMux] = 1;       // Stores the type of controllers to be read
  else slideType[numMux] = 0;                           // 1 = Slidi     0 = Not Slidi
  
  if (_anaType == "button") analogType[numMux] = 0;     // Stores whether controllers should be read using analog or digital read
  else analogType[numMux] = 1;                          // 1 = Analog    0 = Digital
 
  inputPins[numMux] = _pin;                              // Put assigned input pin into an array
  pinMode(inputPins[numMux], INPUT);                     // The new input pin is designated as an INPUT
  numMux++;                                              // Advance one step in array for next usage of attach() method
}

void Modi::refresh() {
  for (int x=0; x<muxChannels; x++) {                    // Stepping through the channels allows for changing selPins[] less often
    for (int y=0; y<numMux; y++) {                       // numMux taken from how many times attach method was used
      int rawVal;                                        // Local variable to store value of incoming reading
      if (analogType[y]) {                               // Are the readings from the multiplexer push button signals?
        rawVal = analogRead(inputPins[y]);               // Take the reading
        if (smoothSel) {                                 // Is smoothing enabled?        
         // Check to see whether the incoming signal is from a Slidi module to determine which organize array to use
         if (slideType[y]) finalVals[y][muxOrganizeSlidi[x]] = smooth(rawVal, smoothAmount, finalVals[y][muxOrganizeSlidi[x]]);
         else finalVals[y][muxOrganize[x]] = smooth(rawVal, smoothAmount, finalVals[y][muxOrganize[x]]);
        } 
        else {          
         // Check to see whether the incoming signal is from a Slidi module to determine which organize array to use
         if (slideType[y]) finalVals[y][muxOrganizeSlidi[x]] = rawVal;
         else finalVals[y][muxOrganize[x]] = rawVal;
        }
      }      
      else {                                             // Are the readings from the multiplexer push button signals?
        rawVal = digitalRead(inputPins[y]);              // Digital read the buttons
        finalVals[y][muxOrganize[x]] = rawVal;           // Assign the reading to the finalVals[][] array
      }
    }
    digitalWrite(changePins[x], changeVals[x]);          // Change the selection pins based on the two Gray Code arrays
  }
}

int Modi::getReading(byte _mux, byte _channel) {
  return finalVals[_mux][_channel];                      // Return a value from the finalVals[][] array
}

// smooth   v0.2 by Paul Badger 2007 - http://playground.arduino.cc/Main/Smooth
int Modi::smooth(int data, float filterVal, float smoothedVal) {
  if (filterVal > 1) filterVal = .99;
  else if (filterVal <= 0) filterVal = 0;
  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);
  return (int)smoothedVal;
}

