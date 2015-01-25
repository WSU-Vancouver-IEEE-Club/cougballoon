///////////////////////////////
// Written for Arduino Mega w/ Ethernet Shield
// Michael Hamilton
// #cougballoon
// Ground Station Primary Receiver
// Jan 8 2015
///////////////////////////////

//MUST SET LABELS AND STUFF MANUALLY IN PLOTLY ONCE GRAPHS STARTS
//NEED TO PLAY WITH MAXPOINTS TO TEST WHAT LOOKS BEST

//ADD TRANSMIT PORTION? MAYBE FOR CUTDOWN? OR FOR CHANGING VIEW OF CAMERAS?

//NEED TO DECIDE WHAT INFO GOES TO WHAT GRAPH AND ACTIVATE IT IN CODE BELOW

#include <SoftwareSerial.h>
#include "plotly_streaming_ethernet.h"
#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>
#include <Time.h>

// View your API key and streamtokens here: https://plot.ly/settings
#define nTracesA 8 //BATTERY LEVELS
#define nTracesB 2 //AIR QUALITY
#define nTracesC 3 //TEMPERATURES
//ADD MORE??

/*
Not all the functions are listed on the main SD library page, 
because they are part of the library's utility functions.

The communication between the microcontroller and the SD 
card uses SPI, which takes place on digital pins 11, 12, 
and 13 (on most Arduino boards) or 50, 51, and 52 (Arduino Mega). 
Additionally, another pin must be used to select the SD card. 
This can be the hardware SS pin - pin 10 (on most Arduino boards) 
or pin 53 (on the Mega) - or another pin specified in the call 
to SD.begin(). Note that even if you don't use the hardware SS 
pin, it must be left as an output or the SD library won't work. 
Different boards use different pins for this functionality, so 
be sure you’ve selected the correct pin in SD.begin().

On the Ethernet Shield, CS is pin 4. Note that even if it's not
used as the CS pin, the hardware CS pin (10 on most Arduino boards,
53 on the Mega) must be left as an output or the SD library functions will not work.
*/
const int chipSelect = 4;

File dataFile;

byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0xB0, 0x25};
// the dns server ip
IPAddress dnServer(75, 75, 75, 75);
// the router's gateway address:
IPAddress gateway(10, 0, 0, 1);
// the subnet:
IPAddress subnet(255, 255, 255, 0);

//STILL PLAYING WITH THESE...
//byte my_ip[] = {76,115,75,114};// FOR HOME NETWORK
//byte my_ip[] = {166,171,249,250};
byte my_ip[] = {10,0,0,99};
//byte my_ip[] = {192,168,1,3};
//byte my_ip[] = {192,168,1,143};//THIS ONE WORKED ONCE.....

char *tokens1[nTracesA] = {"nf4u8crau4", "tyoekcb695", "85tk2ys77r", "2nznlhyjuy", "hveclaset9", "tjojhtubne", "ql5jh44r2f", "lfyq2b2qep"};
char *tokens2[nTracesB] = {"dzl1r1ik7l", "kpjiisg7dy"};
char *tokens3[nTracesC] = {"y9vwycvseh", "l6pq0nuui0", "1650czs560"};

//ADDED TO TRY NEW CODE FROM CHRIS
char *traces1[nTracesA] = {"Radio/GPS", "HackHD1", "HackHD2", "HackHD3", "HackHD4", "HackHD5", "HackHD6", "Video Tx"};
char *traces2[nTracesB] = {"CO", "CH4"};
char *traces3[nTracesC] = {"Internal", "External", "Video Tx"};

// arguments: username, api key, streaming token, filename
plotly graph1 = plotly("michael.l.hamilton", "cxrph8m4eh", "#cougballoon battery levels", tokens1, traces1, "cougballoon battery levels", nTracesA);
plotly graph2 = plotly("michael.l.hamilton", "cxrph8m4eh", "#cougballoon air quality", tokens2, traces2, "cougballoon air quality", nTracesB);
plotly graph3 = plotly("michael.l.hamilton", "cxrph8m4eh", "#cougballoon temperatures", tokens3, traces3, "cougballoon temperatures", nTracesC);


