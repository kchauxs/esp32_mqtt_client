void initWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifissid, WifiPassword);
    Serial.print("\nInfo: Connecting to the Wi-Fi network ");

    short attempt = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        attempt++;
        if (attempt > wifiConnectionAttempts)
            break;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("");
        Serial.print("Info: Wi-Fi  IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Info: Wi-Fi Channel: ");
        Serial.println(WiFi.channel());
        Serial.print("Info: Wi-Fi RSSI: ");
        Serial.println(WiFi.RSSI());
    }
}

void wifiLoop()
{
    unsigned long currentmillis = millis();
    if (WiFi.status() != WL_CONNECTED && (currentmillis - previousWifiMillis >= intervalWifi))
    {
        WiFi.disconnect();
        WiFi.reconnect();
        previousWifiMillis = currentmillis;
    }
}

bool checkWifiConnection()
{
    return WiFi.isConnected();
}

int getRSSIasQuality(int RSSI)
{
    int quality = 0;

    if (RSSI <= -100)
        quality = 0;

    else if (RSSI >= -50)
        quality = 100;

    else
        quality = 2 * (RSSI + 100);

    return quality;
}

String showWifiConnectionStatus()
{
    wl_status_t WIFI_STATUS = WiFi.status();
    String wifiMessageStatus = "";

    switch (WIFI_STATUS)
    {
    case WL_NO_SHIELD:
        wifiMessageStatus = "WL NO SHIELD";
        break;

    case WL_IDLE_STATUS:
        wifiMessageStatus = "IDLE STATUS";
        break;

    case WL_NO_SSID_AVAIL:
        wifiMessageStatus = "NO SSID AVAIL";
        break;

    case WL_SCAN_COMPLETED:
        wifiMessageStatus = "SCAN COMPLETED";
        break;

    case WL_CONNECTED:
        wifiMessageStatus = "CONNECTED";
        break;

    case WL_CONNECT_FAILED:
        wifiMessageStatus = "CONNECT FAILED";
        break;

    case WL_CONNECTION_LOST:
        wifiMessageStatus = "CONNECTION LOST";
        break;

    case WL_DISCONNECTED:
        wifiMessageStatus = "DISCONNECTED";
        break;

    default:
        wifiMessageStatus = "DISCONNECTED";
        break;
    }

    return String(WIFI_STATUS) + ": " + wifiMessageStatus;
}