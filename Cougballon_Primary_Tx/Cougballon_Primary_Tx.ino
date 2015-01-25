////////////////////////////////////
// Written for Teensy 3.1
// Michael Hamilton
// #cougballoon
// Ground Station Primary Transmitter
// Jan 8 2015
////////////////////////////////////

//Serial1 RX will be receiving the Air Quality/Temp Levels
//Serial2 RX will be receiving the GPS Data
//Serial3 RX will be receiving the Battery Levels

//Serial1 TX will be sending data to the radio 

String stringFromSensors = "";
String stringFromGPS = "";
String stringFromBatteries = "";

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
        
  Serial1.println("");
  Serial1.println("");
  Serial1.println("########################################");
  Serial1.println("#COUGBALLOON TRANSMITTER INITIALIZING...");
  Serial1.println("########################################");
  Serial1.println("");
  Serial1.println("");
  pinMode(13,OUTPUT);
  
  //BLINK THREE TIMES TO SHOW IT'S ON
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(100);
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(100);
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(100);
  
}

void loop() {
 
  //COMMENTED OUT FOR TESTING, ACTIVATE WHEN CONNECTING EXTERNAL uCs
  /*
  if (Serial1.available()) {
    incomingData1();
    Serial1.println(stringFromSensors);
    Serial.println(stringFromSensors);//FOR DEBUGGING
    stringFromSensors = "";
  }
  
  if (Serial2.available()) {
    incomingData2();
    Serial1.println(stringFromGPS);
    Serial.println(stringFromGPS);//FOR DEBUGGING
    stringFromGPS = "";
  }
  
  if (Serial3.available()) {
    incomingData3();
    Serial1.println(stringFromBatteries);
    Serial.println(stringFromBatteries);//FOR DEBUGGING
    stringFromBatteries = "";
  }
  */
  
  //THE FOLLOWING IS FOR TEST TRANSMISSIONS, FAKE VALUES TO TEST THE SYSTEM 
  Serial.println("SENT!");
  
  Serial1.println("A1.23");// A IS HUMIDITY
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("B2.34");// B IS CO LEVEL
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("C4.32");// C IS CH4 LEVEL
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("D84.3212312");// D IS RADIO/GPS BATTERY LEVEL (PERCENTAGE LEFT)
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("E87.321231123");// E IS HACKHD1 BATTERY LEVEL (PERCENTAGE LEFT)
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("F93.3434523452435");// F IS HACKHD2 BATTERY LEVEL (PERCENTAGE LEFT)
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("G75.2462345124531");// G IS HACKHD3 BATTERY LEVEL (PERCENTAGE LEFT)
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("I62.123412341");// I IS HACKHD4 BATTERY LEVEL (PERCENTAGE LEFT)
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("J98.99999999");// J IS HACKHD5 BATTERY LEVEL (PERCENTAGE LEFT)
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("K99.111111111");// K IS HACKHD6 BATTERY LEVEL (PERCENTAGE LEFT)
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("L58.345234");// L IS VIDEO TRANSMITTER BATTERY LEVEL (PERCENTAGE LEFT)
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("HackHD111001"); // H IS HACKHD STATUS
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("P3.45");// P IS INTERNAL PRESSURE
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("Q4.56");// Q IS EXTERNAL PRESSURE
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("T5.67");// T IS INTERNAL TEMPERATURE
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("U6.78");// U IS VIDEO TRANSMITTER TEMPERATURE
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("V7.89");// V IS EXTERNAL TEMPERATURE
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
  Serial1.println("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(2000);
   
}

void incomingData1() {
  while (Serial1.available()){
    char inChar = (char)Serial1.read(); 
    stringFromSensors += inChar;
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
    stringFromBatteries += inChar;
    delay(5); //give the serial port time to catch up
  }
}
