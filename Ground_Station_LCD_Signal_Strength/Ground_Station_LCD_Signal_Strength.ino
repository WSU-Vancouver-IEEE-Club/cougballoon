#include <Wire.h>
#include <math.h>
#include <Adafruit_RGBLCDShield.h>
#include <SPI.h>
#include <SD.h>
#include <avr/sleep.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

const int analogInPin = A0;

int sensorValue = 0;
int outputValue = 0;
int colorValue = 0;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  //Print initial message, pause.
  lcd.setBacklight(TEAL);
  lcd.setCursor(0,0);
  lcd.print("#cougballoon");
  lcd.setCursor(0,1);
  lcd.print("Initializing....");
  delay(3000);
  
}

void loop() {
  
  updateDisplay();
  updateColor();
  delay(1000);
  
}

void updateColor() {
  
  switch (colorValue) {
  case 0:
    lcd.setBacklight(RED);
    break;
  case 1:
    lcd.setBacklight(YELLOW);
    break;
  case 2:
    lcd.setBacklight(VIOLET);
    break;
  case 3:
    lcd.setBacklight(BLUE);
    break;
  case 4: 
    lcd.setBacklight(TEAL);
    break;
  case 5: 
    lcd.setBacklight(GREEN);
    break;
  } 
}  

void updateDisplay() {
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Antenna Strength");
  lcd.setCursor(0,1);
  lcd.print("(0-1000): ");
  lcd.print(signalStrength());
}  
  
int signalStrength() {
  
  sensorValue = analogRead(analogInPin);            
  outputValue = map(sensorValue, 0, 1023, 0, 1000);
  colorValue = outputValue / 200;
  return outputValue;
}
