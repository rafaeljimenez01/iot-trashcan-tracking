#include <ArduinoJson.h>
//------------------------------------------
#include <FirebaseESP8266.h>
#include <FirebaseESP8266HTTPClient.h>
#include <FirebaseFS.h>
#include <FirebaseJson.h>

#define FIREBASE_HOST "iot-trashcan-app.firebaseio.com/"
#define FIREBASE_AUTH "sdv5W0uduPp3BdFo1dUCpEuRqVLaMuQm619lgUaA"
//------------------------------------------
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
//------------------------------------------
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11 //Definimos el tipo de sensor de humedad (DHT11 o DHT22)
//------------------------------------------

#define Trig    D0
#define Echo    D1
#define DHTPin  D3
#define LED     D5
#define TiltS   D8


//Define humidity/temperature sensor object.
DHT dht(DHTPin, DHTTYPE);


void blinkLed(uint8_t pin, int frecuency);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(TiltS, INPUT);
}

void loop() {
  float h = dht.readHumidity();  //Lectura de Humedad
  float t = dht.readTemperature(); //Lectura de Temperatura
  Serial.print("Temperature = ");
  Serial.println(t);
  Serial.print("Humidity = ");
  Serial.println(h);
  Serial.print("Used storage: ");
  Serial.print(getUsedCapacity(Trig, Echo, 50));
  Serial.println("%");
  Serial.print("Tilt value: ");
  Serial.println(getTiltVal(TiltS));

  delay(1000);
}

void blinkLed(uint8_t pin, int frecuency){
  digitalWrite(pin, HIGH);
  delay(frecuency);
  digitalWrite(pin, LOW);
  delay(frecuency);
}

/*
  getTiltVal
  Sensor: tilt switch
  Returns 0 if trashcan is in vertical position, 1 if it's horizontal.
*/
int getTiltVal(uint8_t pin) {
  return (digitalRead(pin) == 0);
}

/*
  getDistanceUS
  Sensor: ultrasonic distance sensor
  Returns distance (in cm) from the trashcan's top to the trash level.
*/
float getDistanceUS(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  float duration_us = pulseIn(echoPin, HIGH);
  float distance_cm = 0.017 * duration_us;
  return distance_cm;
}

/*
  getUsedCapacity
  Sensor: ultrasonic distance sensor
  Returns percentage of capacity in trashcan that is being used based on trashcan's height
*/
float getUsedCapacity(uint8_t trigPin, uint8_t echoPin, float height_cm) {
  float distance = getDistanceUS(trigPin, echoPin);
  return (1 - distance / height_cm) * 100;
}
