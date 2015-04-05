///////////////////////////////
// Written for Arduino UNO
// Michael Hamilton
// #cougballoon
// GPS Transmitter Board
// v1.0 Jan 9 2015
// v1.1 March 23, 2015
// v2.0 April 5, 2015 Added thermistor readings
///////////////////////////////

//Working on adding SD datalogging support for logging all temps and NMEA strings

//Serial out goes to primary transmitter

//Can only have one software serial port open at a time on UNO
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define THERMISTORPIN1 A5
#define THERMISTORNOMINAL1 109400 
#define TEMPERATURENOMINAL1 23.5
#define BCOEFFICIENT1 3950
#define SERIESRESISTOR1 9700 

#define THERMISTORPIN2 A4
#define THERMISTORNOMINAL2 109400 //Need to update 
#define TEMPERATURENOMINAL2 24 //Need to update
#define BCOEFFICIENT2 3950 // Need tp update
#define SERIESRESISTOR2 9700

#define NUMSAMPLES 5

SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);

int samples[NUMSAMPLES];

String NMEA1 = "";
String NMEA2 = "";
char c;

int CO_RawValue;
int METHANE_RawValue;

const double dV  = 0.0048828125;         // ADC step 5V/1024 4,88mV (10bit ADC)
//const double Vadc_33 = 0.0032226562;         // ADC step 3,3V/1024 3,22mV (10bit ADC)
double coRl = 5470.0; 
double coRs;
double coVrl;                                  // Output voltage
double coppm;                                  // ppm
double coratio;  
double ch4Rl = 5470.0; 
double ch4Rs;
double ch4Vrl;                                  // Output voltage
double ch4ppm;                                  // ppm
double ch4ratio; 

void calculate_CO_PPM() {
  double lgPPM;
  CO_RawValue = analogRead(0);       // read analog input pin 0
  coVrl = (double)CO_RawValue * dV;             // For 5V Vcc use Vadc_5  and  for 3V Vcc use Vadc_33
  coRs = coRl * (5 - coVrl)/coVrl; // Calculate sensor resistance
  coratio = coRs/coRl;           // Calculate ratio
  lgPPM = (log10(coratio) * -4.6)+ 0.9948;      // Calculate ppm
  //lgPPM = (log10(coratio) * -3.7)+ 0.9948;      // Calculate ppm
  coppm = pow(10,lgPPM);                        // Calculate ppm
}

void calculate_CH4_PPM() {
  double lgPPM;
  METHANE_RawValue = analogRead(1);       // read analog input pin 0
  ch4Vrl = (double)METHANE_RawValue * dV;             // For 5V Vcc use Vadc_5  and  for 3V Vcc use Vadc_33
  ch4Rs = ch4Rl * (5 - ch4Vrl)/ch4Vrl; // Calculate sensor resistance
  ch4ratio = ch4Rs/ch4Rl;           // Calculate ratio
  lgPPM = (log10(ch4ratio) * -0.9)+ 1.1;      // Calculate ppm
  ch4ppm = pow(10,lgPPM);                        // Calculate ppm
}

void setup()  
{

  Serial.begin(9600);
  GPS.begin(9600);
  //Serial.println("");
  //Serial.println("");
  //Serial.println("########################################");
  //Serial.println("#COUGBALLOON TRANSMITTER INITIALIZING...");
  //Serial.println("########################################");
  //Serial.println("");
  //Serial.println("");
  delay(1000);

  GPS.sendCommand("PGCMD,33,0*6D");
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  
  //Setup for 3.3V reference voltage
  analogReference(EXTERNAL);
  
}

void loop()  {

  //Send the air quality and temperature data
  delay(1000);
  for (int i = 0; i < 12; i++) {
    calculate_CO_PPM();
    Serial.print("F");
    Serial.println(coppm);
    delay(200);
    calculate_CH4_PPM();
    Serial.print("G");
    Serial.println(ch4ppm);
    delay(200);
    float externalTemperature = extThermistorReading();
    Serial.print("A");
    Serial.println(externalTemperature);
    delay(200);
    float internalTemperature = intThermistorReading();
    Serial.print("C");
    Serial.println(internalTemperature);
    delay(5000);
  }
  
  //Send the GPS data
  readGPS();
  delay(1000);
 
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

float extThermistorReading() {
  uint8_t i;
  float average;
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN1);
   delay(10);
  }
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
  average = 1023 / average - 1;
  average = SERIESRESISTOR1 / average;
  float steinhart;
  steinhart = average / THERMISTORNOMINAL1;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT1;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL1 + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  return steinhart;
}

float intThermistorReading() {
  uint8_t i;
  float average;
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN2);
   delay(10);
  }
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
  average = 1023 / average - 1;
  average = SERIESRESISTOR2 / average;
  float steinhart;
  steinhart = average / THERMISTORNOMINAL2;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT2;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL2 + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  return steinhart;
}
