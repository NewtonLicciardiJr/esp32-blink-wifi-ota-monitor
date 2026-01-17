// Bibliotecas necessárias para o programa
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

// Define o pino do LED embutido se não estiver definido
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// Credenciais do Wi-Fi
const char* WIFI_SSID = "xxx";
const char* WIFI_PASS = "xxx";

// Configurações do OTA (Over-The-Air)
const char* OTA_HOSTNAME = "esp32-blink";   // nome na rede
const char* OTA_PASS     = "123456";        // opcional (pode deixar "")

// Variáveis para controlar o blink do LED
unsigned long lastBlink = 0;
bool ledState = false;

// Função para conectar ao Wi-Fi
void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Conectando no Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi OK");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Função para configurar o OTA (atualização over-the-air)
void setupOta() {
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  if (OTA_PASS && OTA_PASS[0] != '\0') ArduinoOTA.setPassword(OTA_PASS);

  ArduinoOTA.onStart([]() {
    Serial.println("OTA start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA end");
  });
  ArduinoOTA.onProgress([](unsigned int p, unsigned int t) {
    Serial.printf("OTA: %u%%\r", (p * 100) / t);
  });
  ArduinoOTA.onError([](ota_error_t e) {
    Serial.printf("OTA error[%u]\n", e);
  });

  ArduinoOTA.begin();
  Serial.println("OTA pronto");
}

// Função de configuração inicial
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  setupWifi();
  setupOta();
}

// Loop principal do programa
void loop() {
  // OTA precisa ser atendido sempre
  ArduinoOTA.handle();

  // Blink sem delay (fica responsivo pro OTA)
  if (millis() - lastBlink >= 600) {
    lastBlink = millis();
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
  }
}
