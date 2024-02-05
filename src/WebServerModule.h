// WebServerModule.h
#ifndef WebServerModule_h
#define WebServerModule_h

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "INA219Module.h"

class WebServerModule {
public:
    WebServerModule(INA219Module& inaModule, int relayPin, int secondRelayPin);

  void begin();
  bool getRelayState() const;
  bool getSecondRelayState() const;
  void handleToggleState(bool state);
  void handleSecondToggleState(bool state);
  
private:
  INA219Module& _inaModule;
  AsyncWebServer _server;

  bool relayState;
  bool secondRelayState;
  int _relayPin;
  int _secondRelayPin;
  
  void handleRoot(AsyncWebServerRequest *request);
  void handleValues(AsyncWebServerRequest *request);
  void handleToggle(AsyncWebServerRequest *request);
  void handleSecondToggle(AsyncWebServerRequest *request);
};

#endif
