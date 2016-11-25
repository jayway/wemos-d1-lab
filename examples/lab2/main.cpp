#include <Arduino.h>
#include <WiFiManager.h>

char name[] = "mywemos";

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered AP mode for configuration");
  Serial.print("SSID: ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}

void setup()
{
  Serial.begin(115200);

  WiFiManager wifiManager;

  // for debugging
  //wifiManager.resetSettings();

  wifi_station_set_hostname(name);

  // set callback that gets called when connecting to previous WiFi fails,
  // and enters AP mode
  wifiManager.setAPCallback(configModeCallback);

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    // reset and try again
    ESP.reset();
    delay(1000);
  }

  Serial.println("Connected!");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Hostname: ");
  Serial.println(WiFi.hostname());
  Serial.print("GW IP: ");
  Serial.println(WiFi.gatewayIP());
}

void loop()
{
  Serial.print(".");
  delay(5000);
}