///////////////////////////////
// Written for Arduino UNO/Sparkfun RedBoard
// (well, 99% of it borrowed..)
// Michael Hamilton
// #cougballoon
// Pitch/Roll/Heading Board
// v1.0 Apr 5, 2015
///////////////////////////////

//Sends new data every 5 seconds

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_10DOF.h>

/* Assign a unique ID to the sensors */
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

void initSensors()
{
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    while(1);
  }
}

void setup(void)
{
  Serial.begin(9600);
  /* Initialize the sensors */
  initSensors();
}

void loop(void)
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;

  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    /* 'orientation' should have valid .roll and .pitch fields */
    Serial.print("L");
    Serial.println(orientation.roll);
    delay(300);
    Serial.print("P");
    Serial.println(orientation.pitch);
    delay(300);
  }
  
  /* Calculate the heading using the magnetometer */
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    /* 'orientation' should have valid .heading data now */
    Serial.print("Q");
    Serial.println(orientation.heading);
    delay(300);
  }

  /* Calculate the altitude using the barometric pressure sensor */
  bmp.getEvent(&bmp_event);
  if (bmp_event.pressure)
  {
    /* Get ambient temperature in C */
    float temperature;
    bmp.getTemperature(&temperature);
    
    Serial.print("T");
    Serial.println(bmp_event.pressure);
    delay(300);
    
    /* Convert atmospheric pressure, SLP and temp to altitude    */
    float altInFeet = bmp.pressureToAltitude(seaLevelPressure,
                                        bmp_event.pressure,
                                        temperature); 
    Serial.print("U");
    Serial.println(altInFeet/3.2808); 
    delay(300);
    /* Display the temperature */
    Serial.print("V");
    Serial.println(temperature);
    delay(300);
    //Serial.print(F(" C"));
  }
  
  //Serial.println(F(""));
  delay(5000);
}
