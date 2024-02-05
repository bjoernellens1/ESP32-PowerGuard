#ifndef WiFiModule_h
#define WiFiModule_h

#include <WiFi.h>

class WiFiModule {
public:
    WiFiModule(const char* ssid, const char* password);
    void connect();
    bool isConnected();
    IPAddress getLocalIP();
    void printConnectionInfo();

private:
    char _ssid[32]; // Speicher für SSID und Passwort reservieren
    char _password[32]; // Angenommene Maximalwerte, entsprechend anpassen falls nötig
};

#endif
