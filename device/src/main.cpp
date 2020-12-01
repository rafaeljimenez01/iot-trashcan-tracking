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


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//Define humidity/temperature sensor object.
DHT dht(DHTPin, DHTTYPE);

//Define FirebaseESP8266 data object for data sending and receiving
FirebaseData firebaseData;


void blinkLed(uint8_t pin, int frecuency);
int getTiltVal(uint8_t pin);
float getDistanceUS(uint8_t trigPin, uint8_t echoPin);
float getUsedCapacity(uint8_t trigPin, uint8_t echoPin, float heigh_cm);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(TiltS, INPUT);
  
  wifiConnect();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  String id_container = WiFi.macAddress();
  float longi = 20.731615;
  float alt = -103.454605;
  Firebase.pushString(firebaseData, "container/id_container", id_container);  
  Firebase.pushFloat(firebaseData, "container/longitude", longi); 
  Firebase.pushFloat(firebaseData, "container/latitude", alt); 
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:

  float h = dht.readHumidity();  //Lectura de Humedad
  float t = dht.readTemperature(); //Lectura de Temperatura
  String id_container = WiFi.macAddress();
  
  Serial.print("Temperature = ");
  Serial.println(t);
  Serial.print("Humidity = ");
  Serial.println(h);

  // Demostración de inclinación y ultrasónico:
  Serial.print("Used storage: ");
  Serial.print(getUsedCapacity(Trig, Echo, 50));
  Serial.println("%");
  Serial.print("Tilt value: ");
  Serial.println(getTiltVal(TiltS));
  
  Firebase.pushString(firebaseData, "DHT/id_container", id_container);  
  Firebase.pushFloat(firebaseData, "DHT/temperatura", t);  
  Firebase.pushFloat(firebaseData, "DHT/humidity", h);

  Firebase.pushString(firebaseData, "ultra/id_container", id_container);  
  Firebase.pushFloat(firebaseData, "ultra/distance", getDistanceUS(Trig, Echo));  
  Firebase.pushFloat(firebaseData, "ultra/percentageUsed", getUsedCapacity(Trig, Echo, 50)); 

  Firebase.pushString(firebaseData, "tilt/id_container", id_container);  
  Firebase.pushFloat(firebaseData, "tilt/tilted", getTiltVal(TiltS));  
  
    

  if(WiFi.status() != WL_CONNECTED) {
      wifiConnect();
  } 
  delay(10000);
}

void blinkLed(uint8_t pin, int frecuency){
  digitalWrite(pin, HIGH);
  delay(frecuency);
  digitalWrite(pin, LOW);
  delay(frecuency);
}

void wifiConnect(){
  digitalWrite(LED, HIGH);
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  
  // Uncomment and run it once, if you want to erase all the stored information
  // wifiManager.resetSettings();
  
  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with generated name (ESP + ChipID)
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect();

  digitalWrite(LED, LOW);
} //End wifiConnect()

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
  Returns percentage of capacity in trashcan that is being used based on height
*/
float getUsedCapacity(uint8_t trigPin, uint8_t echoPin, float height_cm) {
  float distance = getDistanceUS(trigPin, echoPin);
  return (1 - distance / height_cm) * 100;
}
