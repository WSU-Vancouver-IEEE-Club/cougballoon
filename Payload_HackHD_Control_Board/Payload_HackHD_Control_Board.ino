///////////////////////////////
// Written for Arduino UNO pins
// Michael Hamilton
// #cougballoon
// HackHD Control Board
// version 1.0 Nov 21 2014
// version 1.1 Mar 8 2015
///////////////////////////////

//Serial out goes to the primary transmitter
//The delay between each camera test is set to 5 seconds

//Initialze the pin numbers
const int HackHD1LED = 8;
const int restartHackHD1 = 2;
const int HackHD2LED = 9;
const int restartHackHD2 = 3;
const int HackHD3LED = 10;
const int restartHackHD3 = 4;
const int HackHD4LED = 11;
const int restartHackHD4 = 5;
const int HackHD5LED = 12;
const int restartHackHD5 = 6;
const int HackHD6LED = 13;
const int restartHackHD6 = 7;

//Initialize the states
int HackHD1State = 0;
int HackHD2State = 0;
int HackHD3State = 0;
int HackHD4State = 0;
int HackHD5State = 0;
int HackHD6State = 0;

unsigned long timeStart1;

//Begin the string
String cameraStatusToTransmit = "HackHD";

void setup(){

  //Initializes pins
  //Will set LED pins to inputs when needed, are outputs initially
  pinMode(HackHD1LED, INPUT);
  pinMode(restartHackHD1, OUTPUT);
  pinMode(HackHD2LED, INPUT);
  pinMode(restartHackHD2, OUTPUT);
  pinMode(HackHD3LED, INPUT);
  pinMode(restartHackHD3, OUTPUT);
  pinMode(HackHD4LED, INPUT);
  pinMode(restartHackHD4, OUTPUT);
  pinMode(HackHD5LED, INPUT);
  pinMode(restartHackHD5, OUTPUT);
  pinMode(HackHD6LED, INPUT);
  pinMode(restartHackHD6, OUTPUT);
  
  digitalWrite(restartHackHD1, HIGH);
  digitalWrite(restartHackHD2, HIGH);
  digitalWrite(restartHackHD3, HIGH);
  digitalWrite(restartHackHD4, HIGH);
  digitalWrite(restartHackHD5, HIGH);
  digitalWrite(restartHackHD5, HIGH);

  Serial.begin(9600);

  Serial.println("");
  Serial.println("");
  Serial.println("###########################################");
  Serial.println("#COUGBALLOON HACKHD MONITOR INITIALIZING...");
  Serial.println("###########################################");
  Serial.println("");
  Serial.println("");

  //Delay just to let everything settle
  delay(2000);

  //Turn each camera on
  digitalWrite(restartHackHD1, LOW);
  delay(500);
  digitalWrite(restartHackHD1, HIGH);
  digitalWrite(restartHackHD2, LOW);
  delay(500);
  digitalWrite(restartHackHD2, HIGH);
  digitalWrite(restartHackHD3, LOW);
  delay(500);
  digitalWrite(restartHackHD3, HIGH);
  digitalWrite(restartHackHD4, LOW);
  delay(500);
  digitalWrite(restartHackHD4, HIGH);
  digitalWrite(restartHackHD5, LOW);
  delay(500);
  digitalWrite(restartHackHD5, HIGH);
  digitalWrite(restartHackHD6, LOW);
  delay(500);
  digitalWrite(restartHackHD6, HIGH);

  //Initialize the timer
  timeStart1 = millis();
}

