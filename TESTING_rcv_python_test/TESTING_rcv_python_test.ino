///////////////////////////////
// Written for Arduino UNO
// Michael Hamilton
// #cougballoon
// GPS Transmitter Board
// Jan 9 2015
///////////////////////////////

//SENDS NOTHING BUT GPS STRINGS

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
  Serial.println("A1.23");// A IS HUMIDITY
  delay(2000);
  //Serial.println("B2.34");// B IS CO LEVEL
  //delay(2000);
  Serial.println("C4.32");// C IS CH4 LEVEL
  delay(2000);
  //Serial.println("D84.3212312");// D IS RADIO/GPS BATTERY LEVEL (PERCENTAGE LEFT)
  //delay(2000);
  Serial.println("E87.321231123");// E IS HACKHD1 BATTERY LEVEL (PERCENTAGE LEFT)
  delay(2000);
  Serial.println("F93.3434523452435");// F IS HACKHD2 BATTERY LEVEL (PERCENTAGE LEFT)
  delay(2000);
  Serial.println("G75.2462345124531");// G IS HACKHD3 BATTERY LEVEL (PERCENTAGE LEFT)
  delay(2000);
  //Serial.println("I62.123412341");// I IS HACKHD4 BATTERY LEVEL (PERCENTAGE LEFT)
  //delay(2000);
  //Serial.println("J98.99999999");// J IS HACKHD5 BATTERY LEVEL (PERCENTAGE LEFT)
  //delay(2000);
  //Serial.println("K99.111111111");// K IS HACKHD6 BATTERY LEVEL (PERCENTAGE LEFT)
  //delay(2000);
  //Serial.println("L58.345234");// L IS VIDEO TRANSMITTER BATTERY LEVEL (PERCENTAGE LEFT)
  //delay(2000);
  //Serial.println("HackHD111001"); // H IS HACKHD STATUS
  //delay(2000);
  //Serial.println("P3.45");// P IS INTERNAL PRESSURE
  //delay(2000);
  //Serial.println("Q4.56");// Q IS EXTERNAL PRESSURE
  //delay(2000);
  //Serial.println("T5.67");// T IS INTERNAL TEMPERATURE
  //delay(2000);
  //Serial.println("U6.78");// U IS VIDEO TRANSMITTER TEMPERATURE
  //delay(2000);
  //Serial.println("V7.89");// V IS EXTERNAL TEMPERATURE
  //delay(2000);
  
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

