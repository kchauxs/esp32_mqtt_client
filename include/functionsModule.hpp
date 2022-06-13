#ifdef __cplusplus
extern "C"
{
#endif
    uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();


void setOnSingle(int pin)
{
    digitalWrite(pin, HIGH);
}

void setOffSingle(int pin)
{
    digitalWrite(pin, LOW);
}

void pressPublishButton()
{
    if (publishPortal.getSingleDebouncedPress())
        publishData();
}

float cpuTemperature()
{
    float cpuTemperature = ((temprature_sens_read() - 32) / 1.8);

    if (isnan(cpuTemperature) || cpuTemperature <= 0)
        cpuTemperature = 0;

    return round(cpuTemperature);
}

String getJsonDeviceStatus()
{
    String payload;
    DynamicJsonDocument jsonDoc(1024);
    jsonDoc["wifiSSID"] = WiFi.SSID();
    jsonDoc["wifiChannel"] = WiFi.channel();
    jsonDoc["wifiDbm"] = WiFi.status() == WL_CONNECTED ? String(WiFi.RSSI()) : F("0");
    jsonDoc["wifiPercent"] = WiFi.status() == WL_CONNECTED ? String(getRSSIasQuality(WiFi.RSSI())) : F("0");
    jsonDoc["tempCpu"] = cpuTemperature();
    serializeJson(jsonDoc, payload);
    return payload;
}

void logs()
{
    long currentmillis = millis();

    if (currentmillis - previousLogsMillis > intervalLogs)
    {
        previousLogsMillis = currentmillis;

        Serial.println("");
        Serial.println("+-------------------------------[DEVICE LOGS]-----------------------------------+");
        Serial.println("");
        Serial.println("Info: CPU Temperature: " + (String)cpuTemperature());
        Serial.println("Info: Last Incomming Msg: " + lastReceivedMsg);
        Serial.println("Info: Mqtt is Connected: " + (String)(getMqttConnection() ? "Ok" : "Not") + " | Status: " + showMqttConnectionStatus());
        Serial.println("Info: Wi-Fi is Connected: " + (String)(checkWifiConnection() ? "Ok" : "Not") + " | Status: " + showWifiConnectionStatus());
        Serial.println("Info: Wi-Fi SSID: " + WiFi.SSID());
        Serial.print("Info: Wi-Fi IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.println("Info: Wi-Fi RSSI: " + (String)WiFi.RSSI());
        Serial.println("Info: Wi-Fi Channel: " + (String)WiFi.channel());
        Serial.println("-------------------------------------------------------------------------------");
    }
}