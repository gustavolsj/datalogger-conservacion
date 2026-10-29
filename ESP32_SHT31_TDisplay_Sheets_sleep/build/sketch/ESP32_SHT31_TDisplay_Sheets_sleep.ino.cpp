#include <Arduino.h>
#line 1 "G:\\My Drive\\Trabajo\\IIE\\Actividades_Externas\\Digital\\Arduino\\03 Data logger Arduino\\sketches_datalogger\\PLACAS\\LiliGo_TDisplay\\ESP32_SHT31_TDisplay_Sheets_sleep\\ESP32_SHT31_TDisplay_Sheets_sleep.ino"
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "Adafruit_SHT31.h"
#define LGFX_TTGO_TDISPLAY 
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

//const char* nombre_red = "test";
//const char* contrasena_red = "testest";
const char* nombre_red = "INFINITUM7D09";
const char* contrasena_red = "FtMGgeuu2F";
const char* url_script = "https://script.google.com/macros/s/AKfycbxkOabaKtNJl7yB2IezNog1Ux-Lp0RO9RzBjGde_3dIU8EBx73aDjyJlzn76OhKGrf0dA/exec";


#define TIEMPO_SLEEP 1 * 60 * 1000000ULL
#define VBAT_PIN 34

LGFX tft;                // Instancia definida en lgfx_user.h
Adafruit_SHT31 sht31 = Adafruit_SHT31();

float t;
float h;
RTC_DATA_ATTR bool estado = false;
uint16_t amarillo = tft.color565(242, 255, 0);
uint16_t violeta = tft.color565(234, 0, 255);
uint16_t azul_claro = tft.color565(0, 183, 255);
uint16_t gris = tft.color565(122, 122, 122);
uint16_t fondo;

bool conectarWiFi();
float leerBateria();
void enviarDatos(float temperatura, float humedad, float bateria);
void apagarPantalla();

#line 38 "G:\\My Drive\\Trabajo\\IIE\\Actividades_Externas\\Digital\\Arduino\\03 Data logger Arduino\\sketches_datalogger\\PLACAS\\LiliGo_TDisplay\\ESP32_SHT31_TDisplay_Sheets_sleep\\ESP32_SHT31_TDisplay_Sheets_sleep.ino"
void setup(void);
#line 114 "G:\\My Drive\\Trabajo\\IIE\\Actividades_Externas\\Digital\\Arduino\\03 Data logger Arduino\\sketches_datalogger\\PLACAS\\LiliGo_TDisplay\\ESP32_SHT31_TDisplay_Sheets_sleep\\ESP32_SHT31_TDisplay_Sheets_sleep.ino"
void loop();
#line 38 "G:\\My Drive\\Trabajo\\IIE\\Actividades_Externas\\Digital\\Arduino\\03 Data logger Arduino\\sketches_datalogger\\PLACAS\\LiliGo_TDisplay\\ESP32_SHT31_TDisplay_Sheets_sleep\\ESP32_SHT31_TDisplay_Sheets_sleep.ino"
void setup(void) {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  tft.setBrightness(128);

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  if (!sht31.begin(0x44)) {
    Serial.println("No se encontró el sensor SHT31");
    tft.println("ERROR SENSOR");
    delay(2000);
    apagarPantalla();
    esp_deep_sleep(TIEMPO_SLEEP);
  }

  String mensaje = "Start...................";
  for (int i = 0; i < mensaje.length(); i++) {
    tft.print(mensaje[i]);
    delay(150);
  }
  delay(200);

  h = sht31.readHumidity();
  t = sht31.readTemperature();
  float bateria = leerBateria();

  estado = !estado;
  fondo = estado ? TFT_BLACK : TFT_WHITE;

  Serial.printf("Temp: %.1f C, Hum: %.1f %%, Bat: %.2f V\n", t, h, bateria);

  tft.fillScreen(fondo);

  tft.setCursor(20, 20);
  tft.setTextColor(gris, fondo);
  tft.setTextSize(3.1);
  tft.print("Temp:");
  tft.println("C");

  tft.setCursor(10, 60);
  tft.setTextColor(violeta, fondo);
  tft.setTextSize(5.3);
  tft.print(t, 1);


  tft.setCursor(20, 120);
  tft.setTextColor(gris, fondo);
  tft.setTextSize(3.1);
  tft.print("Hum:");
  tft.println("%");

  tft.setCursor(10, 160);
  tft.setTextColor(azul_claro, fondo);
  tft.setTextSize(5.3);
  tft.print(h, 1);

  if (!isnan(t) && !isnan(h)) {
    if (conectarWiFi()) {
      enviarDatos(t, h, bateria);
      WiFi.disconnect(true);
    }
  }
  else {
    Serial.println("Error al leer el sensor SHT31");
  }

  delay(2000);
  apagarPantalla();
  Serial.println("Entrando en deep sleep...");
  esp_deep_sleep(TIEMPO_SLEEP);
}

void loop() {
  // El ESP32 reinicia setup() al despertar del deep sleep.
}

bool conectarWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }

  Serial.printf("Conectando a WiFi: %s\n", nombre_red);
  WiFi.begin(nombre_red, contrasena_red);

  const unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - inicio < 10000) {
    delay(250);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("WiFi conectado. IP: %s\n", WiFi.localIP().toString().c_str());
    return true;
  }

  Serial.println("No se pudo conectar a WiFi");
  WiFi.disconnect(true);
  return false;
}

float leerBateria() {
  uint32_t lectura = analogRead(VBAT_PIN);
  return (lectura / 4095.0) * 2.0 * 3.3;
}

void enviarDatos(float temperatura, float humedad, float bateria) {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  if (!http.begin(client, url_script)) {
    Serial.println("No se pudo iniciar la conexion HTTPS");
    return;
  }

  http.addHeader("Content-Type", "application/json");
  http.setTimeout(10000);
  String json = "{\"temperature\":" + String(temperatura, 2) +
    ",\"humidity\":" + String(humedad, 2) +
    ",\"battery\":" + String(bateria, 2) + "}";
  int codigo_http = http.POST(json);

  Serial.printf("Google Sheets HTTP: %d\n", codigo_http);
  if (codigo_http > 0) {
    Serial.println(http.getString());
  }
  http.end();
}

void apagarPantalla() {
  tft.fillScreen(TFT_BLACK);
  tft.setBrightness(0);
  tft.sleep();
}
