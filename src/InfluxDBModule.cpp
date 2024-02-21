// InfluxDBModule.cpp
#include "InfluxDBModule.h"

InfluxDBModule::InfluxDBModule(String url, String org, String bucket, String token)
    : client(url, org, bucket, token, InfluxDbCloud2CACert) {}

bool InfluxDBModule::connectToInfluxDB() {
    return client.validateConnection();
}

bool InfluxDBModule::sendData(float current, float busVoltage, float power, float avgPower, float totalEnergy, int relay1State, int relay2State, int relay3State, int relay4State) {
    Point sensorData("sensor_data");
    sensorData.addTag("device", "ESP32");
    sensorData.addField("current", current);
    sensorData.addField("busVoltage", busVoltage);
    sensorData.addField("power", power);
    sensorData.addField("avgPower", avgPower);
    sensorData.addField("totalEnergy", totalEnergy);
    sensorData.addField("relay1StateINT", relay1State);
    sensorData.addField("relay2StateINT", relay2State);
    sensorData.addField("relay3StateINT", relay3State);
    sensorData.addField("relay4StateINT", relay4State);

    return client.writePoint(sensorData);
}