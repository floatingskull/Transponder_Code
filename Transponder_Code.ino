 /* - Mobile Projector Cart Transponder Module - */
/* -       January 2014, David Cornwell       - */

/*****************
Setup Instructions
*****************/
/* Assuming you have an assembled Yun board with the DB-9 serial shield on it, you need to properly set
up the WiFi to connect to your campus wireless network.  You can follow instructions on how to do this
here: http://arduino.cc/en/Guide/ArduinoYun#toc13

After doing that, program the Yun by loading this code to it over USB.  You will need to set two
variables before loading the code.  The first is assNum, which should be set to the asset number of the
Portable Projection Package into which the unit will be installed.  The second is ident, which is a unique
identifier for this transponder.  Check the Crestron server that these devices report to and find an
identifier that has never been used.  They'll be the ones with blank Last_Handshake values.

Troubleshooting Instructions

The on-board LED will blink at certain times and for a certain number of blinks during
code execution. This list is a quick reference for what those blinks mean.

1 blink every 15 seconds - The LED will blink every 15 seconds while waiting between projector pollings 
to give the user feedback that code is still being properly executed.

2 blinks every 5 seconds - This indicates that the device is attempting to handshake with the server on 
initial power-up.  If this is happening repeatedly, it means that the transponder cannot communicate with
the server.

3 blinks - Successfully sent data to the server and received a coherent reply from the server.

10 blinks - This is the error indicator.  The device encountered an error of some type during execution.
This error will usually be reported to RoomView's error log. */


/*********************
User Editable Values
*********************/

//Set assNum to the asset number of the projector package.
const String assNum = "TEST";
<<<<<<< HEAD
//String assNum = "";
=======
>>>>>>> 92bb9eae580c63fd1cca05cc75218b611a489ecb

//Set ident to the unique identifier for this board.
const int ident = 1;

<<<<<<< HEAD
=======
//Store the IP address and port number for communicating to the Crestron processor here.
const byte serverIP[4] = { XXX, XXX, XXX, XXX };
const int serverPort = XXXX;

>>>>>>> 92bb9eae580c63fd1cca05cc75218b611a489ecb
/*********************
Global Variables and Libraries
*********************/

//Libraries!
#include <LiquidCrystal.h>
#include <YunClient.h>
#include <Process.h>

//The system will automatically change projType to 0 for old Panasonic projectors, 1 for Hitachis,
//and 2 for newer Panasonic models during projComSetup().  It will stay -1, for Debug Mode, if 
//we're not connected to one of these three types of projector.
int projType = -1;

//Feedback constants.  lcdConnected is for debugging to an lcd shield on the arduino board,
//serialReply is for debugging to the USB serial connection, connected to a computer.
const int lcdConnected = 0;
const int serialReply = 1;

//Assorted global variables.
String txString, rxString, queryPowerString, queryLampString, queryFilterString, shutdownString, startupString, errorText = "";
int powerState, lampHrs, filtHrs;
char startChar, endChar;
bool shutDownProj = false, startUpProj = false, handshake = false, projTypeConfirmed = false;

//These char array declarations accommodate Hitachi's nonsense of putting null characters into their tx strings oh my god.
const char queryPowerChars[13] = { '\xBE', '\xEF', '\x03', '\x06', '\x00', '\x19', '\xD3', '\x02', '\x00', '\x00', '\x60', '\x00', '\x00' };
const char queryLampChars[13] = { '\xBE', '\xEF', '\x03', '\x06', '\x00', '\xC2', '\xFF', '\x02', '\x00', '\x90', '\x10', '\x00', '\x00' };
const char shutdownChars[13] = { '\xBE', '\xEF', '\x03', '\x06', '\x00', '\x2A', '\xD3', '\x01', '\x00', '\x00', '\x60', '\x00', '\x00' };
const char startupChars[13] = { '\xBE', '\xEF', '\x03', '\x06', '\x00', '\xBA', '\xD2', '\x01', '\x00', '\x00', '\x60', '\x01', '\x00' };

//The maximum length of a feedback string that we'll read.
const int rxLen = 32;
//Value of the pin of the on-board LED, and the speed that we print characters to the LCD.
const int led = 13, pspd = 50;

