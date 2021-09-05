#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "";
const char *password = "";

const char *mqtt_server = "";
const int mqtt_port = 1883;
const char *mqtt_user = "";
const char *mqtt_pass = "";

const char *pub_topic = "";
const char *sub_topic = "";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[100];

//*****************************
//*** DECLARACION FUNCIONES ***
//*****************************
void setup_wifi();
void callback(char *topic, byte *payload, unsigned int length);
void reconnect();

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT);
    Serial.begin(115200);
    randomSeed(micros());
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }

    client.loop();

    long now = millis();
    if (now - lastMsg > 3000)
    {
        lastMsg = now;

        String to_send = "on";
        to_send.toCharArray(msg, 100);
        Serial.print("Publicamos mensaje -> ");
        Serial.println(msg);
        client.publish(pub_topic, msg);
    }
}

//*****************************
//***    CONEXION WIFI      ***
//*****************************
void setup_wifi()
{
    delay(10);
    // Nos conectamos a nuestra red Wifi
    Serial.println();
    Serial.print("Conectando a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Conectado a red WiFi!");
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String incoming = "";
    Serial.print("Mensaje recibido desde -> ");
    Serial.print(topic);
    Serial.println("");
    for (int i = 0; i < length; i++)
    {
        incoming += (char)payload[i];
    }
    incoming.trim();
    Serial.println("Mensaje -> " + incoming);
    //Validamos los comandos
    if (incoming == "on")
    {
        digitalWrite(BUILTIN_LED, HIGH);
    }
    if (incoming == "off")
    {
        digitalWrite(BUILTIN_LED, LOW);
    }
}

void reconnect()
{

    while (!client.connected())
    {
        Serial.print("Intentando conexión Mqtt...");
        // Creamos un cliente ID
        String clientId = "esp32_";
        clientId += String(random(0xffff), HEX);
        // Intentamos conectar
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
        {
            Serial.println("Conectado!");
            digitalWrite(BUILTIN_LED, HIGH);
            // Nos suscribimos
            client.subscribe(sub_topic);
        }
        else
        {
            Serial.print("Falló :( con error -> ");
            Serial.print(client.state());
            Serial.println(" Intentamos de nuevo en 5 segundos");
            delay(5000);
        }
    }
}