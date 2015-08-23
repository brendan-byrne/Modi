// ------------------------------------------------------------------
// MODI: MIDI Controller Using Hybri
// Xiwi Electronics - http://xiwielectronics.com/
// GITHUB REPO: https://github.com/brendan-byrne/Modi
// BOARD: Teensy - USB Type set to MIDI OR Leonardo with TeeOnArdu
// DESCRIPTION: This sketch reports all values from your Modi Matrix
// ------------------------------------------------------------------

#include <Modi.h>                             // Include Modi Library   
#define numberOfRows 2                        // Number of rows in your Modi System. Hybri has 2. If you used two Turni boards this number would be 4

Modi Matrix(0, 1, 2, 1);                      // Modi Matrix(pcbA, pcbB, pcbC, toggleSmoothing)
                                              // toggleSmoothing = 0 : Smoothing OFF
                                              // toggleSmoothing = 1 : Smoothing ON

int lastReading[numberOfRows][muxChannels];   // Check new readings against these values to determine whether or not to transmit MIDI value

byte midiVals[numberOfRows][muxChannels] = {  // This array represents CC and Note values for a Hybri PCB. Add more rows if you have more boards
  {20, 21, 22, 23, 24, 25, 26, 27},           // Pots    - CC Values
  {48, 49, 50, 51, 52, 53, 54, 55}            // Buttons - Note Values
};

void setup() {
  // ATTACHING PINS : Matrix.attach(pcb1-8, typeOfRow)
  // Accepted typeOfRow strings  "pot", "button", or "slide"
  // When attaching pins start with the topmost row and work down with each Matrix.attach
  Matrix.attach(A0, "pot");                   
  Matrix.attach(A1, "button");
  Matrix.smoothAmount = .5;                   // If toggleSmoothing = 1 : Adjust amount of smoothing
                                              // .999 = More Smoothing  : .001 = Less Smoothing
}

void loop() {
  Matrix.refresh();                                                               // REQUIRED TO RECEIVE UPDATED READINGS
  for (int x = 0; x < Matrix.numMux; x++) {                                       // Loop through the rows
    for (int y = 0; y < muxChannels; y++) {                                       // Loop through the columns
      int reading = Matrix.getReading(x, y);                                      // Assign (x,y) reading to temporary variable      
      if (Matrix.analogType[x]) {                                                 // Matrix.analogType stores whether the row is an analog or digital reading. 0 = digital and 1 = analog
        if (reading > lastReading[x][y] + 7 || reading < lastReading[x][y] - 7) { // New reading is only accepted if it's different enough. Keeps readings very steady
          lastReading[x][y] = reading;                                            // Assign the reading to lastReading to check against later
          reading = map(reading, 0, 1023, 0, 127);                                // Map readings to MIDI friendly range
          usbMIDI.sendControlChange(midiVals[x][y], reading, 1);                  // Send control change message on channel 1
        }
      } 
      else if (reading != lastReading[x][y]) {                                    // If reading is different from last reading then proceed
        if (reading == HIGH) usbMIDI.sendNoteOn(midiVals[x][y], 100, 1);          // Send MIDI note on channel 1 with velocity of 100
        if (reading == LOW) usbMIDI.sendNoteOff(midiVals[x][y], 0, 1);            // Turn MIDI note off if reading changes from HIGH to LOW
        lastReading[x][y] = reading;
      }
    }
  }
}































