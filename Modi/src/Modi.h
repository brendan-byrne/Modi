#ifndef Modi_h
#define Modi_h
#include "Arduino.h"

    /* --- NOTES ON CONSTANTS ------------------------------------------------------   
    changePin[] and changeVal[] 
    To increase the speed that Modi boards operate I used the following two arrays
    to facilitate a reading processs that uses as few digitalWrite() commands as 
    possible. This is accomplished by using Gray Code in which the state of only
    one pin is changed each loop. changePin[] contains the pin to be changed and 
    changeVal[] designates the state to change that pin to. 
    
    muxOrganize[] and muxOrganizeSlidi[] 
    The Gray code and the traces of the PCB causes the readings of the 4051 multi-
    plexers to be read out of order. These variables arrange the readings to fit
    and be read nicely from another array. If I had made perfect circuit boards these 
    variables would not have been necessary. 
    ----------------------------------------------------------------------------   */

const byte muxChannels = 8;                                     // The 4051 multiplexer has 8 channels
const byte changePins[] = {0, 1, 0, 2, 0, 1, 0, 2};
const boolean changeVals[] = {1, 1, 0, 1, 1, 0, 0, 0};
const byte muxOrganize[] = {5, 6, 4, 7, 1, 2, 3, 0};
const byte muxOrganizeSlidi[] = {6, 5, 7, 4, 2, 1, 0, 3};

class Modi {
  public:
    Modi(byte _selPinA, byte _selPinB, byte _selPinC, byte _smoothSel);
   
    byte numMux;                // This number starts at 0 and increases by 1 with each .attach method   
    float smoothAmount;         // Sets the amount of smoothing applied to signal
    boolean smoothSel;          // Determines whether to smooth incoming signals or not
    byte inputPins[8];          // Pins connected to the analog output of each 4051
    byte selPins[3];            // Connected to digital outputs of microcontroller. Determines channel selection
    boolean slideType[8];       // Holds values that determine which organize array to apply to incoming signals
    boolean analogType[8];      // Holds values that determine whether analogRead() or digitalRead() is used
    int finalVals[8][8];
  
    void attach(byte _pin, String _digiAna);                   // Associate microcontroller pin with a Modi multiplexer
    void refresh();                                            // Capture readings from all controllers
    int getReading(byte _mux, byte _channel);                  // Access a reading from finalVals[]
    int smooth(int data, float filterVal, float smoothedVal);  // Method for smoothing incoming readings
  private:
};

#endif
