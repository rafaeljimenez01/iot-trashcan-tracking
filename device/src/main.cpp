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

const int DHTPin = 0;  //Conectar sensor DHT a pin D3 = GPIO_0
#define DHTTYPE DHT11 //Definimos el tipo de sensor de humedad (DHT11 o DHT22)
//------------------------------------------

#define Trig D0
#define Echo D1
#define LED D5


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//Define humidity/temperature sensor object.
DHT dht(DHTPin, DHTTYPE);

//Define FirebaseESP8266 data object for data sending and receiving
FirebaseData firebaseData;


void blinkLed(uint8_t pin, int frecuency);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  
  wifiConnect();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:


  Serial.print("Temperature = ");
  Serial.println(dht.readTemperature());
  Serial.print("Humidity = ");
  Serial.println(dht.readHumidity());
  float h = dht.readHumidity();  //Lectura de Humedad
  float t = dht.readTemperature(); //Lectura de Temperatura
    
  Firebase.pushFloat(firebaseData, "DHT/2/temperatura", t);  
  Firebase.pushFloat(firebaseData, "DHT/2/humedad", h);         
    

  if(WiFi.status() != WL_CONNECTED) {
      wifiConnect();
  } 
  delay(1000);
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
  wifiManager.resetSettings();
  
  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with generated name (ESP + ChipID)
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect();

  digitalWrite(LED, LOW);
} //End wifiConnect()