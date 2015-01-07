// MIDI example
// This sketch requires a teensy device or an arduino that sports an atmega32u4 chip and the tee on ardu library
// Under the "Tools" drop-down menu. Set USB Type to "MIDI."

#include "modi.h"                            // Include Modi Library    
#define muxChannels 8                        // Number of channles on CD4051 Multiplexer

Modi Grid(6, 7, 8, 1);                       // Create a Modi Object. "Grid" serves as a descriptive name. Smoothing type set to 1.
// Grid.filterVal = .5;                      // If you select '2' as a smoothing option you can adjust it here

int lastReading[8][muxChannels];

// Uncomment this section to use your own CC values
// This two-dimensional array supports 2 rows of multiplexers. Add more rows if you have more modules
// This array represents CC and Note values
//byte midiVals[8][8] {              
//  {20, 21, 22, 23, 24, 25, 26, 27}, // CC values from Hybri
//  {30, 31, 32, 33, 34, 35, 36, 37}, // Note Values from Hybri
//}

void setup() { 
  // Hybri Default 
  Grid.attach(A0, "pot");                    
  Grid.attach(0, "button");
}

void loop() {
  Grid.refresh();                                    // Required to get updated readings
  for (int x=0; x<Grid.numMux; x++) {                // Run through the rows
    for (int y=0; y<muxChannels; y++) {              // Run through the columns
      int reading = Grid.getReading(x, y);
        if (Grid.muxType[x] == 0) {                  // Check to see if the multiplexer is a button
          if (reading != lastReading[x][y]) {        // Only do something if the state of the button has changed
            
            // Comment this section to use your own note values
            if (reading == HIGH) usbMIDI.sendNoteOn(30+x+y, 100, 1);    // If button HIGH, send a note on message
            if (reading == LOW) usbMIDI.sendNoteOff(30+x+y, 0, 1);      // If button LOW, send a note off message
            
            // Uncomment this section to use your own note values
//            if (reading == HIGH) usbMIDI.sendNoteOn(midiVals[x][y], 100, 1);
//            if (reading == LOW) usbMIDI.sendNoteOff(midiVals[x][y], 0, 1);
            
            lastReading[x][y] = reading;                                 // Set lastReading, to check against 
          }
        }
        else if (reading > lastReading[x][y]+7 || reading < lastReading[x][y]-7) {
          lastReading[x][y] = reading;
          reading = map(reading, 0, 1023, 0, 127);
          reading = constrain(reading, 0, 127); 
           
          // Comment this line to use your own CC values        
          usbMIDI.sendControlChange(20+x+y, reading, 1);
          
          // Uncomment this line to use your own CC values
//          usbMIDI.sendControlChange(midiVals[x][y]
        }
    }
  }
}























