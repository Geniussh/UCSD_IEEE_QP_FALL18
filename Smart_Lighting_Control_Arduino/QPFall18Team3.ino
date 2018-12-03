#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // SparkFunMLX90614 Arduino library
#include <AltSoftSerial.h>
AltSoftSerial ASSserial; //Create an AltSoftSerial object to interact with Bluetooth Module
IRTherm therm; // Create an IRTherm object to interact with throughout
byte LED = 2; 
int BUTTON = 4;
char c=' ';
int function = 0;
void setup() 
{
  Serial.begin(9600); // Initialize Serial to log output
  ASSserial.begin(9600);
  therm.begin(); // Initialize thermal IR sensor
  therm.setUnit(TEMP_C); // Set the library's units to Celcius/Farenheit
  
  pinMode(LED, OUTPUT); // LED pin as output
  digitalWrite(LED,0);
}
void loop() 
{
  Serial.println("Room Temperature is " + String(therm.ambient()));
  if (ASSserial.available()){
    delay(10);
    c = ASSserial.read();
//    Serial.println(c);  //for testing
    if (c == 48) { function = 0;}
    if (c == 49) { function = 1;}
  }
//  Serial.println("The function is "+ String(function)); //for testing
  if (function == 1){
    Serial.println("Now I am detecting the temperature...");
    if (therm.read())   // Call therm.read() to read object and ambient temperatures from the sensor.On success, read() will return 1, on fail 0.
    { 
      if (therm.object() - therm.ambient() >= 3){
        digitalWrite(LED,HIGH);
        Serial.print("Object: " + String(therm.object(), 2));
        Serial.write(0xC0 |(176 >> 6));
        Serial.write(176 & 0x3F | 0x80);
        Serial.println("C");
       }
       else {
        digitalWrite(LED,LOW);
        Serial.print("Object: " + String(therm.object(), 2));
        Serial.write(0xC0 |(176 >> 6));
        Serial.write(176 & 0x3F | 0x80);
        Serial.println("C");
       }
    }
    else
      Serial.println("Unable to read the thermal value.");
  }
  else if (function == 0){
    Serial.println("Manual Mode.");
    if (digitalRead(BUTTON)==HIGH && digitalRead(LED)==HIGH){
      digitalWrite(LED,LOW);
      delay(500);
    }
    else if (digitalRead(BUTTON)==HIGH && digitalRead(LED)==LOW){
      digitalWrite(LED,HIGH);
      delay(500);
    }
  }
  delay(1000);
}
