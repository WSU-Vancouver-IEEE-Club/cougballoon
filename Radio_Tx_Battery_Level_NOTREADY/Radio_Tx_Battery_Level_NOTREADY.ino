///////////////////////////////
// Written for Arduino UNO
// Michael Hamilton
// #cougballoon
// Transmitter Test
// Jan 9 2015
///////////////////////////////


#include <SoftwareSerial.h>

#define VIO 2 // Just used for the HIGH reference voltage
#define INT 3 // On 328 Arduinos, only pins 2 and 3 support interrupts
#define POL 4 // Polarity signal
#define GND 5 // Just used for the LOW reference voltage
#define CLR 6 // Unneeded in this sketch, set to input (hi-Z)
#define SHDN 7 // Unneeded in this sketch, set to input (hi-Z)

SoftwareSerial mySerial(10, 11); // RX, TX

volatile double battery_mAh = 2000.0; // milliamp-hours (mAh)
volatile double battery_percent = 100.0;  // state-of-charge (percent)

int led = 13;

volatile boolean isrflag;
volatile long int time, lasttime;
volatile double mA;
double ah_quanta = 0.17067759; // mAh for each INT
double percent_quanta; // calculate below

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Ready to transmit");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  Serial.println("test");
  pinMode(led, OUTPUT);
  
  pinMode(GND,OUTPUT); // Make this pin LOW for "ground"
  digitalWrite(GND,LOW);

  pinMode(VIO,OUTPUT); // Make this pin HIGH for logic reference
  digitalWrite(VIO,HIGH);

  pinMode(INT,INPUT); // Interrupt input pin (must be D2 or D3)

  pinMode(POL,INPUT); // Polarity input pin

  pinMode(CLR,INPUT); // Unneeded, disabled by setting to input
  
  pinMode(SHDN,INPUT); // Unneeded, disabled by setting to input
  
  percent_quanta = 1.0/(battery_mAh/1000.0*5859.0/100.0);
  
  // Enable active-low interrupts on D3 (INT1) to function myISR().
  // On 328 Arduinos, you may also use D2 (INT0), change '1' to '0'. 

  isrflag = false;
  attachInterrupt(1,myISR,FALLING);
}

void loop() // run over and over
{
  /* //Below code works, just taken out to transmit coulomb info
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  mySerial.println("test");
  Serial.println("Sent message 'test'.");
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second


  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  mySerial.println("HELLO!!!!!");
  Serial.println("Sent message 'test'.");
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  */ 
 
  static int n = 0;
  
  if (isrflag)
  {
    // Reset the flag to false so we only do this once per INT
    
    isrflag = false;

    // Print out current status (variables set by myISR())

    Serial.print("mAh: ");
    Serial.print(battery_mAh);
    Serial.print(" soc: ");
    Serial.print(battery_percent);
    Serial.print("% time: ");
    Serial.print((time-lasttime)/1000000.0);
    Serial.print("s mA: ");
    Serial.println(mA);
    mySerial.print("mAh: ");
    mySerial.print(battery_mAh);
    mySerial.print(" soc: ");
    mySerial.print(battery_percent);
    mySerial.print("% time: ");
    mySerial.print((time-lasttime)/1000000.0);
    mySerial.print("s mA: ");
    mySerial.println(mA);
  }
  
}

void myISR() // Run automatically for falling edge on D3 (INT1)
{
  static boolean polarity;
  
  // Determine delay since last interrupt (for mA calculation)
  // Note that first interrupt will be incorrect (no previous time!)

  lasttime = time;
  time = micros();

  // Get polarity value 

  polarity = digitalRead(POL);
  if (polarity) // high = charging
  {
    battery_mAh += ah_quanta;
    battery_percent += percent_quanta;
  }
  else // low = discharging
  {
    battery_mAh -= ah_quanta;
    battery_percent -= percent_quanta;
  }

  // Calculate mA from time delay (optional)

  mA = 614.4/((time-lasttime)/1000000.0);

  // If charging, we'll set mA negative (optional)
  
  if (polarity) mA = mA * -1.0;
  
  // Set isrflag so main loop knows an interrupt occurred
  
  isrflag = true;
}

