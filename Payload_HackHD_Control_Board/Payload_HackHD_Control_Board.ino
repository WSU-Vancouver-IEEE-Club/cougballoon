///////////////////////////////
// Written for Arduino UNO pins
// Michael Hamilton
// #cougballoon
// HackHD Control Board
// version 1.0 Nov 21 2014
// version 1.1 Mar 8 2015
///////////////////////////////

//STILL WORKING ON IT....

//Serial out goes to the primary transmitter

//Initialze the pin numbers
const int HackHD1LED = 8;
const int HackHD1Button = 2;
const int HackHD2LED = 9;
const int HackHD2Button = 3;
const int HackHD3LED = 10;
const int HackHD3Button = 4;
const int HackHD4LED = 11;
const int HackHD4Button = 5;

//Initialize the states
int HackHD1State = 0;
int HackHD2State = 0;
int HackHD3State = 0;
int HackHD4State = 0;

unsigned long timeStart;

//Begin the string
String cameraStatusToTransmit = "HackHD";

void setup(){

  //Initializes pins
  //Will set LED pins to outputs and ground the buttons when needed, are outputs initially set to input
  pinMode(HackHD1LED, INPUT);
  pinMode(HackHD1Button, INPUT);
  digitalWrite(HackHD1Button, LOW);
  pinMode(HackHD2LED, INPUT);
  pinMode(HackHD2Button, INPUT);
  digitalWrite(HackHD2Button, LOW);
  pinMode(HackHD3LED, INPUT);
  pinMode(HackHD3Button, INPUT);
  digitalWrite(HackHD3Button, LOW);
  pinMode(HackHD4LED, INPUT);
  pinMode(HackHD4Button, INPUT);
  digitalWrite(HackHD4Button, LOW);

  Serial.begin(9600);

  //Serial.println("");
  //Serial.println("");
  //Serial.println("###########################################");
  //Serial.println("#COUGBALLOON HACKHD MONITOR INITIALIZING...");
  //Serial.println("###########################################");
  //Serial.println("");
  //Serial.println("");

  //Turn the cameras on
  pinMode(HackHD1Button, OUTPUT);
  delay(100);
  pinMode(HackHD1Button, INPUT);
  pinMode(HackHD2Button, OUTPUT);
  delay(100);
  pinMode(HackHD2Button, INPUT);
  pinMode(HackHD3Button, OUTPUT);
  delay(100);
  pinMode(HackHD3Button, INPUT);
  pinMode(HackHD4Button, OUTPUT);
  delay(100);
  pinMode(HackHD4Button, INPUT);
 
  //Initialize the timer
  timeStart = millis();
  
  //Cameras are started in Automode, need 8 seconds to start recording
  delay(8000);
}

void loop(){
  //Initialize the string after each pass
  cameraStatusToTransmit = "";
  cameraStatusToTransmit = "HackHD";

  //CAMERA 1 STATE TEST
  HackHD1State = digitalRead(HackHD1LED);
  delay(50);
  if(HackHD1State) {
    //Serial.println("HackHD1GOOD"); 
    cameraStatusToTransmit += "1";
  }
  while(!HackHD1State){
    delay(20);
    HackHD1State = digitalRead(HackHD1LED);
    if(((millis() - timeStart) > 3000) && (!HackHD1State)){
      digitalWrite(HackHD1Button, LOW);
      delay(100);
      digitalWrite(HackHD1Button, HIGH);
      //Serial.println("HackHD1RESET");
      cameraStatusToTransmit += "0";
      timeStart = millis();
      break;
    }
    else if (HackHD1State) {
      //Serial.println("HackHD1GOOD");
      cameraStatusToTransmit += "1";
      timeStart = millis();
      break;
    }  
  }
  delay(1000);//SET TO LONGER AFTER TESTING IS COMPLETE

  //COMMENTED OUT FOR QUICK TESTING
  /*
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
  delay(500);


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
  delay(500);


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
  delay(500);
  */

  //SEND THE STRING TO THE RADIO
  Serial.println(cameraStatusToTransmit);
}


