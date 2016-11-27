#include <Arduino.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient mqtt_client(wifiClient);

String clientId = "ESP-" + String(ESP.getChipId());
bool led_state;
unsigned long last_time;
const char* out_topic = "esp8266/hello";
const char* led_topic = "esp8266/led";

void updateLed()
{
  digitalWrite(LED_BUILTIN, led_state ? LOW : HIGH);
}

void mqttCallback(char* topic, unsigned char* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  led_state = ( (char)payload[0] == '1' );
  updateLed();
}

void mqtt_reconnect()
{
  while (!mqtt_client.connected())
  {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (mqtt_client.connect(clientId.c_str())) {
      Serial.println("connected");

      mqtt_client.publish(out_topic, "hello world");
      mqtt_client.subscribe(led_topic);

    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  led_state = false;
  updateLed();

  WiFiManager wifiManager;
  wifi_station_set_hostname((char*)clientId.c_str());
  wifiManager.autoConnect();

  mqtt_client.setServer("com.example.mqtt", 1883);
  mqtt_client.setCallback(mqttCallback);
}

void loop()
{
  if (!mqtt_client.connected())
  {
    mqtt_reconnect();
  }
  mqtt_client.loop();

  long current_time = millis();
  if (current_time - last_time > 5000)
  {
    last_time = current_time;
    String msg = "hello " + String(last_time);
    Serial.print("Publish message: ");
    Serial.println(msg);
    mqtt_client.publish(out_topic, msg.c_str());
  }
}
