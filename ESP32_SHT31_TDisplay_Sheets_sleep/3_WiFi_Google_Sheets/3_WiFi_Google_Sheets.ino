//  PASO 3: Sensor + Serial + Pantalla + WiFi y Google Sheets
//  Incluye lo del paso 1 (leer el SHT31 y mostrar por Serial) y lo del paso 2 (mostrar
//  los valores en la pantalla T-Display).
//  Concepto nuevo: conectarse a una red WiFi y enviar los datos por HTTPS (formato JSON)
//  a una hoja de Google Sheets mediante un Apps Script.

////////////////////////////  Sección de librerías y variables  ////////////////////////////

#include <Wire.h>
#include <WiFi.h>             // Librería para conectar el ESP32 a una red WiFi
#include <HTTPClient.h>       // Librería para hacer peticiones HTTP (enviar datos a internet)
#include <WiFiClientSecure.h> // Permite conexiones seguras (HTTPS), necesarias para Google
#include "Adafruit_SHT31.h"
#define LGFX_TTGO_TDISPLAY
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

const char *nombre_red = "TU_RED_WIFI";                                             // Nombre (SSID) de la red WiFi a la que nos conectaremos
const char *contrasena_red = "TU_CONTRASENA";                                       // Contraseña de esa red
const char *url_script = "https://script.google.com/macros/s/TU_ID_DE_SCRIPT/exec"; // URL del Apps Script que recibe los datos y los guarda en la hoja

LGFX tft;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

float t;
float h;
uint16_t violeta = tft.color565(234, 0, 255);
uint16_t azul_claro = tft.color565(0, 183, 255);
uint16_t gris = tft.color565(122, 122, 122);
uint16_t fondo = TFT_BLACK;

bool conectarWiFi(); // Prototipos: avisamos al compilador que estas funciones se definen más abajo
void enviarDatos(float temperatura, float humedad);

////////////////////////////  Sección de configuración  ////////////////////////////////////

void setup(void)
{
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  tft.setBrightness(128);

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  if (!sht31.begin(0x44))
  {
    Serial.println("No se encontró el sensor SHT31");
    tft.println("ERROR SENSOR");
    while (1)
      delay(1000);
  }
}

////////////////////////////  Sección de ciclo  ////////////////////////////////////////////

void loop()
{
  h = sht31.readHumidity();
  t = sht31.readTemperature();

  if (!isnan(t) && !isnan(h))
  {
    Serial.printf("Temp: %.1f C, Hum: %.1f %%\n", t, h);

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

    if (conectarWiFi())
    {                        // Solo enviamos datos si logramos conectarnos a la red
      enviarDatos(t, h);     // Mandamos temperatura y humedad a Google Sheets
      WiFi.disconnect(true); // Nos desconectamos del WiFi para ahorrar energía
    }
  }
  else
  {
    Serial.println("Error al leer el sensor SHT31");
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("ERROR LECTURA");
  }

  delay(60000); // Esperamos 60 s entre cada medición y envío
}

////////////////////////////  Sección de funciones  ////////////////////////////////////////////

bool conectarWiFi()
{ // Devuelve true si hay conexión y false si no se pudo conectar
  if (WiFi.status() == WL_CONNECTED)
  { // Si ya estamos conectados no hace falta reconectar
    return true;
  }

  Serial.printf("Conectando a WiFi: %s\n", nombre_red);
  WiFi.begin(nombre_red, contrasena_red); // Inicia la conexión con la red

  const unsigned long inicio = millis(); // millis() = milisegundos desde que arrancó la placa; guardamos el momento de inicio
  while (WiFi.status() != WL_CONNECTED && millis() - inicio < 10000)
  { // Esperamos hasta conectar o hasta 10 s (timeout)
    delay(250);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.printf("WiFi conectado. IP: %s\n", WiFi.localIP().toString().c_str()); // Mostramos la IP que nos asignó el router
    return true;
  }

  Serial.println("No se pudo conectar a WiFi");
  WiFi.disconnect(true);
  return false;
}

void enviarDatos(float temperatura, float humedad)
{
  WiFiClientSecure client; // Cliente para conexiones HTTPS
  client.setInsecure();    // No valida el certificado del servidor (simplifica el taller; no recomendado en producción)

  HTTPClient http;
  if (!http.begin(client, url_script))
  { // Prepara la petición hacia la URL del script
    Serial.println("No se pudo iniciar la conexion HTTPS");
    return;
  }

  http.addHeader("Content-Type", "application/json");          // Avisamos que el contenido que enviamos es JSON
  http.setTimeout(10000);                                      // Tiempo máximo de espera de la respuesta: 10 s
  String json = "{\"temperature\":" + String(temperatura, 2) + // Armamos el texto JSON: {"temperature":22.50,"humidity":45.10}
                ",\"humidity\":" + String(humedad, 2) + "}";
  int codigo_http = http.POST(json); // Enviamos los datos con POST; devuelve el código de respuesta (200 = OK, negativo = error)

  Serial.printf("Google Sheets HTTP: %d\n", codigo_http);
  if (codigo_http > 0)
  {
    Serial.println(http.getString()); // Mostramos la respuesta del servidor
  }
  http.end(); // Cerramos la conexión y liberamos recursos
}
