// Test Example
// This sketch reports all values from 

#include "modi.h"                            // Include Modi Library    
#define muxChannels 8                        // Number of channles on CD4051 Multiplexer

Modi Grid(6, 7, 8, 1);                       // Create a Modi Object. "Grid" serves as a descriptive name
// Grid.filterVal = .5;                      // If you select '2' as a smoothing option you can adjust it here

void setup() { 
  // Hybri Default 
  Grid.attach(A0, "pot");                    
  Grid.attach(0, "button");
}

void loop() {
  Grid.refresh();                            // Required to get updated readings
  for (int x=0; x<Grid.numMux; x++) {        // Run through the rows
    for (int y=0; y<muxChannels; y++) {      // Run through the columns
      Serial.print(Grid.getReading(x, y));  // Print the reading at the (x, y) coordinate
      Serial.print("\t");                   // Tab to create space between readings
    }
    Serial.println();                       // New line for next row of readings
  }
  Serial.println();                         // Print a blank line
  delay(50);
}