//The delay time between loops, in 15 second increments.  Standard value should be 8.
const unsigned int delayTime = 8;

//Delay time when we turn off serial and turn on the bridge.  Maybe unnecessary.
const int comSwitchDelay = 0;
//How often, in minutes, we should reset the program.  We have to do this for stupid reasons.
const unsigned long resetMinutes = 120;
//LCD pin storing variable.
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
<<<<<<< HEAD
//Store the IP address and port number for communicating to the Crestron processor here.
//const byte serverIP[4] = { 172, 22, 144, 81 };
const byte serverIP[4] = { 172, 25, 90, 5 };
const int serverPort = 5555;
=======

>>>>>>> 92bb9eae580c63fd1cca05cc75218b611a489ecb



  /************************************************/
 /******************* setup() ********************/
/************************************************/

//The setup function, which runs at system power on, initializes all of the hardware and
//variables that we'll be using during polling.  It enables the LCD, if it's connected,
//calls the projComSetup function, then initiates a handshake with the main server to ensure
//that wifi based IP communication is working properly, and that the server is ready to accept
//polling information.

void setup() {
  //Set our blinky pin.
  pinMode(led, OUTPUT);
  //Start up the serial/USB bus, so that we can print to the debugger
  Serial.begin(9600);
  //If the lcd is connected, start it up.
  if(lcdConnected) { lcd.begin(16,2); }
  
<<<<<<< HEAD
  
  /*********************/
  //THIS IS TROUBLESHOOTING CODE, TAKE IT OUT BEFORE RELEASE
  //if(ident == 1) { assNum = "TEST"; /*projType = -1;*/ }
  //if(ident == 2) { assNum = "37030"; /*projType = 2;*/ }
  //if(ident == 3) { assNum = "18615"; /*projType = 1;*/ }
  //if(ident == 4) { assNum = "23064"; /*projType = 0;*/ }
  //if(ident == 5) { assNum = "20082"; /*projType = 1;*/ }
  /*********************/
  
  
=======
>>>>>>> 92bb9eae580c63fd1cca05cc75218b611a489ecb
  //Determine what type of projector we're connected to.
  projComSetup();
  
  //Wait until IP communications are complete before proceeding with polling.  Blink the LED
  //after every attempt to handshake.  Wait a total of 5 seconds between attempts.
  while(handshake == false) {
    Serial.print("I am board number: ");
    Serial.println(ident);
    
    if(serialReply) { Serial.println("Handshaking..."); }
    txString = "|" + String(ident) + "|" + assNum + "|" + projType + "|" + "Handshake" + "|";
    phoneHome(txString);
    blinkLED(2);
    delay(4600);
  }  
}



  /********************************************************/
 /******************** projComSetup() ********************/
/********************************************************/

//The projComSetup function will determine what type of projector we're connected
//to, automagically!  It does this by sending out the various power state queries
//at varying baud rates until it gets a legible response.  Once we determine the
//proper projType, we then load all of the serial codes appropriate for that model
//of projector.

