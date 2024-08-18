#define BLYNK_TEMPLATE_ID "TMPL3zwl_JG7R"
#define BLYNK_TEMPLATE_NAME "GreenTechFlow"
#define BLYNK_AUTH_TOKEN "neRgw2w3qM4BWoebjm-sRcx7NIBWGwN1"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

char ssid[] = "NIKHIL OFFICE";
char pass[] = "NIKHIL@844";

#define sensorPin D3
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

OneWire oneWire(D2);
DallasTemperature sensors(&oneWire);

BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t))
  {
    Blynk.virtualWrite(V5, h);  // V5 is for Humidity
    Blynk.virtualWrite(V6, t);  // V6 is for Temperature
  }
  else
  {
    Serial.println("Failed to read from DHT sensor!");
  }
}

void sendTemps()
{
  int sensor = analogRead(A0);
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  Serial.println(temp);
  Serial.println(sensor);

  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2, sensor);
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(sensorPin, INPUT);
  dht.begin();
  sensors.begin();
  
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
  sendTemps();

  sensorState = digitalRead(sensorPin);
  Serial.println(sensorState);

  if (sensorState == 1 && lastState == 0)
  {
    Serial.println("needs water, send notification");
    // Blynk.notify("Water your plants");
    lastState = 1;
  }
  else if (sensorState == 1 && lastState == 1)
  {
    Serial.println("has not been watered yet");
  }
  else
  {
    Serial.println("does not need water");
    lastState = 0;
  }
}
