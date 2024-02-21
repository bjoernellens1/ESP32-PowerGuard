// include secrets file.
#include "secrets.h"

#ifndef CONFIG_H
#define CONFIG_H

// InfluxDB config - This part needs to be changed when you are doing the Arduino setup wizard in InfluxDB.
const char* INFLUXDB_URL = "https://db.bjoernellens1.com";
const char* INFLUXDB_ORG = "48cec24009273b5e";
const char* INFLUXDB_BUCKET = "bucket-digitalisierung";

// Other configurations
// #define RELAY_PIN 2
// #define SECOND_RELAY_PIN 15
#define RELAY_PIN 2
#define SECOND_RELAY_PIN 1
#define THIRD_RELAY_PIN 4
#define FOURTH_RELAY_PIN 3

#endif  // CONFIG_H
