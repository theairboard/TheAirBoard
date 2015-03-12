// This sample code demonstrates the use of a TinyGPS object on TheAirBoard prototype board.

// TinyGPS - a small GPS library from Mikal Hart
#include <TinyGPS.h>

#include <SoftwareSerial.h>

TinyGPS gps;

SoftwareSerial Uart(A5,A4);

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);

int ONOFF = 11;
int LED   = 13;
boolean DataReceived = false;

void setup()
{
  digitalWrite(ONOFF,LOW);
  pinMode(ONOFF, OUTPUT);
  digitalWrite(LED,LOW);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Uart.begin(4800);
  delay(1000);
  Serial.println("GPS Hornet Nano Demo started");
  digitalWrite(LED,HIGH);
  digitalWrite(ONOFF,HIGH);
  delay(100);
  digitalWrite(ONOFF,LOW);
}

void loop()
{
  bool newdata = false;
  DataReceived = false;
  unsigned long start = millis();

  // Every 5 seconds we print an update
  while (millis() - start < 5000) {
    if (Uart.available()) {
      DataReceived = true;
      char c = Uart.read();
      //Serial.print(c);  // uncomment to see raw GPS data
      if (gps.encode(c)) {
        newdata = true;
        // break;  // uncomment to print new data immediately!
      }
    }
  }
  if (!DataReceived) { // Resync the start of the GPS
    Serial.println("Wake UP the device");
    digitalWrite(LED,LOW);
    digitalWrite(ONOFF,HIGH);
    delay(100);
    digitalWrite(ONOFF,LOW);
  } 
  else {
    digitalWrite(LED,HIGH);
    if (newdata) {
      Serial.println("Acquired Data");
      Serial.println("-------------");
      gpsdump(gps);
      Serial.println("-------------");
      Serial.println();
      Serial.println("Enter in standby mode for 10 seconds");
      digitalWrite(ONOFF,HIGH);
      delay(100);
      digitalWrite(ONOFF,LOW);
      unsigned long start = millis();
      delay(10000);
      Serial.println("Wake UP the device");
      digitalWrite(ONOFF,HIGH);
      delay(100);
      digitalWrite(ONOFF,LOW);
    } 
    else Serial.print(".");
  }
}

void gpsdump(TinyGPS &gps)
{
  long lat, lon;
  float flat, flon;
  unsigned long age, date, time, chars;
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned short sentences, failed;

  gps.f_get_position(&flat, &flon, &age);
  Serial.print("Lat/Long: "); 
  printFloat(flat, 5); 
  Serial.print(", "); 
  printFloat(flon, 5);
  Serial.print(" Fix age: "); 
  Serial.print(age); 
  Serial.println("ms.");

  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  Serial.print("Date: "); 
  Serial.print(static_cast<int>(month)); 
  Serial.print("/"); 
  Serial.print(static_cast<int>(day)); 
  Serial.print("/"); 
  Serial.print(year);
  Serial.print("  Time: "); 
  Serial.print(static_cast<int>(hour)); 
  Serial.print(":"); 
  Serial.print(static_cast<int>(minute)); 
  Serial.print(":"); 
  Serial.print(static_cast<int>(second));
  Serial.print("."); 
  Serial.print(static_cast<int>(hundredths));
  Serial.print("  Fix age: ");  
  Serial.print(age); 
  Serial.println("ms.");

  Serial.print("Alt: ");     
  printFloat(gps.f_altitude()); 
  Serial.print(" Course: "); 
  printFloat(gps.f_course());
  Serial.print(" Speed: ");   
  printFloat(gps.f_speed_kmph()); 
  Serial.println("Km/h");

  gps.stats(&chars, &sentences, &failed);
  Serial.print("Stats: characters: "); 
  Serial.print(chars); 
  Serial.print(" sentences: ");
  Serial.print(sentences); 
  Serial.print(" failed checksum: "); 
  Serial.println(failed);
}

void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0) {
    Serial.print('-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    Serial.print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint;
  }
}



