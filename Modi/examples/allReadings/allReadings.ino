// ------------------------------------------------------------------ 
// MODI: Report Readings Over Serial Example
// Xiwi Electronics - http://xiwielectronics.com/
// GITHUB REPO: https://github.com/brendan-byrne/Modi
// BOARD: Arduino / Teensy / Any microcontroller with Serial
// DESCRIPTION: This sketch reports all values from your Modi Matrix
// ------------------------------------------------------------------ 

#include <Modi.h>                  // Include Modi Library    

Modi Matrix(2, 3, 4, 1);           // Modi Matrix(pcbA, pcbB, pcbC, toggleSmoothing)  
                                   // toggleSmoothing = 0 : Smoothing OFF
                                   // toggleSmoothing = 1 : Smoothing ON

void setup() {   
  // ATTACHING PINS : Matrix.attach(pcb1-8, typeOfRow)
  // Accepted typeOfRow strings  "pot", "button", or "slide"
  // When attaching pins start with the topmost row and work down with each Matrix.attach

  // UNCOMMENT ONE OF THE FOLLOWING TO TEST A SINGLE MODI BOARD  
  // ----------------------------------------------------------
  Matrix.attach(A0, "pot");        // HYBRI
  Matrix.attach(A1, "button");  
  
//  Matrix.attach(A0, "pot");      // TURNI
//  Matrix.attach(A1, "pot");  

//  Matrix.attach(A0, "button");   // PUSHI
//  Matrix.attach(A1, "button");  

//  Matrix.attach(A0, "slide");    // SLIDI
  // ----------------------------------------------------------
  Matrix.smoothAmount = .9;           // If toggleSmoothing = 1 : Adjust amount of smoothing
                                   // .999 = More Smoothing  : .001 = Less Smoothing
  Serial.begin(9600);              // Begin Serial communication
}

void loop() {
  Matrix.refresh();                             // REQUIRED TO RECEIVE UPDATED READINGS
  
  for (int x=0; x<Matrix.numMux; x++) {         // Loop through the rows. numMux is equal to the number of times attach() is called in setup
    Serial.print(x);                            // Print row number at start of every line
    Serial.print(" : \t");
    for (int y=0; y<muxChannels; y++) {  // Nested loop through the columns
      Serial.print(Matrix.getReading(x, y));    // Print the reading at (x, y) coordinates
      Serial.print("\t");                       // Tab to create space between readings
    }
    Serial.println();                           // New line for next row of readings
  }
  
  delay(100);                                   // Makes easier to read serial monitor
}























