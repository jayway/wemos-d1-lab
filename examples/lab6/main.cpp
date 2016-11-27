#include <Arduino.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 2 /* This (GPIO 2) is pin D4 */
#define DHTTYPE DHT11

struct Data
{
  unsigned long timestamp;
  float temperature;
  float humidity;
};

WiFiClient wifiClient;
PubSubClient mqtt_client(wifiClient);
DHT dht(DHTPIN, DHTTYPE);

String clientId = "ESP-" + String(ESP.getChipId());
const char* temp_topic = "esp8266/temp";
const char* led_topic = "esp8266/led";
bool led_state;
unsigned long last_sensor_read = 0;

void updateLed()
{
  digitalWrite(LED_BUILTIN, led_state ? LOW : HIGH);
}

Data readData()
{
  pinMode(DHTPIN, INPUT_PULLUP);
  return { millis(), dht.readTemperature(), dht.readHumidity() };
}

void sendData(Data data)
{
  String msg = "timestamp: " + String(data.timestamp)
   + "\ttemperature: " + String(data.temperature)
   + "\thumidity: " + String(data.humidity);
  Serial.print("Sending data: ");
  Serial.println(msg);
  mqtt_client.publish(temp_topic, msg.c_str());
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
      Serial.println("Connected");
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

  readData();
  last_sensor_read = millis();
}

void loop()
{
  if (!mqtt_client.connected())
  {
    mqtt_reconnect();
  }
  mqtt_client.loop();

  unsigned long current_time = millis();
  if (current_time - last_sensor_read > 5000)
  {
    Data data = readData();
    last_sensor_read = current_time;
    sendData(data);
  }
}
