Transponder_Code
================

Code for the Arduino projector cart transponders.  This code is meant to run on an Arduino Yun with an appropriate DB9 shield.  It allows the device to be connected to a data projector, which it will poll for information.  It then reports this information back to a Crestron controller, running the PortableProjectorPackage server code.
<<<<<<< HEAD
=======


Setup Instructions
==================

Assuming you have an assembled Yun board with the DB-9 serial shield on it, you need to properly set up the WiFi to connect to your campus wireless network.  You can follow instructions on how to do this here: http://arduino.cc/en/Guide/ArduinoYun#toc13

After doing that, program the Yun by loading this code to it over USB.  You will need to set two variables before loading the code.  The first is assNum, which should be set to the asset number of the Portable Projection Package into which the unit will be installed.  The second is ident, which is a unique identifier for this transponder.  Check the Crestron server that these devices report to and find an identifier that has never been used.  They'll be the ones with blank Last_Handshake values.


Troubleshooting Instructions
============================

The on-board LED will blink at certain times and for a certain number of blinks during code execution. This list is a quick reference for what those blinks mean.

1 blink every 15 seconds - The LED will blink every 15 seconds while waiting between projector pollings to give the user feedback that code is still being properly executed.

2 blinks every 5 seconds - This indicates that the device is attempting to handshake with the server on initial power-up.  If this is happening repeatedly, it means that the transponder cannot communicate with the server.

3 blinks - Successfully sent data to the server and received a coherent reply from the server.

10 blinks - This is the error indicator.  The device encountered an error of some type during execution.  This error will usually be reported to RoomView's error log.
>>>>>>> 92bb9eae580c63fd1cca05cc75218b611a489ecb
