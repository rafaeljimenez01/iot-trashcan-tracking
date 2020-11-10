#include <Arduino.h>
#include <ESP8266WiFi.h>

char* ssid = "IZZI-8E07";
char* password = "HERNANDEZ2019";

WiFiClient client;

#define Trig D0
#define Echo D1
#define LED D5


void blinkLed(uint8_t pin, int frecuency);
void connectToWifi(char* ssid, char* password);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  connectToWifi(ssid, password);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == 3) {
    blinkLed(LED, 0);
  }
}

void blinkLed(uint8_t pin, int frecuency){
  digitalWrite(pin, HIGH);
  delay(frecuency);
  digitalWrite(pin, LOW);
  delay(frecuency);
}

void connectToWifi(char* ssid, char* password) {
  Serial.println("Connecting to: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    blinkLed(LED, 100);
  }

  Serial.println("");
  Serial.println("WiFi Connected");
}