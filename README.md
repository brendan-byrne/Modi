# MODI - Modular Controller System#
Brendan Byrne, January 2015 <br />
[http://brendanbyrne.info](http://brendanbyrne.info) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [@bigwetdognose](https://twitter.com/bigwetdognose)

![](http://i.imgur.com/fdjiZf9.jpg)

Modi is a stackable controller bank system designed for AVR microcontroller platforms. The system is composed our four different types of modules each consisting of either 8 or 16 individually readable analog or digital controllers. Currently, a Modi system
supports up to 64 inputs utilizing 3 digital pins and an additional analog or digital pin for each set of 8 controllers. 

This is an open source project. <br /> 
(Licensing Information Here)
  
**CONSTRUCTOR**  
Modi Grid(byte selA, byte, selB, byte selC, byte smoothSelect)

- Set the arguments selA, selB, and selC to the pins that the Modi A, B, and C PCB pins are connected to. These can be any digital output pins.
- The smoothSelect argument allows you to choose from three smoothing options.
	- 0 = no smoothing
	- 1 = digitalSmooth by [Paul Badger 2007](http://tiny.cc/ycb2rx) 
	- 2 = smooth by [Paul Badger 2007](http://tiny.cc/0db2rx) 
		- You have the option of changing the amount of smoothing when selecting this option. Grid.filterVal stores this value. .0001 indicates maximum smoothing while  1 results in no smoothing. 
		- The default value is .5
 
**ATTACHING PINS**   
void Grid.attach(byte pin, String type)

- When attaching pins, start with the top most row. The library uses this to reference its position automatically.
- The first argument for the "attach" member function indicates the pin the row is connected to.
- The second argument indicates the kind of row. The Modi library recognizes the following three strings: "pot", "button", and "slide".
- The numbers for signal paths on the PCB are entirely arbitary and are intended to be used to plan *electronically*. They have no direct equivalent in this library.
    
**NEW READINGS**     
void Grid.refresh()

- Call this function at the beginning of each loop to refresh readings.
  
**GET READINGS**    
int Grid.getReading(byte row/x, byte column/y)

- Call this function to retrieve the readings from the Modi grid.
- The first reading starts at (0, 0)
- The Grid object supports a matrix of up to (7, 7)
- Pins marked as "buttons" will return a 0 or 1
- Pins marked as "pot" or "slide" will return values between 0 and 1023