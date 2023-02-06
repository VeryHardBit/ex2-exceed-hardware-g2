// If you want to run in WOKWi
// change pin and wifi
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>
#include "traffic.h"

#define red <led red pin>
#define yellow <led yellow pin>
#define green <led green pin>
#define ldr <ldr pin>
#define button <button pin>

#define light <แสดงมันมืด มีค่าเท่าไหร่>

int state = 1;
int count = 0;
Bounce debouncer = Bounce();

void Connect_Wifi();

void setup()
{
  Serial.begin(115200);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(ldr, INPUT);
  debouncer.attach(button, INPUT_PULLUP);
  debouncer.interval(25);
  Connect_Wifi();

  delay(200);
  // start LED with GREEN and POST to database
  digitalWrite(green, HIGH);
  POST_traffic("green");
}

long int time0 = 0;
long int time1 = 0;
void loop(){
  // *** write your code here ***
  // Your can change everything that you want
  debouncer.update();
  bool btn_pressed=debouncer.fell();
  if (state == 1){
    // while led GREEN
    digitaWrite(green, HIGH);
    if(btn_pressed){
      state=2;
      digitalWrite(green, LOW);
      POST_traffic("yellow");
      time0=millis();
    }
  }
  else if (state == 2){
    // while led YELLOW
    digitalWrite(yellow, HIGH);
    if(millis()-time0>8000){
      state=3;
      digitalWrite(yellow, LOW);
      POST_traffic("red");
      time0=millis();
    }
  }
  else if (state == 3){
    // while led RED
    digitalWrite(red, HIGH);
    if(millis()-time0>5000){
      while(1){
        
      }
      state=4;
      digitalWrite(red, LOW);
      POST_traffic("green");
      time0=millis();
    }
  }
}

void Connect_Wifi()
{
  const char *ssid = "Your Wifi Name";
  const char *password = "Your Wifi Password";
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("OK! IP=");
  Serial.println(WiFi.localIP());
  Serial.println("----------------------------------");
}