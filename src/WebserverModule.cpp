// WebServerModule.cpp
#include "WebServerModule.h"

WebServerModule::WebServerModule(INA219Module& inaModule, int relayPin, int secondRelayPin) 
  : _inaModule(inaModule), _server(80), _relayPin(relayPin), _secondRelayPin(secondRelayPin), relayState(false), secondRelayState(false) {}

void WebServerModule::begin() {
  // Konfiguriere die Pins für die Relais als Ausgänge
  pinMode(_relayPin, OUTPUT);
  pinMode(_secondRelayPin, OUTPUT);

  // Konfiguriere die Server-Endpunkte
  _server.on("/", HTTP_GET, std::bind(&WebServerModule::handleRoot, this, std::placeholders::_1));
  _server.on("/values", HTTP_GET, std::bind(&WebServerModule::handleValues, this, std::placeholders::_1));
  _server.on("/toggle1", HTTP_GET, std::bind(&WebServerModule::handleToggle, this, std::placeholders::_1));
  _server.on("/toggle2", HTTP_GET, std::bind(&WebServerModule::handleSecondToggle, this, std::placeholders::_1));

  _server.begin();
}

void WebServerModule::handleRoot(AsyncWebServerRequest *request) {
  String html = "<html><head><style>";
  // CSS-Stil für die Schalter und Beschriftungen
  html += ".toggle-container {";
  html += "  display: flex;";
  html += "  align-items: center;";
  html += "  margin-bottom: 10px;";
  html += "}";
  html += ".toggle-label {";
  html += "  margin-right: 10px;";
  html += "}";
  html += ".toggle {";
  html += "  position: relative;";
  html += "  display: inline-block;";
  html += "  width: 60px;";
  html += "  height: 34px;";
  html += "}";
  html += ".toggle input {";
  html += "  opacity: 0;";
  html += "  width: 0;";
  html += "  height: 0;";
  html += "}";
  html += ".slider {";
  html += "  position: absolute;";
  html += "  cursor: pointer;";
  html += "  top: 0;";
  html += "  left: 0;";
  html += "  right: 0;";
  html += "  bottom: 0;";
  html += "  background-color: #ccc;";
  html += "  border-radius: 34px;";
  html += "  transition: .4s;";
  html += "}";
  html += ".inner-slider {";
  html += "  position: absolute;";
  html += "  content: \"\";";
  html += "  height: 26px;";
  html += "  width: 26px;";
  html += "  left: 4px;";
  html += "  bottom: 4px;";
  html += "  background-color: white;";
  html += "  border-radius: 50%;";
  html += "  transition: .4s;";
  html += "}";
  html += "input:checked + .slider {";
  html += "  background-color: #2196F3;";
  html += "}";
  html += "input:focus + .slider {";
  html += "  box-shadow: 0 0 1px #2196F3;";
  html += "}";
  html += "</style></head><body>";
  html += "<h1>Energieverbrauchsmonitor</h1>";
  // Anzeige der Werte
  html += "<p>Strom: <span id='current_A'></span> A</p>";
  html += "<p>Spannung: <span id='busVoltage_V'></span> V</p>";
  html += "<p>Leistung: <span id='power_W'></span> W</p>";
  html += "<p>Durchschnittliche Leistung: <span id='avgPower_W'></span> W</p>";
  html += "<p>Gesamtenergie: <span id='totalEnergy'></span> Wh</p>";
  // JavaScript-Code für die Werteaktualisierung und Schaltersteuerung
  html += "<script>setInterval(updateValues, 5000);";
  html += "function updateValues() { fetch('/values').then(response => response.json()).then(data => {";
  html += "document.getElementById('current_A').textContent = data.current_A;";
  html += "document.getElementById('busVoltage_V').textContent = data.busVoltage_V;";
  html += "document.getElementById('power_W').textContent = data.power_W;";
  html += "document.getElementById('avgPower_W').textContent = data.avgPower_W;";
  html += "document.getElementById('totalEnergy').textContent = data.totalEnergy;";
  html += "}); }</script>";
  // Schalter 1 (K1)
  html += "<div class='toggle-container'>";
  html += "<label class='toggle-label'>Relais 1</label>"; // Beschriftung für den ersten Schalter
  html += "<label class='toggle'>";
  html += "<input type='checkbox' id='relaySwitch1' onchange='toggleRelay(1, this.checked)'>";
  html += "<span class='slider'></span>";
  html += "<span class='inner-slider'></span>";
  html += "</label>";
  html += "</div>";
  // Schalter 2 (K2)
  html += "<div class='toggle-container'>";
  html += "<label class='toggle-label'>Relais 2</label>"; // Beschriftung für den zweiten Schalter
  html += "<label class='toggle'>";
  html += "<input type='checkbox' id='relaySwitch2' onchange='toggleRelay(2, this.checked)'>";
  html += "<span class='slider'></span>";
  html += "<span class='inner-slider'></span>";
  html += "</label>";
  html += "</div>";
  // JavaScript-Code für die Schaltersteuerung
  html += "<script>";
  html += "function toggleRelay(relay, state) {";
  html += "  fetch(`/toggle${relay}?state=${Number(state)}`).then(response => response.text()).then(data => {";
  html += "    const innerSlider = document.querySelectorAll('.inner-slider')[relay - 1];"; // Das entsprechende Inner Slider
  html += "    const translation = state ? '26px' : '0';"; // Verschiebungsberechnung
  html += "    innerSlider.style.transform = `translateX(${translation})`;"; // Anwenden der Verschiebung
  html += "  });";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  request->send(200, "text/html", html);
}

