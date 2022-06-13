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

- If the device exceeds the number of wifi connection attempts `wifiConnectionAttempts = 30`, it will continue to work and try to connect every 20 seconds `wifi Loop()`.

- If the device loses connection to the Mqtt server, it will try to connect every 30 seconds `unsigned int intervalMqtt = 30000`.

- When the `#define PUBLISH_BUTTON 0` button is pressed, it sends the server the network name `wifiSSID`, the channel `wifiChannel`, the received signal strength indicator `wifiDbm`, the connection quality percentage `wifiPercent ` and the internal temperature of the cpu `tempCpu`.

- A través de un cliente Mqtt, encienda o apague el led `#define MQTT_LED 2` como se muestra en los ejemplos.


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