void projComSetup() {
  char rxChar[rxLen];
  
  //Make sure to turn off whatever serial communications that might be enabled on
  //Serial1.
  Serial1.end();
  
  if(serialReply) { Serial.println("Checking projector type..."); }
  
  bool success = false;
  
  //Check with Panasonic codes at 9600 baud to see if this is an older Panasonic.
  Serial1.begin(9600);
  Serial1.println("\x02QPW\x03");
  delay(5);
  Serial1.readBytes(rxChar, rxLen);
  if(rxChar[0] == '\x02') { projType = 0; success = true; }
  Serial1.end();
  
  if(!success) {
    //Check with Hitachi codes at 19200 baud to see if it's a Hitachi.
    Serial1.begin(19200);
    for(int i = 0; i < 13; i++) {
      Serial1.print(queryPowerChars[i]);
    }
    delay(5);
    Serial1.readBytes(rxChar, rxLen);
    if(rxChar[0] == '\x1D') { projType = 1; success = true; }
  }
  
  if(!success) {
    //Check with Panasonic codes at 19200 to see if it's a new Panasonic.
    Serial1.println("\x02QPW\x03");
    delay(5);
    Serial1.readBytes(rxChar, rxLen);
    if(rxChar[0] == '\x02') { projType = 2; success = true; }
    Serial1.end();
  }
  
  //Print the projector type to debug, if we are doing debug things.
  if(serialReply) { Serial.print("Projector Type: "); Serial.println(projType); }
  
  //Set up our serial strings appropriate to whichever projector model we're
  //connected to.  Or the test strings, if that's what we're using.
  switch(projType) {
    //If we have set the projector type to old Panasonic, load the Panasonic codes.
    case 0:
      Serial1.begin(9600);
      queryPowerString = "\x02QPW\x03";
      queryLampString = "\x02Q$L\x03";
      queryFilterString = "\x02QFI:0\x03";
      shutdownString = "\x02POF\x03";
      startupString = "\x02PON\x03";
      startChar = '\x02';
      endChar = '\x03';
      break;
    //If we have set the projector type to Hitachi, the load those codes instead.
    case 1:
      Serial1.begin(19200);
      queryPowerString = "pow";
      queryLampString = "lamp";
      queryFilterString = "";
      shutdownString = "sd";
      startupString = "su";
      startChar = '\x1D';
      endChar = '\x00';
      break;
    //If we have set the projector type to new Panasonic, load the Panasonic codes.
    case 2:
      Serial1.begin(19200);
      queryPowerString = "\x02QPW\x03";
      queryLampString = "\x02Q$L\x03";
      queryFilterString = "\x02QFI:0\x03";
      shutdownString = "\x02POF\x03";
      startupString = "\x02PON\x03";
      startChar = '\x02';
      endChar = '\x03';
      break;
    //If we set the type to Debug Mode, load test codes.
    case -1:
      queryPowerString = "power?";
      queryLampString = "lamp?";
      queryFilterString = "filt?";
      shutdownString = "shutdown!";
      startupString = "startup!";
      startChar = '|';
      endChar = ';';
      break;
    //Blink the LED frantically if we somehow get the wrong projector type.
    default:
      error("Bad Proj Type", 0);
   }
}



  /*******************************************************/
 /******************** querySerial() ********************/
/*******************************************************/

//The querySerial() function sends out a string provided in the arugment,
//then waits for a response from the projector.  If one is not received 
//within 5 seconds, it calls a timeout error().

String querySerial(String tx) {
  String rx = "";
  int time, exit = 0, err = 0;
  
  //Send out our argument string if we're not querying a Hitachi.
  if(projType != 1) {
    if(projType != -1) { Serial1.println(tx); }
    else if(projType == -1) { Serial.println(tx); }
  }
  
  //Accommodate Hitachi now:
  else if(projType == 1) {
    if(tx == queryPowerString) {
      for(int i = 0; i < 13; i++) {
        Serial1.print(queryPowerChars[i]);
      }
    }
    else if(tx == queryLampString) {
      for(int i = 0; i < 13; i++) {
        Serial1.print(queryLampChars[i]);
      }
    }
    Serial1.println();
  }
  
  //Wait a bit to see if we get a reply.  If not, error.
  time = 0;
  while(exit == 0) {
    if(projType != -1) { if(Serial1.available() > 0) { exit = 1; } }
    else if(projType == -1) { if(Serial.available() > 0) { exit = 1; } }
    if(time > 1000) {
      error("No Reply", 0);
      err = 1;
      exit = 1;
    }
    time++;
    delay(10);
  }
  
  //If we didn't get an error, retrieve the information from the serial buffer.
  if(!err) {
    int startLoc = -1, endLoc = -1;
    char rxChar[rxLen];
    
    //If we're not debugging, then read from the actual serial buffer.  If we
    //are debugging (projType -1) then read from the USB serial buffer.
    if(projType != -1) { Serial1.readBytes(rxChar, rxLen); }
    else if(projType == -1) { Serial.readBytes(rxChar, rxLen); }
    
    //Find the first occurence of the start character
    for(int i = 0; i < rxLen; i++) {
      if(rxChar[i] == startChar) {
        startLoc = i;
        break;
      } 
    }
    
    //If it doesn't exist in the reply we got, error.
    if(startLoc == -1) {
      error("No Start Char", 0);
      err = 1;
      return("-1");
    }

    //Find the first occurence of the end character.
    for(int i = 0; i < rxLen; i++) {
      if(rxChar[i] == endChar) {
        endLoc = i;
        break;
      }
    }

    //If it doesn't exist in the reply we got, error.
    if(endLoc == -1) {
      error("No End Char", 0);
      err = 1;
      return("-1");
    }   

    //Put together the return string, assuming we didn't error.
    if(!err) {
      //If we're not parsing Hitachi information, do it the easy way
      if(projType != 1) {
        for(int i = startLoc + 1; i < endLoc; i++) {
          rx += rxChar[i];
        }
      }
      
      //If we're querying Hitachi power states, do this.
      if(projType == 1 && tx == queryPowerString) {
        for(int i = startLoc + 1; i < endLoc; i++) {
          rx += String(int(rxChar[i]));
        }
      }
    
      //This is Hitachi Lamp Processing NONSENSE!
      //Hitachi projectors send back lamp hours as two bytes of data,
      //with the lowbyte first and highbyte last.  Here we adjust that
      //into a nice clean decimal value.
      if(projType == 1 && tx == queryLampString) {
        if(endLoc - startLoc == 2) {
          rx = String(int(rxChar[startLoc + 1]));
        }
        else if (endLoc - startLoc > 2) {
          rx = String(int(rxChar[startLoc + 1]) + (int(rxChar[startLoc + 2]) * 256));
        }
      }
      //END OF HITACHI NONSENSE.
    }
    
    //If we didn't have an error, return the value we came up with.
    return(rx);
  }
  else {
    //Otherwise, if we DID error, return -1.
    return("-1");
  }
}
    


  /****************************************************/
 /******************** lcdWrite() ********************/
