///////////////////////////////
// Written for Arduino UNO
// Michael Hamilton
// #cougballoon
// Air Quality Sensor Board
// Jan 9 2015
///////////////////////////////

char _int2str[7];
char* int2str( register int i ) {
  register unsigned char L = 1;
  register char c;
  register boolean m = false;
  register char b;  // lower-byte of i
  // negative
  if ( i < 0 ) {
    _int2str[ 0 ] = '-';
    i = -i;
  }
  else L = 0;
  // ten-thousands
  if( i > 9999 ) {
    c = i < 20000 ? 1
      : i < 30000 ? 2
      : 3;
    _int2str[ L++ ] = c + 48;
    i -= c * 10000;
    m = true;
  }
  // thousands
  if( i > 999 ) {
    c = i < 5000
      ? ( i < 3000
          ? ( i < 2000 ? 1 : 2 )
          :   i < 4000 ? 3 : 4
        )
      : i < 8000
        ? ( i < 6000
            ? 5
            : i < 7000 ? 6 : 7
          )
        : i < 9000 ? 8 : 9;
    _int2str[ L++ ] = c + 48;
    i -= c * 1000;
    m = true;
  }
  else if( m ) _int2str[ L++ ] = '0';
  // hundreds
  if( i > 99 ) {
    c = i < 500
      ? ( i < 300
          ? ( i < 200 ? 1 : 2 )
          :   i < 400 ? 3 : 4
        )
      : i < 800
        ? ( i < 600
            ? 5
            : i < 700 ? 6 : 7
          )
        : i < 900 ? 8 : 9;
    _int2str[ L++ ] = c + 48;
    i -= c * 100;
    m = true;
  }
  else if( m ) _int2str[ L++ ] = '0';
  // decades (check on lower byte to optimize code)
  b = char( i );
  if( b > 9 ) {
    c = b < 50
      ? ( b < 30
          ? ( b < 20 ? 1 : 2 )
          :   b < 40 ? 3 : 4
        )
      : b < 80
        ? ( i < 60
            ? 5
            : i < 70 ? 6 : 7
          )
        : i < 90 ? 8 : 9;
    _int2str[ L++ ] = c + 48;
    b -= c * 10;
    m = true;
  }
  else if( m ) _int2str[ L++ ] = '0';
  // last digit
  _int2str[ L++ ] = b + 48;
  // null terminator
  _int2str[ L ] = 0;  
  return _int2str;
}

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


// Usage example:
int i = -12345/3;
char* s;

void setup()
{
  s = int2str(i);
  Serial.begin(9600);      // sets the serial port to 9600
  
  Serial.println("");
  Serial.println("");
  Serial.println("########################################");
  Serial.println("#COUGBALLOON AIR SENSORS INITIALIZING...");
  Serial.println("########################################");
  Serial.println("");
  Serial.println("");
}

void loop()
{
  calculate_CO_PPM();// B
  //Serial.print("Carbon Monoxide (CO) level: ");
  Serial.print("B");
  Serial.println(coppm);
  //Serial.println(" ppm");
  //Serial.println(" ");
  //char* analogValueCOppm = int2str(coppm);
  //Serial.write(analogValueCOppm);
  //Serial.println(coppm);
  //Serial.print("Carbon Monoxide (CO) level: ");
  //Serial.println(analogValueCOppm);

  delay(2000);
  
  calculate_CH4_PPM();// C
  char* analogValueCH4ppm = int2str(ch4ppm);
  //Serial.print("Methane (CH4) level:        ");
  Serial.print("C");
  Serial.println(ch4ppm);
  //Serial.println(" ppm");
  //Serial.println(" ");
  //Serial.write(ch4ppm);
  
  delay(2000);

}

