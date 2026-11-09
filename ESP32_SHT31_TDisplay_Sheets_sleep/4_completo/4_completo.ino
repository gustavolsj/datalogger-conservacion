////////////////////////////////////////////////////////////////////////////////////////////
//  PASO 4: Sensor + Serial + Pantalla + WiFi/Google Sheets + Deep sleep y batería
//  Incluye lo de los pasos 1, 2 y 3 (leer el SHT31, mostrar por Serial y en la pantalla
//  T-Display, y enviar los datos por WiFi a Google Sheets).
//  Conceptos nuevos: ahorro de energía con deep sleep (la placa duerme y despierta sola),
//  lectura del voltaje de la batería, memoria que sobrevive al sueño (RTC_DATA_ATTR)
//  para alternar el color de fondo, y apagado de la pantalla antes de dormir.
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////  Sección de librerías y variables  ////////////////////////////

#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "Adafruit_SHT31.h"
#define LGFX_TTGO_TDISPLAY 
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

const char* nombre_red = "TU_RED_WIFI";
const char* contrasena_red = "TU_CONTRASENA";
const char* url_script = "https://script.google.com/macros/s/TU_ID_DE_SCRIPT/exec";


#define TIEMPO_SLEEP 5 * 60 * 1000000ULL   // Tiempo que dormirá la placa: 5 min expresados en microsegundos
#define VBAT_PIN 34                        // Pin analógico donde se lee el voltaje de la batería

LGFX tft;                // Instancia definida en lgfx_user.h
Adafruit_SHT31 sht31 = Adafruit_SHT31();

float t;
float h;
RTC_DATA_ATTR bool estado = false;   // RTC_DATA_ATTR guarda la variable en memoria que NO se borra durante el deep sleep
uint16_t amarillo = tft.color565(242, 255, 0);
uint16_t violeta = tft.color565(234, 0, 255);
uint16_t azul_claro = tft.color565(0, 183, 255);
uint16_t gris = tft.color565(122, 122, 122);
uint16_t fondo;

bool conectarWiFi();
float leerBateria();
void enviarDatos(float temperatura, float humedad, float bateria);
void apagarPantalla();

////////////////////////////  Sección de configuración  ////////////////////////////////////

void setup(void) {   // Con deep sleep, setup() se ejecuta en cada despertar: aquí va todo el trabajo
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
    apagarPantalla();               // Apagamos la pantalla para no gastar batería
    esp_deep_sleep(TIEMPO_SLEEP);   // En lugar del while(1), dormimos y reintentamos al despertar
  }

  String mensaje = "Start...................";   // Animación de arranque: escribimos el texto letra por letra
  for (int i = 0; i < mensaje.length(); i++) {
    tft.print(mensaje[i]);
    delay(150);
  }
  delay(200);

  h = sht31.readHumidity();
  t = sht31.readTemperature();
  float bateria = leerBateria();   // Leemos el voltaje de la batería con la función definida abajo

  estado = !estado;                          // Invertimos el valor guardado en memoria RTC en cada despertar
  fondo = estado ? TFT_BLACK : TFT_WHITE;    // Operador ternario: si estado es true el fondo es negro, si no, blanco

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
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("ERROR LECTURA");
  }

  delay(2000);
  apagarPantalla();
  Serial.println("Entrando en deep sleep...");
  esp_deep_sleep(TIEMPO_SLEEP);   // Duerme la placa TIEMPO_SLEEP microsegundos; al despertar reinicia desde setup()
}


////////////////////////////  Sección de ciclo  ////////////////////////////////////////////


void loop() {
  // El ESP32 reinicia setup() al despertar del deep sleep, por eso loop() queda vacío.
}



////////////////////////////  Sección de funciones  ////////////////////////////////////////////


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
  uint32_t lectura = analogRead(VBAT_PIN);   // Lectura del ADC: valor de 0 a 4095 según el voltaje en el pin
  return (lectura / 4095.0) * 2.0 * 3.3;     // Lo convertimos a voltios: 3.3 V es la referencia y *2 compensa el divisor de tensión de la placa
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
    ",\"battery\":" + String(bateria, 2) + "}";   // Nuevo campo "battery" en el JSON
  int codigo_http = http.POST(json);

  Serial.printf("Google Sheets HTTP: %d\n", codigo_http);
  if (codigo_http > 0) {
    Serial.println(http.getString());
  }
  http.end();
}

void apagarPantalla() {
  tft.fillScreen(TFT_BLACK);   // Borramos lo que se muestra
  tft.setBrightness(0);        // Brillo al mínimo
  tft.sleep();                 // Ponemos la pantalla en modo de bajo consumo
}