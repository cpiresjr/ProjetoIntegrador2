#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>
#include <Wire.h>
#include <WiFiClient.h>
#include "ArduinoJson.h"

#define SSID "nome_rede_wifi"
#define PASSWD "senha_rede_wifi"

ESP8266WiFiMulti WiFiMulti;

const char fingerprint[] PROGMEM =  "31 B5 D0 C3 74 CC 25 98 7F 67 32 9D DE FE 5149 E9 AD 8C D1";
char json[400] = {0};

StaticJsonDocument<256> doc;

void setup() {
  //1 - Para testar, vamos usar a serial
  Serial.begin(9600);

  //2 - iniciamos a conexão WiFi...
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, PASSWD);

  //3 - acesse arduinojson.org/v6/assistant e passe uma amostra pra calcular a capacidade
  const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(8) + 146;
  DynamicJsonDocument doc(capacity);

}

void loop(String status_planta) { 
  Serial.println("loop started...");
        if ((WiFiMulti.run() == WL_CONNECTED)){
          std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
          client->setFingerprint(fingerprint);
          Serial.println("connected...");
          //WiFiClient client;

          HTTPClient http;

        //3 - iniciamos a URL alvo, pega a resposta e finaliza a conexão
        if (http.begin(*client,"https://elastic-maize-cheshire.glitch.me/api/sendgarden?token=9d9048d7-8817-4649-9502-b43c787e6f1f&status=" + status_planta )){
          Serial.println("http.begin ok");
        }
        int httpCode = http.GET();
        if (httpCode > 0) { //Maior que 0, tem resposta a ser lida
            String payload = http.getString();
            Serial.println(httpCode);
            Serial.println(payload);
        }
        else {
          Serial.println(httpCode);
            Serial.println("Falha na requisição");
        }
        http.end();
        }
        delay(5000);
}