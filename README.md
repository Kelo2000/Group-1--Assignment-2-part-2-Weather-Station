# Indoor and Personal Weather station.

# Temperature Sensor
Group 1 - Assignment 2 
, Part 2 -  

# Objective : 
 The Weather station is the devide that showsrelevant information about the temperature and relative humidity on both the M5 Screen as well as a webpage hosted on the device



# Getting Started: 
Install M5Atom library before running your code. 

Open Arduino IDE, then Sketch>Include Library>Manage Libraries>Search "M5Atom " library>INstall

Include M5Atom library at the start of your code. 




# Functionalities:
  -In normal operation, the device is measuring temperature and recording it at a predetermined interval, and the
screen displays nothing.

  -When holding the device in the hand with the screen facing relatively upwards, pressing the screen button activates the screen.
  
  -When the screen is activated, the user is able to change modes between the following by tipping the device left or right (assuming the bottom of the device is where the USB-C port resides)

  -To exit the active display mode, the user turns the device face-down, and the device returns to background reading
  
   # MODES : 
  
  MODE 1:  Current temperature in degrees Celcius Units
  
  MODE 2: Average of last 24 hours of temperature in degrees Celcius Units
  
  MODE 3: color scale of temperature range + current temperature as color
  
  MODE 4: graph of temperature across a predefined range(Last 24 hours, each LED showing the color scale of temperature at a particular hour).
  
  MODE 5: Change units of current temperature from degrees Celcius Units to degrees Fahrenheit


