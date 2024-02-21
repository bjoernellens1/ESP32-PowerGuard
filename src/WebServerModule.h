// WebServerModule.h
#ifndef WebServerModule_h
#define WebServerModule_h

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "INA219Module.h"

class WebServerModule {
public:
    WebServerModule(INA219Module& inaModule, int relayPin, int secondRelayPin, int thirdRelayPin, int fourthRelayPin);

  void begin();
  bool getRelayState() const;
  bool getSecondRelayState() const;
  bool getThirdRelayState() const;
  bool getFourthRelayState() const;
  void handleToggleState(bool state);
  void handleSecondToggleState(bool state);
  void handleThirdToggleState(bool state);
  void handleFourthToggleState(bool state);
  
private:
  INA219Module& _inaModule;
  AsyncWebServer _server;

  bool relayState;
  bool secondRelayState;
  bool thirdrelayState;
  bool fourthRelayState;
  int _relayPin;
  int _secondRelayPin;
  int _thirdRelayPin;
  int _fourthRelayPin;
  
  void handleRoot(AsyncWebServerRequest *request);
  void handleValues(AsyncWebServerRequest *request);
  void handleToggle(AsyncWebServerRequest *request);
  void handleSecondToggle(AsyncWebServerRequest *request);
  void handleThirdToggle(AsyncWebServerRequest *request);
  void handleFourthToggle(AsyncWebServerRequest *request);
};

#endif
