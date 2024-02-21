// InfluxDBModule.h
#ifndef InfluxDBModule_h
#define InfluxDBModule_h

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

class InfluxDBModule {
public:
    InfluxDBModule(String url, String org, String bucket, String token);
    bool connectToInfluxDB();
    bool sendData(float current, float busVoltage, float power, float avgPower, float totalEnergy, int relay1State, int relay2State, int relay3State, int relay4State);
private:
    InfluxDBClient client;
};

#endif