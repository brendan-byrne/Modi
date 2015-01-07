#ifndef modi_h
#define modi_h

#include "Arduino.h"

class Modi {
  public:
    Modi(byte _selPinA, byte _selPinB, byte _selPinC, byte _smoothSel);
    
    byte inputPins[8];
    byte selPins[3];
    byte muxChannels;
    byte muxShift[8];
    byte muxSlideShift[8];
    byte numMux;
    byte smoothSel;
    
    boolean muxType[8];
    boolean muxSlide[8];
    
    int smoothArray[8][8][13];
    int rawVal;
    int smoothedVal[8][8];
    float filterVal;
    
    void attach(byte _pin, String _digiAna);
    void refresh();
    int getReading(byte _mux, byte _channel);
    int digitalSmooth(int rawIn, int *sensSmoothArray);
    int readMux(int _readPin, int _channel, boolean _muxType);
    int smooth(int data, float filterVal, float smoothedVal);
  private:
  
};

#endif
