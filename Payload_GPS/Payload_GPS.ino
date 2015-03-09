///////////////////////////////
// Written for Arduino UNO
// Michael Hamilton
// #cougballoon
// GPS Transmitter Board
// Jan 9 2015
///////////////////////////////

//SENDS NOTHING BUT GPS STRINGS

//Serial out goes to primary transmitter

//Can only have one software serial port open at a time on UNO
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);

String NMEA1 = "";
String NMEA2 = "";
char c;

void setup()  
{

  Serial.begin(9600);
  GPS.begin(9600);
  Serial.println("");
  Serial.println("");
  Serial.println("########################################");
  Serial.println("#COUGBALLOON TRANSMITTER INITIALIZING...");
  Serial.println("########################################");
  Serial.println("");
  Serial.println("");
  delay(1000);

  GPS.sendCommand("PGCMD,33,0*6D");
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  
}

void loop()  {

  readGPS();
  //SET DELAY FOR MILLISECONDS WE WANT DELAYED
  delay(2000);
  
}

void readGPS() {

  clearGPS();
  while(!GPS.newNMEAreceived()) {
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
  NMEA1=GPS.lastNMEA();

  while(!GPS.newNMEAreceived()) {
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
  NMEA2=GPS.lastNMEA();

  Serial.println(NMEA1);
  delay(2000);
  Serial.println(NMEA2);
  delay(2000);
  //Serial.println("");
}

void clearGPS() {

  while(!GPS.newNMEAreceived()) {
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA());

  while(!GPS.newNMEAreceived()) {
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA());

  while(!GPS.newNMEAreceived()) {
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
}

