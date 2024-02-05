#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "WiFiModule.h"
#include <Adafruit_INA219.h>
#include <Adafruit_BusIO_Register.h>
#include <ESPAsyncWebServer.h>
#include "INA219Module.h"
#include "InfluxDBModule.h"
#include "WebServerModule.h"
#include <ArduinoJson.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include "Config.h"

#define DEVICE "ESP32"

WiFiModule wifiModule(WIFI_SSID, WIFI_PASSWORD);
INA219Module inaModule;
InfluxDBModule influxDB(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
WebServerModule webServer(inaModule, RELAY_PIN, SECOND_RELAY_PIN);

void readAndSendSensorData();
void controlRelays();

void setup() {
  Serial.begin(115200);
  Serial.println("... init ...");
  Wire.begin();
  Serial.println("Wire initialized");

  wifiModule.connect();
  Serial.println("WiFi connected");
  wifiModule.printConnectionInfo();
  Serial.println("WiFi info printed");

  if (!inaModule.begin()) {
    Serial.println("INA219 sensor not found");
    while (1) {
      delay(10);
    }
  }
  Serial.println("INA219 sensor found");

  if (!influxDB.connectToInfluxDB()) {
    Serial.println("Failed to connect to InfluxDB");
  }
  Serial.println("Connected to InfluxDB");

  webServer.begin();
  Serial.println("Web server started");

  Serial.println();
  Serial.println("......");
  Serial.println();
}

void loop() {
  readAndSendSensorData();
  controlRelays();
  delay(1000);
}

void readAndSendSensorData() {
  float current = inaModule.getCurrent();
  float busVoltage = inaModule.getBusVoltage();
  float power = inaModule.getPower();
  float avgPower = inaModule.calculateAveragePower();
  float totalEnergy = inaModule.getTotalEnergy();
  bool relay1State = webServer.getRelayState();
  bool relay2State = webServer.getSecondRelayState();
  // Ändere die bool's zu Float-Werten
  int relay1StateInt = relay1State ? 1 : 0;
  int relay2StateInt = relay2State ? 1 : 0;

  influxDB.sendData(current, busVoltage, power, avgPower, totalEnergy, relay1StateInt , relay2StateInt );

  Serial.print("current: ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("busVoltage: ");
  Serial.print(busVoltage);
  Serial.println(" V");

  Serial.print("power: ");
  Serial.print(power);
  Serial.println(" W");

  Serial.print("avgPower: ");
  Serial.print(avgPower);
  Serial.println(" W");

  Serial.print("totalEnergy: ");
  Serial.print(totalEnergy);
  Serial.println(" Wh");

  Serial.println();
  Serial.println("... waiting .... ");
  Serial.println();
}

void controlRelays() {
    float power = inaModule.getPower();
    float busVoltage = inaModule.getBusVoltage();

    // Zentrale Logik für die Relaissteuerung
    if (power > 70.0) {
        Serial.println("Turning on relays...");
        webServer.handleToggleState(false);  // Relay 1 ausschalten
        webServer.handleSecondToggleState(false);  // Relay 2 ausschalten
    }

    // Zusatz: Relais einschalten, wenn die Spannung zwischen 0 und 14V liegt
    if (busVoltage > 0.0 && busVoltage < 14.0) {
        Serial.println("Turning on relays due to voltage condition...");
        webServer.handleToggleState(true);  // Relay 1 einschalten
        webServer.handleSecondToggleState(true);  // Relay 2 einschalten
    }
}
