#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


ESP8266WebServer server(80);
int step_number = 0;
int dir = -1;
bool work = false;
int pins[] = {D1, D2, D3, D4};

void start_motor(bool direction) {
  for (int i = 0; i < 4; i++) {
    int sig;
    if (i == step_number) {
      sig = HIGH;
    } else {
      sig = LOW;
    }
    digitalWrite(pins[i], sig);
  }
  step_number += 1 * dir;
  step_number = (step_number + 4) % 4;
  delay(2);
}

void handleRootPath() {
    String response = "";
    for (int i = 0; i < server.args(); i++){
        if (server.argName(i) == "dir") {
          if (server.arg(i) == "1"){
            dir = 1;
          }
          else if (server.arg(i) == "-1") {
            dir = -1;
          }
          response += "Direction set as " + server.arg(i) + "\n";
        }
        if (server.argName(i) == "work") {
          if (server.arg(i) == "1"){
            work = true;
          }
          else if (server.arg(i) == "0") {
            work = false;
          }
          response += "Work set as " + server.arg(i) + "\n";
        }
    }
    server.send(200, "text/plain", response);
}

void setup() {
    Serial.begin(9600);
    WiFi.begin("UPC1857056", "hzevsvf2anfU");
    while (WiFi.status() != WL_CONNECTED) {   //Wait for connection
        delay(1000);
        Serial.println("Waiting to connect...");
    }
    Serial.println(WiFi.localIP());
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    Serial.println(D1);
    pinMode(D2, OUTPUT);
    Serial.println(D2);
    pinMode(D3, OUTPUT);
    Serial.println(D3);
    pinMode(D4, OUTPUT);
    Serial.println(D4);
    server.on("/", handleRootPath);
    server.begin();
}

void loop() {
    server.handleClient();
    if (work) {
      start_motor(dir);
    }
    Serial.println(step_number);
}
