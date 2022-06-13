# Esp32_mqtt_client

In this example, the device connects to a Wi-Fi network and to an Mqtt server with the following parameters:

```cpp
const char *wifissid = "";
const char *WifiPassword = "";
const char *mqttServer = "";
const char *mqttUser = "";
const char *mqttPassword = "";
const char *publishTopic = "";
const char *subscribeTopic = "";
const int mqttPort = 1883;
```

If your Mqtt server doesn't ask for username or password, you can leave the spaces empty `const char *mqttUser = ""` and ` const char *mqttPassword = ""`.


## How does it work

- If the device exceeds the number of wifi connection attempts `wifiConnectionAttempts = 30`, it will continue to work and try to connect every 20 seconds `wifiLoop()`.

- If the device loses connection to the Mqtt server, it will try to reconnect every 30 seconds `unsigned int intervalMqtt = 30000`.

- When the `#define PUBLISH_BUTTON 0` button is pressed, it sends to the server the name of the network `wifiSSID`, the channel `wifiChannel`, the received signal strength indicator `wifiDbm`, the percentage of the connection quality `wifiPercent` and the internal temperature of the cpu `tempCpu`.

- Through an Mqtt client you can turn on or off a `#define MQTT_LED 2` led as shown in the following example.


## Example of subscription and publication:

Subscribe example:

- To turn on the led

```json
{
  "command": "led",
  "value": "on"
}
```

- To turn off the led

```json
{
  "command": "led",
  "value": "off"
}
```

Publish example:

```json
{
  "wifiSSID": "The name of your Wi-Fi network",
  "wifiChannel": 11,
  "wifiDbm": "-72",
  "wifiPercent": "56",
  "tempCpu": 57
}
```