void startEthernet(){
  Serial.println("... Initializing ethernet");
  if(Ethernet.begin(mac) == 0){
    Serial.println("... Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, my_ip);
    //Ethernet.begin(mac, my_ip, dnServer, gateway, subnet);
  }
  //Ethernet.begin(mac, my_ip); //ADDED TO TEST
  Serial.println("... Done initializing ethernet");
  delay(1000);
}


String stringFromBalloon = "";
String dateStringFromGPS = "";
String separator = "||||";
boolean stringFromBalloonComplete = false;//DO I REALLY NEED THIS?
float x = 0.0;
char floatbuf[10];

void setup() {
  Serial.begin(9600); 
  Serial1.begin(9600);
  Serial2.begin(9600);//Serial 2 sends NMEA to a seperate arduino/computer for processing
  
  stringFromBalloon.reserve(200);
  
  Serial.println("");
  Serial.println("");
  Serial.println("#####################################");
  Serial.println("#COUGBALLOON RECEIVER INITIALIZING...");
  Serial.println("#####################################");
  Serial.println("");
  Serial.println("");
  /*
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("SD card initialized.");
  // open a new file and immediately close it:
  Serial.println("Creating balloon.txt..."); //COMMENTED OUT FOR TESTING!!!!!
  /* FAT file systems have a limitation when it comes to naming conventions. 
  You must use the 8.3 format, so that file names look like “NAME001.EXT”, 
  where “NAME001” is an 8 character or fewer string, and “EXT” is a 3 
  character extension. People commonly use the extensions .TXT and .LOG. It 
  is possible to have a shorter file name (for example, mydata.txt, or time.log), 
  but you cannot use longer file names. Read more on the 8.3 convention. */
  //dataFile = SD.open("balloon.txt", FILE_WRITE);
  //dataFile.close();
  // Check to see if the file exists: 
  /*
  if (SD.exists("balloon.txt")) {
    Serial.println("File balloon.txt exists.");
  }
  else {
    Serial.println("File balloon.txt doesn't exist.");
    // open a new file and immediately close it:
    Serial.println("Creating balloon.txt...");
    dataFile = SD.open("balloon.txt", FILE_WRITE);
    dataFile.close();
    Serial.println("File balloon.txt exists.");
  }
  
  dataFile = SD.open("balloon.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("Error opening balloon.txt");
    // Wait forever since we cant write data
    while (1) ;
  }
  dataFile.close();
*/
  startEthernet(); //THIS STARTS THE INTERNET WORKING
  
  //THIS SETS UP THE PLOTLY GRAPHS
  bool success1, success2, success3;
  success1 = graph1.init();
  if(!success1){while(true){}}
  success2 = graph2.init();
  if(!success2){while(true){}}
  success3 = graph3.init();
  if(!success3){while(true){}}
  graph3.openStream();
  //NO NEED TO OPEN ALL STREAMS, STILL WORKS.
  //https://gist.github.com/chriddyp/11222798

}


void loop() {
  
  if (Serial1.available()) {
    stringFromBalloon = "";
    stringFromBalloonComplete = false;
    incomingData();

    //dataFile = SD.open("balloon.txt", FILE_WRITE);
    //THIS WILL SAVE EACH INCOMING STRING TO THE SD CARD ON ETHERNET SHIELD
    //dataFile.println(stringFromBalloon + separator + dateStringFromGPS);
    //dataFile.println(stringFromBalloon);
    //dataFile.close();
    
    if (stringFromBalloon.startsWith("A")) {
      stringFromBalloon = stringFromBalloon.substring(1,6);//Fix second digit for substring lengths depending on what's needed for each.
      x = stringToInteger();
      Serial.print("The humidity is: ");
      Serial.print(x);
      Serial.println(" (insert humidity units here)");
      //graph.plot(millis(), x, tokens[0]); 
    }
    
    else if (stringFromBalloon.startsWith("B")) {
      stringFromBalloon = stringFromBalloon.substring(1,6);
      x = stringToInteger();
      Serial.print("The CO value is: ");
      Serial.print(x);
      Serial.println(" ppm");
      graph2.plot(millis(), x, tokens2[0]); 
    }
    
    else if (stringFromBalloon.startsWith("C")) {
      stringFromBalloon = stringFromBalloon.substring(1,6);
      x = stringToInteger();
      Serial.print("The CH4 value is: ");
      Serial.print(x);
      Serial.println(" ppm");
      graph2.plot(millis(), x, tokens2[1]); 
    }
    
    else if (stringFromBalloon.startsWith("D")) {
      stringFromBalloon = stringFromBalloon.substring(1,6);
      x = stringToInteger();
      Serial.print("Radio/GPS battery level: ");
      Serial.print(x);
      Serial.println(" % remaining");
      graph1.plot(millis(), x, tokens1[0]); 
    }
    
    else if (stringFromBalloon.startsWith("E")) {
      stringFromBalloon = stringFromBalloon.substring(1,6);
      x = stringToInteger();
      Serial.print("HackHD 1 battery level: ");
      Serial.print(x);
      Serial.println(" % remaining");
      graph1.plot(millis(), x, tokens1[1]); 
    }
    
    else if (stringFromBalloon.startsWith("F")) {
      stringFromBalloon = stringFromBalloon.substring(1,6);
      x = stringToInteger();
      Serial.print("HackHD 2 battery level: ");
      Serial.print(x);
      Serial.println(" % remaining");
      graph1.plot(millis(), x, tokens1[2]); 
    }
    
    else if (stringFromBalloon.startsWith("G")) {
      stringFromBalloon = stringFromBalloon.substring(1,6);
      x = stringToInteger();
      Serial.print("HackHD 3 battery level: ");
      Serial.print(x);
      Serial.println(" % remaining");
      graph1.plot(millis(), x, tokens1[3]); 
    }
    
    else if (stringFromBalloon.startsWith("I")) {
      stringFromBalloon = stringFromBalloon.substring(1,6);
      x = stringToInteger();
      Serial.print("HackHD 4 battery level: ");
      Serial.print(x);
      Serial.println(" % remaining");
      graph1.plot(millis(), x, tokens1[4]); 
    }
    
    else if (stringFromBalloon.startsWith("J")) {
      stringFromBalloon = stringFromBalloon.substring(1,6);
      x = stringToInteger();
      Serial.print("HackHD 5 battery level: ");
      Serial.print(x);
      Serial.println(" % remaining");
      graph1.plot(millis(), x, tokens1[5]); 
    }
    
    else if (stringFromBalloon.startsWith("K")) {
      stringFromBalloon = stringFromBalloon.substring(1,6);
      x = stringToInteger();
      Serial.print("HackHD 6 battery level: ");
      Serial.print(x);
      Serial.println(" % remaining");
      graph1.plot(millis(), x, tokens1[6]); 
    }
    
    else if (stringFromBalloon.startsWith("L")) {
      stringFromBalloon = stringFromBalloon.substring(1,6);
      x = stringToInteger();
      Serial.print("Video Transmitter battery level: ");
      Serial.print(x);
      Serial.println(" % remaining");
      graph1.plot(millis(), x, tokens1[7]); 
    }
    
    else if (stringFromBalloon.startsWith("H")) {
      //HOW DO WE DISPLAY THIS ON WEBSITE?
      Serial.print("HackHD Status: ");
      Serial.print(stringFromBalloon);
      if (stringFromBalloon.substring(6,7) == "1") {
        Serial.println("HackHD 1 is recording");
      }
      else {
        Serial.println("HackHD 1 is NOT recording");
      }
      
      if (stringFromBalloon.substring(7,8) == "1") {
        Serial.println("HackHD 2 is recording");
      }
      else {
        Serial.println("HackHD 2 is NOT recording");
      }
      
      if (stringFromBalloon.substring(8,9) == "1") {
        Serial.println("HackHD 3 is recording");
      }
      else {
        Serial.println("HackHD 3 is NOT recording");
      }
      
      if (stringFromBalloon.substring(9,10) == "1") {
        Serial.println("HackHD 4 is recording");
      }
      else {
        Serial.println("HackHD 4 is NOT recording");
      }
      
      if (stringFromBalloon.substring(10,11) == "1") {
        Serial.println("HackHD 5 is recording");
      }
      else {
        Serial.println("HackHD 5 is NOT recording");
      }
      
      if (stringFromBalloon.substring(11,12) == "1") {
        Serial.println("HackHD 6 is recording");
      }
      else {
        Serial.println("HackHD 6 is NOT recording");
      }
    }
    
    else if (stringFromBalloon.startsWith("P")) {
      stringFromBalloon = stringFromBalloon.substring(1,9);//9 digits for pressure
      x = stringToInteger();
      Serial.print("The internal pressure is: ");
      Serial.print(x);
      Serial.println(" kPa");
      //graph.plot(millis(), z, tokens[1]); 
    }
    
    else if (stringFromBalloon.startsWith("Q")) {
      stringFromBalloon = stringFromBalloon.substring(1,9);//9 digits for temperature
      x = stringToInteger();
      Serial.print("The external pressure is: ");
      Serial.print(x);
      Serial.println(" kPa");
      //graph.plot(millis(), z, tokens[1]); 
    }
    
    else if (stringFromBalloon.startsWith("T")) {
      stringFromBalloon = stringFromBalloon.substring(1,9);//9 digits for pressure.
      x = stringToInteger();
      Serial.print("The internal temperature is: ");
      Serial.print(x);
      Serial.println(" C (or F)");
      graph3.plot(millis(), x, tokens3[0]); 
    }
    
    else if (stringFromBalloon.startsWith("U")) {
      stringFromBalloon = stringFromBalloon.substring(1,9);//9 digits for temperature
      x = stringToInteger(); 
      Serial.print("The video transmitter temperature is: ");
      Serial.print(x);
      Serial.println(" C (or F)");
      graph3.plot(millis(), x, tokens3[1]); 
    }
    
    else if (stringFromBalloon.startsWith("V")) {
      stringFromBalloon = stringFromBalloon.substring(1,9);//9 digits for temperature
      x = stringToInteger();
      Serial.print("The external temperature is: ");
      Serial.print(x);
      Serial.println(" C (or F)");
      graph3.plot(millis(), x, tokens3[2]); 
    }
    
    else if (stringFromBalloon.startsWith("$")) {
      //if (stringFromBalloon.startsWith("$GPGGA")) {
        //COPYS TIME FROM GPS STRING
        //dateStringFromGPS = stringFromBalloon.substring(7,13);
      //}
      Serial.println(stringFromBalloon);
      //Serial2.write(Serial1.read());//WILL THIS WORK? NEED TO TEST
      //Serial2.write(stringFromBalloon);//OR DO IT LIKE THIS?
    }
  }
  
  /*
  //Works perfectly for NMEA strings IF NEEDED
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }*/
  
}

float stringToInteger () {
  stringFromBalloon.toCharArray(floatbuf, sizeof(floatbuf));
  float a = atof(floatbuf);
  stringFromBalloon = "";
  return a;
}  

void incomingData() {
  while (Serial1.available() && (stringFromBalloonComplete == false)){
    char inChar = (char)Serial1.read(); 
    stringFromBalloon += inChar;
    delay(5);// give the serial port time to catch up
    //DO I NEED THE PART BELOW??
    if (inChar == '\n'){
      //Serial.print("incoming string: "); Serial.println(stringFromBalloon);
      stringFromBalloonComplete = true;
    }  
  }
}

