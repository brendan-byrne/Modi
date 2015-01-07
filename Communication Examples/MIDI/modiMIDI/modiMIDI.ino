// MIDI example
// this sketch requires a teensy device or an arduino that sports an atmega32u4 chip and the tee on ardu library

#include "modi.h"                            // Include Modi Library    
#define muxChannels 8                        // Number of channles on CD4051 Multiplexer

Modi Grid(6, 7, 8, 1);                       // Create a Modi Object. "Grid" serves as a descriptive name
// Grid.filterVal = .5;                      // If you select '2' as a smoothing option you can adjust it here

int lastReading[8][muxChannels];

void setup() { 
  // Hybri Default 
  Grid.attach(A0, "pot");                    
  Grid.attach(0, "button");
}

void loop() {
  Grid.refresh();                                                       // Required to get updated readings
  for (int x=0; x<Grid.numMux; x++) {                                   // Run through the rows
    for (int y=0; y<muxChannels; y++) {                                 // Run through the columns
      int reading = Grid.getReading(x, y);
        if (Grid.muxType[x] == 0) {
          if (reading != lastReading[x][y]) {
            if (reading == HIGH) usbMIDI.sendNoteOn(30+x+y, 100, 1);
            if (reading == LOW) usbMIDI.sendNoteOff(30+x+y, 0, 1);
            lastReading[x][y] = reading;
          }
        }
        else if (reading > lastReading[x][y]+7 || reading < lastReading[x][y]-7) {
          lastReading[x][y] = reading;
          reading = map(reading, 0, 1023, 0, 127);
          reading = constrain(reading, 0, 127);        
          usbMIDI.sendControlChange(20+x+y, reading, 1);
        }
    }
  }
}























