# README

Code for an interactive installation which implements LEDs, capacitive sensors.
Still a WIP.

## how to use

1) take a picture of the wall (eventually crop this image in order to have it ina squared shape).
2) rename it "wall.jpg" and put it inside the data folder of the Processing sketch
3) Run the sketch and press "B" to enter BACKGROUND MODE, showing the background image.
4) press "E" to enter also the EDIT MODE in order to use the mouse to drag anc move the black ball placeholder on top of "real" ball positions as swhown by the background image.
5) when done, press "B" again to exit the BACKGROUND MODE. You will see two coords arrays printed in the console window. Copy and paste these arrays inside the Arduino code.