/****************************************************/

//The lcdWrite() function will print characters out on the lcd
//in a pretty fashion.  The argument is the string to be printed.

void lcdWrite(String writeString) {
  if(lcdConnected) {
    for(int i = 0; i < writeString.length(); i++) {
      lcd.write(writeString.charAt(i));
      delay(pspd);
    }
  }
}



  /*************************************************/
 /******************** error() ********************/
/*************************************************/

//The error() function will halt all processing and display an error
//message on the LCD panel, if one is connected.  If one is not, the
//funtion will blink the onboard LED instead.  This function also prints
//the error to serial.  The first argument is the error message to be 
//displayed and the second is a boolean value for whether the function 
//should return to processing after a short while, or hold processing 
//indefinitely (because of a fatal error).

void error(String writeString, int fatal) {
  int i = 0; 
  
  //Populate the error string for phoning home.
  errorText = "|" + String(ident) + "|" + assNum + "|" + projType + "|Error|" + writeString + "|";
  
  //If we're doing serial debugging, print the error to serial.
  if(serialReply) { Serial.println(errorText); }
  
  //If the lcd is connected, print the error information on that.
  if(lcdConnected) {
    lcd.clear();
    while(i < 1) {
      lcd.setCursor(0,0);
      lcdWrite("Error:");
      lcd.setCursor(0,1);
      lcdWrite(writeString);
      delay(5000);
      lcd.clear();
      if(!fatal) { i++; };
    }
  }
  
  //If the lcd isn't connected, blink the on-board LED a bit.
  else {
    i = 0; 
    while(i < 10) {
      blinkLED(1);
      //If it's not a fatal error, increment our counter so that
      //we eventually get out of this loop.
      if(!fatal) { i++; }
    }
  }
}



  /****************************************************/
 /******************** blinkLED() ********************/
/****************************************************/

//This function simply blinks the on-board LED a set number of times, as
//determined by the value received in the argument.  Each blink takes one
//fifth of a second, during which no other code is executed.
void blinkLED(int num) {
  for(int i = 0; i < num; i++) {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
  }
}



  /*****************************************************/
 /******************** phoneHome() ********************/
/*****************************************************/

//The phoneHome function will contact the server at the address specified in
//serverIP & serverPort, sending the string we built during projector polling.
//It then waits for a reply and carries on.  Because the Yun is only capable of
//using Serial1 for either the bridge or real serial communication, we've got to
//shut off our communication with the projector, then turn it back on before we
//leave the function.  The argument is the string to be transmitted to the server.