void loop(){
  //Initialize the string after each pass
  cameraStatusToTransmit = "";
  cameraStatusToTransmit = "HackHD";

  //CAMERA 1 STATE TEST
  // pinMode(HackHD1LED, INPUT);
  HackHD1State = digitalRead(HackHD1LED);
  delay(50);
  if(HackHD1State) {
    //Serial.println("HackHD1GOOD"); 
    cameraStatusToTransmit += "1";
  }
  while(!HackHD1State){
    delay(20);
    HackHD1State = digitalRead(HackHD1LED);
    if(((millis() - timeStart1) > 3000) && (!HackHD1State)){
      digitalWrite(restartHackHD1, LOW);
      delay(500);
      digitalWrite(restartHackHD1, HIGH);
      //Serial.println("HackHD1RESET");
      cameraStatusToTransmit += "0";
      timeStart1 = millis();
      break;
    }
    else if (HackHD1State) {
      //Serial.println("HackHD1GOOD");
      cameraStatusToTransmit += "1";
      timeStart1 = millis();
      break;
    }  
  }
  // pinMode(HackHD1LED, OUTPUT);
  delay(5000);


  //CAMERA 2 STATE TEST
  //  pinMode(HackHD2LED, INPUT);
  HackHD2State = digitalRead(HackHD2LED);
  delay(50);
  if(HackHD2State) {
    //Serial.println("HackHD2GOOD");
    cameraStatusToTransmit += "1";
  }
  while(!HackHD2State){
    delay(20);
    HackHD2State = digitalRead(HackHD2LED);
    if(((millis() - timeStart1) > 3000) && (!HackHD2State)){
      digitalWrite(restartHackHD2, LOW);
      delay(500);
      digitalWrite(restartHackHD2, HIGH);
      //Serial.println("HackHD2RESET");
      cameraStatusToTransmit += "0";
      timeStart1 = millis();
      break;
    }
    else if (HackHD2State) {
      //Serial.println("HackHD2GOOD");
      cameraStatusToTransmit += "1";
      timeStart1 = millis();
      break;
    }  
  }
  // pinMode(HackHD2LED, OUTPUT);
  delay(5000);


  //CAMERA 3 STATE TEST
  //  pinMode(HackHD3LED, INPUT);
  HackHD3State = digitalRead(HackHD3LED);
  delay(50);
  if(HackHD3State) {
    //Serial.println("HackHD3GOOD");
    cameraStatusToTransmit += "1";
  }
  while(!HackHD3State){
    delay(20);
    HackHD3State = digitalRead(HackHD3LED);
    if(((millis() - timeStart1) > 3000) && (!HackHD3State)){
      digitalWrite(restartHackHD3, LOW);
      delay(500);
      digitalWrite(restartHackHD3, HIGH);
      //Serial.println("HackHD3RESET");
      cameraStatusToTransmit += "0";
      timeStart1 = millis();
      break;
    }
    else if (HackHD3State) {
      //Serial.println("HackHD3GOOD");
      cameraStatusToTransmit += "1";
      timeStart1 = millis();
      break;
    }  
  }
  //  pinMode(HackHD3LED, OUTPUT);
  delay(5000);


  //CAMERA 4 STATE TEST
  // pinMode(HackHD4LED, INPUT);
  HackHD4State = digitalRead(HackHD4LED);
  delay(500);
  if(HackHD4State) {
    //Serial.println("HackHD4GOOD");
    cameraStatusToTransmit += "1";
  }
  while(!HackHD4State){
    delay(20);
    HackHD4State = digitalRead(HackHD4LED);
    if(((millis() - timeStart1) > 3000) && (!HackHD4State)){
      digitalWrite(restartHackHD4, LOW);
      delay(500);
      digitalWrite(restartHackHD4, HIGH);
      //Serial.println("HackHD4RESET");
      cameraStatusToTransmit += "0";
      timeStart1 = millis();
      break;
    }
    else if (HackHD4State) {
      //Serial.println("HackHD4GOOD");
      cameraStatusToTransmit += "1";
      timeStart1 = millis();
      break;
    }  
  }
  //  pinMode(HackHD4LED, OUTPUT);
  delay(5000);


  //CAMERA 5 STATE TEST
  // pinMode(HackHD5LED, INPUT);
  HackHD5State = digitalRead(HackHD5LED);
  delay(50);
  if(HackHD5State) {
    //Serial.println("HackHD5GOOD");
    cameraStatusToTransmit += "1";
  }
  while(!HackHD5State){
    delay(20);
    HackHD5State = digitalRead(HackHD5LED);
    if(((millis() - timeStart1) > 3000) && (!HackHD5State)){
      digitalWrite(restartHackHD5, LOW);
      delay(500);
      digitalWrite(restartHackHD5, HIGH);
      //Serial.println("HackHD5RESET");
      cameraStatusToTransmit += "0";
      timeStart1 = millis();
      break;
    }
    else if (HackHD5State) {
      //Serial.println("HackHD5GOOD");
      cameraStatusToTransmit += "1";
      timeStart1 = millis();
      break;
    }  
  }
  //  pinMode(HackHD5LED, OUTPUT);
  delay(5000);


  //CAMERA 6 STATE TEST
  //  pinMode(HackHD6LED, INPUT);
  HackHD6State = digitalRead(HackHD6LED);
  delay(50);
  if(HackHD6State) {
    //Serial.println("HackHD6GOOD");
    cameraStatusToTransmit += "1";
  }
  while(!HackHD6State){
    delay(20);
    HackHD6State = digitalRead(HackHD6LED);
    if(((millis() - timeStart1) > 3000) && (!HackHD6State)){
      digitalWrite(restartHackHD6, LOW);
      delay(500);
      digitalWrite(restartHackHD6, HIGH);
      //Serial.println("HackHD6RESET");
      cameraStatusToTransmit += "0";
      timeStart1 = millis();
      break;
    }
    else if (HackHD6State) {
      //Serial.println("HackHD6GOOD");
      cameraStatusToTransmit += "1";
      timeStart1 = millis();
      break;
    }  
  }
  //  pinMode(HackHD6LED, OUTPUT);
  delay(5000);


  //SEND THE STRING TO THE RADIO
  //Serial.print("Camera Status To Transmit: ");
  Serial.println(cameraStatusToTransmit);
}


