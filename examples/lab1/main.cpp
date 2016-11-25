#include <Arduino.h>

int blink = LOW;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, blink);
  Serial.println();
}

void loop()
{
  Serial.print(".");
  blink = (blink == LOW ? HIGH : LOW);
  digitalWrite(LED_BUILTIN, blink);
  delay(1000);
}