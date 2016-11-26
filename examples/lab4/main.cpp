#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 2 /* This (GPIO 2) is pin D4 */
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
unsigned long last_sensor_read = 0;

void readData()
{
  pinMode(DHTPIN, INPUT_PULLUP);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" (C)");
}

void setup()
{
  Serial.begin(115200);
  readData();
  last_sensor_read = millis();
}

void loop()
{
  unsigned long current_time = millis();
  long time_until_next_read = (last_sensor_read + 5000) - current_time;
  if (time_until_next_read > 0)
  {
    Serial.print("Sleeping for ");
    Serial.print(time_until_next_read);
    Serial.println(" millis");
    delay(time_until_next_read);
    return;
  }
  readData();
  last_sensor_read = current_time;
}