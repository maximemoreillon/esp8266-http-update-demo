/**
   httpUpdate.ino

    Created on: 27.11.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include "SSD1306.h"
#include "version.h"
#include "secrets.h"

#define DISPLAY_SDA_PIN 5
#define DISPLAY_SCL_PIN 4

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64


ESP8266WiFiMulti WiFiMulti;
SSD1306 display(0x3c, DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(false);

  Serial.println();
  Serial.print("Current version: v");
  Serial.println(FIRMWARE_VERSION);

  display.init();
  display.setFont(ArialMT_Plain_24);
  display.clear();
  display.drawString(0,0, FIRMWARE_VERSION);
  display.display();

  ESPhttpUpdate.setClientTimeout(2000);  // default was 8000

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
}

void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}


void loop() {
  // wait for WiFi 
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

    // Add optional callback notifiers
    ESPhttpUpdate.onStart(update_started);
    ESPhttpUpdate.onEnd(update_finished);
    ESPhttpUpdate.onProgress(update_progress);
    ESPhttpUpdate.onError(update_error);

    t_httpUpdate_return ret = ESPhttpUpdate.update(client, "http://172.16.98.151:7070/update",FIRMWARE_VERSION);
    // Or:
    // t_httpUpdate_return ret = ESPhttpUpdate.update(client, "server", 80, "file.bin");

    switch (ret) {
      case HTTP_UPDATE_FAILED: Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str()); break;

      case HTTP_UPDATE_NO_UPDATES: Serial.println("HTTP_UPDATE_NO_UPDATES"); break;

      case HTTP_UPDATE_OK: Serial.println("HTTP_UPDATE_OK"); break;
    }
  }

  delay(10000);
}
