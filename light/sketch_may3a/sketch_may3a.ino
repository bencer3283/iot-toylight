/**
   BasicHTTPSClient.ino

    Created on: 20.08.2018

*/

#include <Arduino.h>
#include <Servo.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>
// Fingerprint for demo URL, expires on June 2, 2021, needs to be updated well before this date
// const uint8_t fingerprint[20] = { 0x40, 0xaf, 0x00, 0x6b, 0xec, 0x90, 0x22, 0x41, 0x8e, 0xa3, 0xad, 0xfa, 0x1a, 0xe8, 0x25, 0x41, 0x1d, 0x1a, 0x54, 0xb3 };

const int buttonPin = 12;
const int ledPin = 4;

ESP8266WiFiMulti WiFiMulti;


void setup() {
  
  
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("RISD-MiscDevices", "T3chn0l0gy!");
}

void loop() {
  bool button = false;
  bool remote = false;
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    
    if(digitalRead(buttonPin) == HIGH){
      button = true;
      //digitalWrite(ledPin, HIGH);
    } else {
      button = false;
    }
    Serial.print("button: ");
    Serial.println(button);

    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    // client->setFingerprint(fingerprint);
    // Or, if you happy to ignore the SSL certificate, then use the following line instead:
    client->setInsecure();

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://iot-toylight.onrender.com")) {  // HTTPS

      https.addHeader("Content-Type", "application/json");
      Serial.print("[HTTPS] POST...\n");
      // start connection and send HTTP header
      int httpCode = https.POST("{\"id\":1, \"is_on\":" + String(button) + "}");

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
          if (payload == "true") {
            Serial.println("light up");
            remote = true;
            //digitalWrite(ledPin, HIGH);                       // waits 15ms for the servo to reach the position
            }
          }
          else{
            //digitalWrite(ledPin, LOW);
          }
        
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  if(button || remote){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
  Serial.println("Wait 30ms before next round...");
  delay(30);
}
