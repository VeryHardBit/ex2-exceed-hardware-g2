#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>

const String baseUrl = "https://exceed-hardware-stamp465.koyeb.app";

const String point = "2";
const int nearby_1 = 1;
const int nearby_2 = 3;

void GET_traffic()
{
  DynamicJsonDocument doc(65536);
  HTTPClient http;
  const String url = baseUrl + "/all_traffic";
  http.begin(url);

  Serial.println("Nearby traffic");
  int httpResponseCode = http.GET();
  if (httpResponseCode >= 200 && httpResponseCode < 300)
  {
    String payload = http.getString();
    deserializeJson(doc, payload);

    // *** write your code here ***
    // set up JSON
    Serial.println();
    Serial.println((const char*)doc[1]["point"]);
    Serial.println((const char*)doc[1]["traffic"]);

  }
  else
  {
    Serial.print("Error ");
    Serial.println(httpResponseCode);
  }

  Serial.println("----------------------------------");
}

void POST_traffic(String led)
{
  const String url = baseUrl + "/my_traffic?point=" + point;
  String json;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(2048);
  // *** write your code here ***
  doc["code"] = "zgam1";
  doc["traffic"] = led;
  serializeJson(doc, json);

  Serial.println("POST " + led);
  int httpResponseCode = http.POST(json);
  if (httpResponseCode == 200)
  {
    Serial.print("Done");
    Serial.println();
  }
  else
  {
    Serial.print("Error ");
    Serial.println(httpResponseCode);
  }

  Serial.println("----------------------------------");
}