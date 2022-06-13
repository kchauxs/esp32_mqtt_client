
void mqttLoop()
{
    if (!mqttClient.connected())
    {
        mqttConnect();
        setOffSingle(MQTT_LED);
    }

    else
    {
        mqttClient.loop();
        setOnSingle(MQTT_LED);
    }
}

bool mqttConnect()
{
    long currentmillis = millis();

    if (currentmillis - previousMqttMillis > intervalMqtt)
    {
        previousMqttMillis = currentmillis;

        String strClientId = "device_" + random(1, 9999);
        Serial.print("Info: Connecting to mqtt server: ");

        if (mqttClient.connect(strClientId.c_str(), mqttUser, mqttPassword))
        {
            Serial.println("Ok");
            mqttClient.subscribe(subscribeTopic);
            Serial.println("Info: subscribing to the topic: " + (String)subscribeTopic);
            return true;
        }
        else
        {
            Serial.println("Faild! - state: " + mqttClient.state());
            return false;
        }
    }
    return false;
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String incoming = "";

    for (int i = 0; i < length; i++)
        incoming += (char)payload[i];

    incoming.trim();
    lastReceivedMsg = incoming;
    Serial.println("Info: Topic: " + String(topic));
    Serial.println("Info: Message: " + incoming);

    mqttCommands(incoming);
}

void mqttCommands(String incoming)
{
    DynamicJsonDocument jsonDocument(512);
    deserializeJson(jsonDocument, incoming);

    String command = jsonDocument["command"];
    Serial.println("Info: command: " + command);

    if (command == "led")
    {
        String value = jsonDocument["value"];
        if (value == "on")
            setOnSingle(LED);

        if (value == "off")
            setOffSingle(LED);
    }
}

void publishData()
{
    String payload = getJsonDeviceStatus();
    Serial.print("Info: Publish message: ");

    if (mqttClient.publish(publishTopic, payload.c_str()))
        Serial.println("Ok - payload: " + payload);
    else
        Serial.println("Faild!");
}

String showMqttConnectionStatus()
{
    int MQTT_STATUS = mqttClient.state();
    String messageStatus = "";

    switch (MQTT_STATUS)
    {
    case MQTT_CONNECTION_TIMEOUT:
        messageStatus = "MQTT_CONNECTION_TIMEOUT";
        break;

    case MQTT_CONNECTION_LOST:
        messageStatus = "MQTT_CONNECTION_LOST";
        break;

    case MQTT_CONNECT_FAILED:
        messageStatus = "MQTT_CONNECT_FAILED";
        break;

    case MQTT_DISCONNECTED:
        messageStatus = "MQTT_DISCONNECTED";
        break;

    case MQTT_CONNECTED:
        messageStatus = "MQTT_CONNECTED";
        break;

    case MQTT_CONNECT_BAD_PROTOCOL:
        messageStatus = "MQTT_CONNECT_BAD_PROTOCOL";
        break;

    case MQTT_CONNECT_BAD_CLIENT_ID:
        messageStatus = "MQTT_CONNECT_BAD_CLIENT_ID";
        break;

    case MQTT_CONNECT_UNAVAILABLE:
        messageStatus = "MQTT_CONNECT_UNAVAILABLE";
        break;

    case MQTT_CONNECT_BAD_CREDENTIALS:
        messageStatus = "MQTT_CONNECT_BAD_CREDENTIALS";
        break;
    case MQTT_CONNECT_UNAUTHORIZED:
        messageStatus = "MQTT_CONNECT_UNAUTHORIZED";
        break;

    default:
        messageStatus = "DISCONNECTED";
        break;
    }

    return String(MQTT_STATUS) + ": " + messageStatus;
}

bool getMqttConnection()
{
    return mqttClient.connected();
}