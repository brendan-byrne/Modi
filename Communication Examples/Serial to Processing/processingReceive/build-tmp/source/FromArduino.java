import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class FromArduino extends PApplet {

 //import the Serial library

int numMux = 2;
int muxChannels = 8;

int end = 10;    // the number 10 is ASCII for linefeed (end of serial.println), later we will look for this to break up individual messages
String serial;   // declare a new string called 'serial' . A string is a sequence of characters (data type know as "char")
Serial port;     // The serial port, this is a new instance of the Serial class (an Object)

public void setup() {
  port = new Serial(this, Serial.list()[0], 115200); // initializing the object by assigning a port and baud rate (must match that of Arduino)
  port.clear();                                      // function from serial library that throws out the first reading, in case we started reading in the middle of a string from Arduino
  serial = port.readStringUntil(end);                // function that reads the string from serial port until a println and then assigns string to our string variable (called 'serial')
  serial = null;                                     // initially, the string will be null (empty)
} 

public void draw() {
  while (port.available() > 0) {                //as long as there is data coming from serial port, read it and store it 
    serial = port.readStringUntil(end);
  }
    if (serial != null) {                       //if the string is not empty, print the following

      String[] a = split(serial, ',');          //a new array (called 'a') that stores values into separate cells (separated by commas specified in your Arduino program)
      
      for (int x=0; x<numMux; x++) {            // x<number of multiplexers you are using
        for (int y=0; y<muxChannels; y++) {
          int loc = y + x*muxChannels;
          print(a[loc]);               //print Value1 (in cell 1 of Array - remember that arrays are zero-indexed)
          print(" ");
        }
        println();
      }
    }
}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "FromArduino" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
