#include "WiFiModule.h"

WiFiModule::WiFiModule(const char* ssid, const char* password) {
    // Kopiere SSID und Passwort in die Member-Variablen
    strncpy(_ssid, ssid, sizeof(_ssid) - 1);
    _ssid[sizeof(_ssid) - 1] = '\0'; // Stelle sicher, dass der String nullterminiert ist

    strncpy(_password, password, sizeof(_password) - 1);
    _password[sizeof(_password) - 1] = '\0'; // Stelle sicher, dass der String nullterminiert ist
}

void WiFiModule::connect() {
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
}

bool WiFiModule::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

IPAddress WiFiModule::getLocalIP() {
    return WiFi.localIP();
}

void WiFiModule::printConnectionInfo() {
    if (isConnected()) {
        Serial.println("Connected to WiFi");
        Serial.print("Local IP: ");
        Serial.println(getLocalIP());
    } else {
        Serial.println("Connection to WiFi failed");
    }
}