void phoneHome(String ipString) {
  //Print to USB for debugging reasons.
  if(serialReply) { Serial.println("Connecting..."); }
  
  //We have to stop serial communication on Serial1 so that we can use
  //it for IP communication.
  Serial1.end();
  
  delay(comSwitchDelay);
  
  Bridge.begin();
  
  delay(comSwitchDelay);
  
  YunClient client;
  
  client.setTimeout(250);
  
  //If we're connected, tell the USB serial we're connected, then report
  //our string back to base.
  
  if(client.connect(serverIP, serverPort)) {
    
    //We're connected!  Let the debugger know.
    if(serialReply) { Serial.println("Connected"); }
    
    //Write our transmit string through the client to the server.
    for(int i = 0; i < ipString.length(); i++) {
      client.write(ipString.charAt(i));
    }
    client.write('\r');
    client.write('\n');
    
    //Wait for the server to reply.
    int timeout = 0;
    while(client.available() == 0 && timeout < 1000) { delay(1); timeout++; }
    
    //If we got that reply, parse it out.  See if we're being told to shut down
    //or start up the projector.  Whatever we receive, print it to debug.
    if(client.available() > 0) {
      String iprx = "";
      char iprxChar[7];
      
      for(int i = 0; i < 7; i++) {
        iprxChar[i] = char(client.read());
      }
      for(int i = 0; i < 7; i++) { iprx += iprxChar[i]; }
      
      if(serialReply) { Serial.print("Received: "); Serial.println(iprx); }
      
      blinkLED(3);
      
      //Now we parse out what we got back from the server.
      //If the server tells us to startup or shutdown the projector, we'll set a global
      //boolean so that we know to do that when we've turned Serial1 back on.
      if(iprx == "startup") { startUpProj = true; }
      if(iprx == "shutdwn") { shutDownProj = true; }
      //If we got our initial handshake from the server, we know IP coms are good and we
      //can move on to polling.
      if(iprx == "handshk") { handshake = true; }
      //If the server explicitly told us to restart, we call that function with the override.
      if(iprx == "restart") { resetProg(true); }
      
      //If we just got the normal reply from the server, meaning we don't have to do anything
      //else, we'll check and see if we have to restart because we've been running to long
      //because of issues with the Yun hardware apparently.
      //if(iprx == "rxgood!") { resetProg(false); }
    
  }
    else { if(serialReply) { Serial.println("Received Nothing"); } }
  }
  else { if(serialReply) { Serial.println("Connection Failed."); } }
  
  resetProg(false);
  
  //Stop IP communications and kill the client.
  client.stop();
  
  //Re-enable serial communication on Serial so that we can poll the
  //projector again.
  switch(projType) {
    case 0: { Serial1.begin(9600); break; }
    case 1: { Serial1.begin(19200); break; }
    case 2: { Serial1.begin(19200); break; }
  }
}



  /*****************************************************/
 /******************** resetProg() ********************/
/*****************************************************/

//In this function we see how long we've been running, since the Yun seems
//to have issues with running for more than a few hours, having to do with
//string buffer overruns.  Apparently.  Whatever's causing the problem,
//we compensate for it here.  The argument is a boolean value which tells
//the function whether to reset the program regardless of how long we've
//been running.

void resetProg(bool override) {
  unsigned long timeOnline;
  bool reset = false;
  
  //Figure out how long we've been running, then print to debug if we want.
  timeOnline = millis();
  if(serialReply) { Serial.print("Online for: "); }
  if(serialReply) { Serial.print(timeOnline/60000); }
  if(serialReply) { Serial.println(" minutes."); }
  
  //If this function was called with the argument "true", we're going to
  //restart no matter what.
  if(override) {
    reset = true;
    if(serialReply) { Serial.println("Override Active"); }
  }
  //Otherwise, we're going to restart if we've run past our allotted max run
  //time, as set in resetMinutes.
  if(timeOnline > (resetMinutes * 60000)) {
    reset = true;
    if(serialReply) { Serial.println("Online too long"); }
  }
  
  //If at least one of our reset conditions was true, start up a process to
  //call the script which resets the arduino processor.
  if(reset) {
    Process restart;
    if(serialReply) { Serial.println("Restarting!"); }
    restart.runShellCommand("/usr/bin/reset-mcu");
  }
} 



  /************************************************/
 /******************** loop() ********************/
