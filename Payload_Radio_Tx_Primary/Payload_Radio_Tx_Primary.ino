#include <Adafruit_NeoPixel.h>

////////////////////////////////////
// Written for Teensy 3.1
// Michael Hamilton
// #cougballoon
// Payload Primary Transmitter
// version 1.0 Jan 8 2015
// version 1.1 Mar 8 2015
////////////////////////////////////



//Serial1 RX will be receiving the HackHD information
//Serial2 RX will be receiving the GPS Data
//Serial3 RX will be receiving the Air Quality/Temp Levels

//Serial1 TX will be sending data to the radio 

String stringFromSensors = "";
String stringFromGPS = "";
String stringFromHackHDtest = "";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial1.println("");
  Serial1.println("");
  Serial1.println("########################################");
  Serial1.println("#COUGBALLOON TRANSMITTER INITIALIZING...");
  Serial1.println("########################################");
  Serial1.println("");
  Serial1.println("");
  pinMode(13, OUTPUT);

  //BLINK THREE TIMES TO SHOW IT'S ON
  digitalWrite(13, HIGH);
  strip.setPixelColor(0, 127, 0, 0);
  strip.show();
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  strip.setPixelColor(1, 127, 127, 0);
  strip.show();
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  strip.setPixelColor(2, 0, 127, 0);
  strip.show();
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  strip.setPixelColor(3, 0, 0, 127);
  strip.show();
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  strip.setPixelColor(4, 127, 0, 127);
  strip.show();
  delay(100);
  digitalWrite(13, LOW);
  delay(100);

}

void loop() {

  if (Serial1.available()) {
    incomingData1();
    Serial1.println(stringFromHackHDtest);
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    //Serial.println(stringFromHackHDtest);//FOR DEBUGGING
    stringFromHackHDtest = "";
  }

  if (Serial2.available()) {
    incomingData2();
    Serial1.println(stringFromGPS);
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    //Serial.println(stringFromGPS);//FOR DEBUGGING
    stringFromGPS = "";
  }

  if (Serial3.available()) {
    incomingData3();
    Serial1.println(stringFromSensors);
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    //Serial.println(stringFromSensors);//FOR DEBUGGING
    stringFromSensors = "";
  }

}

void incomingData1() {
  while (Serial1.available()){
    char inChar = (char)Serial1.read(); 
    stringFromHackHDtest += inChar;
    delay(5); //give the serial port time to catch up
  }
}

void incomingData2() {
  while (Serial2.available()){
    char inChar = (char)Serial2.read(); 
    stringFromGPS += inChar;
    delay(5); //give the serial port time to catch up
  }
}

void incomingData3() {
  while (Serial3.available()){
    char inChar = (char)Serial3.read(); 
    stringFromSensors += inChar;
    delay(5); //give the serial port time to catch up
  }
}





