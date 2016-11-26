#include <Arduino.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>

char name[] = "mywemos";
ESP8266WebServer server(80);

void handleRoot()
{
  server.send(200, "text/plain", "hello from esp8266!");
}

void setup()
{
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifi_station_set_hostname(name);
  wifiManager.autoConnect();

  // register handlers (can both be function pointers or lambdas)
  server.on("/", handleRoot);
  server.onNotFound([](){
      server.send(404, "text/plain", "404 Not found");
  });

  // start server
  server.begin();
  Serial.println("HTTP server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  server.handleClient();
}