/************************************************/

//Main processing loop!  This loop will repeatedly poll the projector for it's
//power state, plus lamp and filter hours.  It checks the results for errors, then
//sends the information we've gained to a Crestron server at a specified IP address.
//If the server tells us to turn on or off the projector, that is done, then the loop
//delays until it's time to poll again.

void loop() {
  /*********************
  Initialization Section
  *********************/
  
  //A variable to track whether we make it through the whole loop without an error.
  int err = 0;
  
  //Initialize the read variables.
  powerState = 0;
  lampHrs = 0;
  filtHrs = 0;
  errorText = "";
  
  /*************************
  Projector Querying Section
  *************************/
  
  //Write "querying" on the display if it's connected.
  if(lcdConnected) { 
    lcd.setCursor(0, 0);
    lcdWrite("Querying...");
  }
  
  if(serialReply) { Serial.println("Querying..."); }
  
  //Query power state (on/off).
  powerState = querySerial(queryPowerString).toInt();
  delay(500);
  //Query lamp hours if we haven't errored.
  if(powerState != -1) { 
    lampHrs = querySerial(queryLampString).toInt(); 
  }
  else { err = 1; }
  delay(500);
  //Query filter hours if the projType isn't Hitachi and it's not new Panasonic
  //and we haven't errored.
  if(projType != 1 && projType != 2) {
    if(powerState != -1 && lampHrs != -1) { 
      filtHrs = querySerial(queryFilterString).toInt(); 
    }
    else { err = 1; }
  }
  
  /************************
  Parsing Projector's Reply
  ************************/
  
  //Some projectors report their power state as 2 or 3 when warming up or shutting
  //down.  We only care about it being on or off, so we have to deal with that here.
  if(powerState > 1) { powerState = 1; }
  
  //If we haven't errored yet, let's make sure we don't have an error in one of
  //our return values.
  if(!err) {
    if(powerState > 1 || powerState < 0) {
      error("Bad Pwr Value", 0);
      err = 1;
      if(serialReply) { Serial.println(powerState); }
    }
    if(lampHrs < 0) {
      error("Bad Lmp Value", 0);
      err = 1;
      if(serialReply) { Serial.println(lampHrs); }
    }
    if(filtHrs < 0) {
      error("Bad Flt Value", 0);
      err = 1;
      if(serialReply) { Serial.println(filtHrs); }
    }
  }
  
  /*************************************
  Sending Projector's Info to the Server
  *************************************/
  
  //If we still, somehow, haven't had an error, let's put together our report string.
  if(!err) {
    txString = "|" + String(ident) + "|" + assNum + "|" + projType + "|" + powerState + "|" + lampHrs + "|" + filtHrs + "|";
    
    //Since we didn't have an error, we know that we have the correct projType.
    projTypeConfirmed = true;
    
    //Let the user know that we're reporting info to the server.
    if(lcdConnected) {     
      lcd.clear();
      lcd.setCursor(0, 0);
      lcdWrite("Reporting...");
    }
    if(serialReply) { Serial.println("Reporting..."); }
    
    //PHONE HOME!
    phoneHome(txString);
    
    //Print the string to serial if we're doing serial things.
    if(serialReply) { Serial.println(txString); }
    //Print the string to the lcd if it's connected.
    if(lcdConnected) {     
      lcd.clear();
      lcd.setCursor(0, 0);
      lcdWrite(txString);
      delay(3000);
      
      //Scroll it off to the side in a fancy way.
      for (int pos = 0; pos < (txString.length()); pos++) {
        lcd.scrollDisplayLeft(); 
        delay(150);
      }
      
      //Wait a bit, then clear the screen.
      delay(350);
      lcd.clear();
    }
  }
  else if(err) {
    //We had an error, send our error text back to the server.
    phoneHome(errorText);
    
    //Since we had an error, we should check to make sure we've got the
    //correct projType, assuming we haven't already confirmed that it's
    //correct.
    if(!projTypeConfirmed) { projComSetup(); }
  }
  
  /************************************************************
  Executing Projector Startup/Shutdown Commands from the Server
  ************************************************************/
  
  //Handle any shutdown/startup requests which we may have received.
  if(startUpProj && powerState == 0) {
    if(serialReply) { Serial.println("Starting Up Projector..."); }
    
    switch(projType) {
      case 0: { 
        //Startup for old Panasonics.
        querySerial(startupString); 
        startUpProj = false; 
        break; 
      }
      case 1: { 
        //Startup for Hitachis.
        for(int i = 0; i < 13; i++) {
          Serial1.print(startupChars[i]);
        }
        Serial1.println();
        startUpProj = false;
        break;
      }
      case 2: { 
        //Startup for new Panasonics.
        querySerial(startupString); 
        startUpProj = false; 
        break; 
      }
    }
  }

  if(shutDownProj && powerState == 1) {
    if(serialReply) { Serial.println("Shutting Down Projector..."); }
    
    switch(projType) {
      case 0: { 
        //Shutdown for old Panasonics.
        querySerial(shutdownString); 
        shutDownProj = false;
        break; 
      }
      case 1: { 
        //Shutdown for Hitachis.
        for(int i = 0; i < 13; i++) {
          Serial1.print(shutdownChars[i]);
        }
        Serial1.println();
        shutDownProj = false;
        break;
      }
      case 2: { 
        //Shutdown for new Panasonics.
        querySerial(shutdownString);
        shutDownProj = false;
        break; 
      }
    }
  }
  
  /****************************
  Waiting to Run the Loop Again
  ****************************/
  
  //In this for loop, we'll delay processing in 15 second chunks and
  //continue after we've completed a number of these chunks equal to
  //the delayTime constant.  Once every 15 seconds, we blink the LED to
  //let people know we're alive.
  for(int wait = 0; wait < delayTime; wait++) {
    //Make sure we let some poor debugger know what board number we are...
    //We will always print this info, even if serialReply is false, because it's the
    //only way to know for sure what ident a board has been programmed with.  Users
    //can connect with PuTTY to get this info.
    Serial.print("I am board number: ");
    Serial.println(ident);
    if(serialReply) { Serial.println("Waiting..."); }
    
    //If we have an lcd, we'll wait by printing stuff to it, giving
    //the user feedback and making debugging easier.
    if(lcdConnected) {
      lcd.setCursor(0, 0);
      lcdWrite("Waiting");
      
      for(int i = 0; i < 15; i++) {
        lcd.write(".");
        delay(1000);
        if(i == 8) { lcd.setCursor(0, 1); }
      }
      
      lcd.clear();
    }
    //Otherwise, we just wait for delayTime number of seconds.
    else { delay(15000); }
    
    //Blink the LED once to show that we're still running code.
    blinkLED(1);
  }

  /*********************************
  We made it to the end of the loop!
  *********************************/
}





/******************** Serial Strings! ********************/

/* 

Useful Strings for Panasonic Projectors

\x02PON\x03 - Power Turn On
\x02POF\x03 - Power Turn Off
\x02QPW\x03 - Query Power State
\x02Q$L\x03 - Query Lamp Hours
\x02QFI:0\x03 - Query Filter Hours

\x02000\x03 - Power Is Off Reply
\x02001\x03 - Power Is On Reply
Replies to lamp and filter hour queries are \x02#\x03


Useful Strings for Hitachi Projectors

\xBE\xEF\x03\x06\x00\xBA\xD2\x01\x00\x00\x60\x01\x00 - Power Turn On
\xBE\xEF\x03\x06\x00\x2A\xD3\x01\x00\x00\x60\x00\x00 - Power Turn Off
\xBE\xEF\x03\x06\x00\x19\xD3\x02\x00\x00\x60\x00\x00 - Query Power State
\xBE\xEF\x03\x06\x00\xC2\xFF\x02\x00\x90\x10\x00\x00 - Query Lamp Hours

\x1D\x00\x00 - Power Is Off Reply
\x1D\x01\x00 - Power Is On Reply

*/
