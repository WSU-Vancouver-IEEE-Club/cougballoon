///////////////////////////////
// Written for Arduino UNO pins
// Michael Hamilton
// #cougballoon
// Arduino Receiver
// (from radio, to computer)
// version 1.0 Mar 22 2015
///////////////////////////////

void setup() {
  
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  //Read from radio and send to serial monitor so Python script can handle it
  if (Serial1.available()) {
    int inByte = Serial1.read();
    Serial.write(inByte); 
  }
  
 
}

