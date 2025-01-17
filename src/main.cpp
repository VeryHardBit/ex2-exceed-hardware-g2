#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>
#include "traffic.h"

#define red 26
#define yellow 25
#define green 33
#define ldr 32
#define button 27

#define dim_light 3900

int state = 1;
int count = 0;
Bounce debouncer = Bounce();

void Connect_Wifi();

void setup(){
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
    debouncer.update();
    bool btn_pressed=debouncer.fell();
    if (state == 1){//green state
        digitalWrite(green, HIGH);
        if(btn_pressed){
            state=2;
            digitalWrite(green, LOW);
            POST_traffic("yellow");
            time0=millis();
        }
    }else if (state == 2){//yellow state
        digitalWrite(yellow, HIGH);
        if(millis()-time0>8000){
            state=3;
            digitalWrite(yellow, LOW);
            POST_traffic("red");
            GET_traffic();
            time0=millis();
        }
    }else if (state == 3){//red state
        digitalWrite(red, HIGH);
        if(millis()-time0>5000){
            while(1){
                int light_value=map(analogRead(ldr),dim_light,4095,0,255);
                if(light_value<=0)break;
            }
            state=1;
            digitalWrite(red, LOW);
            POST_traffic("green");
            GET_traffic();
            time0=millis();
        }
    }
    delay(1000/60);
}

void Connect_Wifi(){
    const char *ssid = "OPPO_KUY";
    const char *password = "oppopass";
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.print("OK! IP=");
    Serial.println(WiFi.localIP());
    Serial.println("----------------------------------");
}
