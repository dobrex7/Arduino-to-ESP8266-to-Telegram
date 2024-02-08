#include<SoftwareSerial.h> // knjiznica za serijsko komunikacijo (z esp8266)
#include <dht11.h> // knjiznica za temperaturo in vlago

#define DHT11PIN 8 // definiramo dht11 vhod na 8
dht11 DHT11;

SoftwareSerial Arduino_SoftSerial(10, 11); //10 povezano na D2, 11 povezano na D1
String dataIn;

void setup() {
  Serial.begin(115200); //inicializiramo serijsko komunikacijo na vrednosti 115200
  Arduino_SoftSerial.begin(9600); // inicializiramo serijsko komunikacijo za posiljanje iz arduina na esp8266, na vrednosti 9600
}

void loop() { 
  int chk = DHT11.read(DHT11PIN); // branje iz senzorja

  Serial.print("Vlaznost (%): ");
  Serial.println((float)DHT11.humidity); // izpis vlage na serijski zaslon

  Serial.print("Temperatura (C): ");
  Serial.println((float)DHT11.temperature); // izpis temperature na serijski zaslon

  delay(3000); // 3 sekunde premora

  // Posiljanje podatkov na ESP8266, vlaznost in temperaturo
  Arduino_SoftSerial.print("Vlaznost (%): "); 
  Arduino_SoftSerial.print((float)DHT11.humidity);
  Arduino_SoftSerial.print(',');

  Arduino_SoftSerial.print("Temperatura (C): ");
  Arduino_SoftSerial.print((float)DHT11.temperature);
  Arduino_SoftSerial.println(); 
}

