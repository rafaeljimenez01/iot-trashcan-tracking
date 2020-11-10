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

/**
 * LED flashes every certain frequency
 * 
 * @param pin pin where LED is connected
 * @param frecuency frecuency the led will be flashing
 */

void blinkLed(uint8_t pin, int frecuency){
  digitalWrite(pin, HIGH);
  delay(frecuency);
  digitalWrite(pin, LOW);
  delay(frecuency);
}

/**
  * Connects to local wifi network
  * 
  * @param ssid name of local wifi network
  * @param password password of local wifi network
*/
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