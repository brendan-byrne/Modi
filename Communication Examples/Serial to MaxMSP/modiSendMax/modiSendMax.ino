// MIDI example
// this sketch requires a teensy device or an arduino that sports an atmega32u4 chip and the tee on ardu library

#include "modi.h"                            // Include Modi Library    
#define muxChannels 8                        // Number of channles on CD4051 Multiplexer

int started = 0; // flag for whether we've received serial yet
Modi Grid(6, 7, 8, 1);                       // Create a Modi Object. "Grid" serves as a descriptive name
// Grid.filterVal = .5;                      // If you select '2' as a smoothing option you can adjust it here

void setup() { 
  Serial.begin(115200);
  // Hybri Default 
  Grid.attach(A0, "pot");                    
  Grid.attach(0, "button");
}

void loop() {
  if (Serial.available()) {
    started = 1;
  }
  
  if (started) {
    Grid.refresh();                                                       // Required to get updated readings
    for (int x=0; x<Grid.numMux; x++) {                                   // Run through the rows
      for (int y=0; y<muxChannels; y++) {                                 // Run through the columns
        int reading = Grid.getReading(x, y);
        if (Grid.muxType[x] == 0) {
          reading = map(reading, 0, 1, 0, 127);
          Serial.print(reading);
          Serial.print(" ");
        }
        else {
          reading = map(reading, 0, 1023, 0, 127);
          reading = constrain(reading, 0, 127);        
          Serial.print(reading);
          Serial.print(" ");
        }
      }
    }
    Serial.println();
    delay(5);
  }
}























