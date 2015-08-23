#Modi Controller Series#
Xiwi Electronics - [http://xiwielectronics.com/](http://xiwielectronics.com/)  
Brendan Byrne - [http://brendanbyrne.info](http://brendanbyrne.info)  
Assembly Instructions - [http://brendanbyrne.info/Hybri-Assembly-Instructions](http://brendanbyrne.info/Hybri-Assembly-Instructions)

![](http://i.imgur.com/fdjiZf9.jpg)

**DESCRIPTION**  
Modi is a expandable controller bank system designed for AVR microcontroller platforms like Arduino and Teensy. There are four distinct module types available, each consisting of either 8 or 16 individually readable analog or digital controllers. Modi boards can be stacked to support up to 64 inputs making them ideal for MIDI/OSC projects.

**INSTALLATION**  
After downloading, move the *Modi* folder to your *Arduino Libraries* folder. Restart the Arduino IDE if already running, then open *File->Examples->Modi>allReadings* sketch.

  
**CLASSES AND METHODS**  
*CONSTRUCTOR*  
Modi Matrix(byte pcbA, byte pcbB, byte pcbC, byte toggleSmoothing)

- Set the arguments pcbA, pcbB, and pcbC to the pins that the Modi A, B, and C PCB pins are connected to. These can be any digital output pins.
- The toggleSmoothing argument allows you to turn a simple smoothing function on and off.
	- 0 = no smoothing
	- 1 = smooth by [Paul Badger 2007](http://tiny.cc/0db2rx) 
		- You have the option of changing the amount of smoothing. *Matrix.smoothingAmount* stores this value. .0001 indicates minimum smoothing while  .999 results in maximum smoothing. The default value is .5
 
*ATTACHING PINS*  
void Matrix.attach(byte pin, String type)

- Call this method in the setup() portion of your sketch.
- Start with the top most row. The library uses this to reference its position automatically.
- The pin argument indicates the pin the row is connected to.
- The second argument indicates the kind of row. The Modi library recognizes the following three strings: "pot", "button", and "slide".
- The numbers for signal paths on the PCB are entirely arbitary and are intended to be used to plan *electronically*.
    
*NEW READINGS*     
void Matrix.refresh()

- Call this function at the beginning of each loop to refresh readings.
  
*GET READINGS*    
int Matrix.getReading(byte row, byte column)

- Call this function to retrieve the readings from the Modi grid.
- (0, 0) is the top-left most controller 
- The Grid object supports a matrix of up to (7, 7)
- Pins marked as "buttons" will return a 0 or 1
- Pins marked as "pot" or "slide" will return values between 0 and 1023

**CONSTANTS AND ATTRIBUTES**  
*muxChannels*  
The number of channels the 4051 multiplexer supports. This constant's value is 8.

*Matrix.smoothAmount*  
When toggleSmoothing is activated this attribute determines the amount of smoothing applied to readings. .001 indicates minimum smoothing while .999 results in maximum smoothing. The default value is .5

*Matrix.numMux*  
numMux is equal to the number of times attach() is called in setup which should also be equal to the total number of multiplexers in your Modi system.

*Matrix.analogType[]*  
An array that stores whether analogRead() or digitalRead() is to be used when collecting readings. This attribute comes in handy when utilizing different commands with different controller types. In the case of  the midiOut example sketch contained within this library, the attribute is used to send some readings as Note On/Off messages and others as CC messages. 0 = digital and 1 = analog

**OPEN SOURCE**  
Modi is an Open-Source Hardware 1.0 Project