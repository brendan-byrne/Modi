#include "Arduino.h"
#include "modi.h"
#define filterSamples  13

Modi::Modi(byte _selPinA, byte _selPinB, byte _selPinC, byte _smoothSel) {
  selPins[0] = _selPinA;
  selPins[1] = _selPinB;
  selPins[2] = _selPinC; 
  for (int i=0; i<sizeof(selPins); i++) pinMode(selPins[i], OUTPUT); 

  muxChannels = 8;
  numMux = 0;
  
  int muxArray[] = {4, 6, 7, 5, 3, 0, 1, 2};
  for (int i=0; i<sizeof(muxShift); i++) muxShift[i] = muxArray[i];
  
  int muxSlideArray[] = {5, 7, 6, 4, 2, 1, 0, 3};
  for (int i=0; i<sizeof(muxSlideShift); i++) muxSlideShift[i] = muxSlideArray[i];    
  
  filterVal = .5;
  
  smoothSel = _smoothSel;
  if (smoothSel > 2 || smoothSel < 0) smoothSel = 0;

  for (int i=0; i<sizeof(inputPins); i++) inputPins[i] = 0;
  for (int i=0; i<sizeof(muxType); i++) muxType[i] = 1;
}

void Modi::attach(byte _pin, String _digiAna) {
  if (_digiAna == "slide") muxSlide[numMux] = 1;
  else muxSlide[numMux] = 0;
  
  if (_digiAna == "button") muxType[numMux] = 0;
  else muxType[numMux] = 1;
 
  inputPins[numMux] = _pin;
  pinMode(inputPins[numMux], INPUT);
  numMux++;
}

void Modi::refresh() {
  for (int x=0; x<numMux; x++) {
    for (int y=0; y<muxChannels; y++) {
      if (muxSlide[x] == 1) rawVal = readMux(inputPins[x], muxSlideShift[y], muxType[x]);
      else rawVal = readMux(inputPins[x], muxShift[y], muxType[x]);
      switch (smoothSel) {
        case 0:
          smoothedVal[x][y] = rawVal;
          break;
        case 1:
          if (muxType[x] == 0) smoothedVal[x][y] = rawVal;
          else smoothedVal[x][y] = digitalSmooth(rawVal, smoothArray[x][y]);
          break;
        case 2:
          if (muxType[x] == 0) smoothedVal[x][y] = rawVal;
          else smoothedVal[x][y] =  smooth(rawVal, filterVal, smoothedVal[x][y]);
          break;
      }
    }  
  } 
}

int Modi::getReading(byte _mux, byte _channel) {
  return smoothedVal[_mux][_channel];   
}

int Modi::readMux(int _readPin, int _channel, boolean _muxType) {
  for (int i=0; i<sizeof(selPins); i++) digitalWrite(selPins[i], bitRead(_channel, i)); 
  if (_muxType == 1)   
    return analogRead(_readPin);
  else if (_muxType == 0);
    return digitalRead(_readPin); 
  
}

int Modi::digitalSmooth(int rawIn, int *sensSmoothArray) {    
  int j, k, temp, top, bottom;
  long total;
  static int i;
  static int sorted[filterSamples];
  boolean done;

  i = (i + 1) % filterSamples;    
  sensSmoothArray[i] = rawIn;                 

  for (j=0; j<filterSamples; j++){     
    sorted[j] = sensSmoothArray[j];
  }

  done = 0;               
  while(done != 1){        
    done = 1;
    for (j = 0; j < (filterSamples - 1); j++){
      if (sorted[j] > sorted[j + 1]){    
        temp = sorted[j + 1];
        sorted [j+1] =  sorted[j] ;
        sorted [j] = temp;
        done = 0;
      }
    }
  }
  
  bottom = max(((filterSamples * 15)  / 100), 1); 
  top = min((((filterSamples * 85) / 100) + 1  ), (filterSamples - 1));   
  k = 0;
  total = 0;
  for ( j = bottom; j< top; j++){
    total += sorted[j];  
    k++; 
  }
  
  return total / k;  
}

int Modi::smooth(int data, float filterVal, float smoothedVal) {
  if (filterVal > 1) filterVal = .99;
  else if (filterVal <= 0) filterVal = 0;
  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);
  return (int)smoothedVal;
}

