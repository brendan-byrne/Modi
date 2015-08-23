// ------------------------------------------------------------------ 
// MODI: Step Sequencer Example
// Xiwi Electronics - http://xiwielectronics.com/
// GITHUB REPO: https://github.com/brendan-byrne/Modi
// BOARD: Teensy - USB Type set to MIDI OR Leonardo with TeeOnArdu
// DESCRIPTION: This sketch use the Hybri board for a step sequencer 
// project. In addition to the Hybri PCB you'll need 2 potentiometers
// and 8 LEDs with resistors. Follow the pin mappings seen below.
// ------------------------------------------------------------------ 

#include <Modi.h>                           

// PIN MAPPING
Modi Matrix(0, 1, 2, 1);                    // selPins 0, 1, ans 2. Smoothing activated 
byte seqLengthPot = A7;                     // Reading from this pot determines sequence length
byte stepTimePot = A6;                      // Reading from this pot determines speed of sequencer
byte leds[] = {3, 4, 5, 6, 9, 10, 12, 13};  // LEDs need resistors

// SEQUENCER VARIABLES
byte seqPosition = 0;                       // Current step of sequencer
unsigned long lastTime;                     // Last time the sequence advanced a step

// Hyrbi Buttons
boolean stepToggles[8];                     // Holds value that determines whether steps are on/off
boolean lastButtonStates[8];                // Holds the last states of button presses

// Hybri Potentiometers
int lastReading[8];                         // Last reading from a Hybri potentiometer
byte midiNote[8];                           // The MIDI note to be sent from each step when activated
byte lastMidiNote;                          // The current MIDI Note On. Variable used to send Note Off when sequence updates

void setup() { 
  Matrix.attach(A0, "pot");                 // Hybri potentiometer multiplexer attached to A0  
  Matrix.attach(A1, "button");              // Hybri button multiplexer attach to A1
  Matrix.smoothAmount = .9;                 // High amount of smoothing

  for (int i=0; i<sizeof(leds); i++) {
    pinMode(leds[i], OUTPUT);               // Set LEDs at output
  }
  for (int i=0; i<sizeof(stepToggles); i++) {
    stepToggles[i] = true;                  // Start program with all steps activated
  }
}

void loop() {
  Matrix.refresh();                              // REQUIRED TO RECEIVE UPDATED READINGS
  
  // READ BUTTONS AND UPDATE TOGGLE STATES
  for (int i=0; i<muxChannels; i++) {
    int currentState = Matrix.getReading(1, i);  // Grab readings from Hybri button mutliplexer
    if (currentState != lastButtonStates[i]) {   // If current reading isn't the same as last reading
      lastButtonStates[i] = currentState;        // Set last reading to current reading
      if (currentState) {                         
        stepToggles[i] = !stepToggles[i];        // Toggle step on/off
      }
    }
  }

  // TAKE AND MAP STEP TIME READING
  int stepTime = analogRead(stepTimePot);        
  stepTime = map(stepTime, 0, 1023, 15, 300);    

  // MIDI NOTE SENDING AND STEP ADVANCING
  if (millis() - lastTime > stepTime) {            // Check BlinkWithoutDelay Arduino example if this looks unfamiliar to you
    lastTime = millis();                         

    int seqLength = analogRead(seqLengthPot);      // It is important that this occurs within this if statement
    seqLength = map(seqLength, 0, 1023, 0, 8);
    seqLength = constrain(seqLength, 0, 7);
    
    usbMIDI.sendNoteOff(lastMidiNote, 0, 1);       // Turn the the last Note sent off
    
    digitalWrite(leds[seqPosition], LOW);          // Set the last LED off
    seqPosition++;                                 // Advance the sequence by one step
    if (seqPosition > seqLength) seqPosition = 0;  // Reset sequence if larger than sequence length
    
    if (stepToggles[seqPosition]) {
      int reading = Matrix.getReading(0, seqPosition);

      // The following code makes it so a new reading is only accepted if it's different enough. Keeps readings very steady
      if (reading > lastReading[seqPosition] + 7 || reading < lastReading[seqPosition] - 7) {
        lastReading[seqPosition] = reading;
        reading = map(reading, 0, 1023, 30, 100);
        midiNote[seqPosition] = reading;
      }
      
      usbMIDI.sendNoteOn(midiNote[seqPosition], 90, 1);
      lastMidiNote = midiNote[seqPosition];        // Retain the sent note value so that it can be turned off next sequence advance phase 
      digitalWrite(leds[seqPosition], HIGH);
    }
  }
}
