void WebServerModule::handleValues(AsyncWebServerRequest *request) {
  String values = "{\"current_A\":" + String(_inaModule.getCurrent()) + ",";
  values += "\"busVoltage_V\":" + String(_inaModule.getBusVoltage()) + ",";
  values += "\"power_W\":" + String(_inaModule.getPower()) + ",";
  values += "\"avgPower_W\":" + String(_inaModule.calculateAveragePower()) + ",";
  values += "\"totalEnergy\":" + String(_inaModule.getTotalEnergy()) + "}";
  request->send(200, "application/json", values);
}

void WebServerModule::handleToggle(AsyncWebServerRequest *request) {
  if (request->hasParam("state")) {
    int state = request->getParam("state")->value().toInt();
    relayState = state;
    digitalWrite(_relayPin, relayState ? HIGH : LOW);

    Serial.print("Relay 1 state set to: ");
    Serial.println(relayState);

    request->send(200, "text/plain", relayState ? "Relay 1 turned on" : "Relay 1 turned off");
  } else {
    request->send(400); // Fehler, keine Zustandsinformation
  }
}

void WebServerModule::handleSecondToggle(AsyncWebServerRequest *request) {
  if (request->hasParam("state")) {
    int state = request->getParam("state")->value().toInt();
    secondRelayState = state;
    digitalWrite(_secondRelayPin, secondRelayState ? HIGH : LOW);

    Serial.print("Relay 2 state set to: ");
    Serial.println(secondRelayState);

    request->send(200, "text/plain", secondRelayState ? "Relay 2 turned on" : "Relay 2 turned off");
  } else {
    request->send(400); // Fehler, keine Zustandsinformation
  }
}

void WebServerModule::handleToggleState(bool state) {
  relayState = state;
  digitalWrite(_relayPin, relayState ? HIGH : LOW);
}

void WebServerModule::handleSecondToggleState(bool state) {
  secondRelayState = state;
  digitalWrite(_secondRelayPin, secondRelayState ? HIGH : LOW);
}

bool WebServerModule::getRelayState() const {
  return relayState;
}

bool WebServerModule::getSecondRelayState() const {
  return secondRelayState;
}