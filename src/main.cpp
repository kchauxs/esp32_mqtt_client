#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Pushbutton.h>

#define PUBLISH_BUTTON 0
#define LED 13
#define MQTT_LED 2

const char *wifissid = "";
const char *WifiPassword = "";
const char *mqttServer = "";
const char *mqttUser = "";
const char *mqttPassword = "";
const char *publishTopic = "";
const char *subscribeTopic = "";
const int mqttPort = 1883;

unsigned int intervalLogs = 3300;
unsigned int intervalWifi = 20000;
unsigned int intervalMqtt = 30000;
unsigned long previousLogsMillis = 0;
unsigned long previousMqttMillis = 0;
unsigned long previousWifiMillis = 0;
const short wifiConnectionAttempts = 30;

String lastReceivedMsg = "{}";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
Pushbutton publishPortal(PUBLISH_BUTTON);

// functionsModule
void logs();
void pressPublishButton();
void setOffSingle(int pin);
void setOnSingle(int pin);
float cpuTemperature();
String getJsonDeviceStatus();
// mqttModule
bool getMqttConnection();
void mqttCommands(String incoming);
bool mqttConnect();
String showMqttConnectionStatus();
void callback(char *topic, byte *payload, unsigned int length);
void mqttLoop();
void publishData();
// wifiModule
void initWifi();
void wifiLoop();
bool checkWifiConnection();
int getRSSIasQuality(int RSSI);
String showWifiConnectionStatus();

#include "functionsModule.hpp"
#include "wifiModule.hpp"
#include "mqttModule.hpp"

void setup()
{
    Serial.begin(115200);
    delay(500);

    pinMode(LED, OUTPUT);
    pinMode(MQTT_LED, OUTPUT);
    pinMode(PUBLISH_BUTTON, INPUT);

    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.setCallback(callback);
    delay(500);

    initWifi();
    delay(500);
}

void loop()
{
    yield();
    wifiLoop();
    mqttLoop();
    pressPublishButton();
    logs();
}