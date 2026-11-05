//  PASO 2: Sensor + Serial + Pantalla
//  Incluye lo del paso 1 (leer el SHT31 y mostrar los valores por Serial).
//  Concepto nuevo: mostrar la temperatura y la humedad en la pantalla TFT de la
//  placa T-Display, con colores, tamaños de texto y posiciones, y avisar errores en pantalla.

////////////////////////////  Sección de librerías y variables  ////////////////////////////

#include <Wire.h>
#include "Adafruit_SHT31.h"
#define LGFX_TTGO_TDISPLAY     // Le indica a la librería que usamos la placa TTGO T-Display (pantalla integrada)
#include <LovyanGFX.hpp>       // Librería gráfica para dibujar texto y formas en la pantalla
#include <LGFX_AUTODETECT.hpp> // Configura automáticamente los pines y el controlador de la pantalla

LGFX tft; // Creamos el objeto "tft" que representa a la pantalla
Adafruit_SHT31 sht31 = Adafruit_SHT31();

float t;
float h;
uint16_t violeta = tft.color565(234, 0, 255);    // color565(R, G, B) convierte un color RGB (0-255) al formato de 16 bits de la pantalla
uint16_t azul_claro = tft.color565(0, 183, 255); // Color personalizado para el valor de humedad
uint16_t gris = tft.color565(122, 122, 122);     // Color para las etiquetas "Temp:" y "Hum:"
uint16_t fondo = TFT_BLACK;                      // Color de fondo de la pantalla (TFT_BLACK ya viene definido en la librería)

////////////////////////////  Sección de configuración  ////////////////////////////////////

void setup(void)
{
  Serial.begin(115200);
  tft.init();             // Inicializa la pantalla
  tft.setRotation(0);     // Orientación de la pantalla (0 = vertical)
  tft.setBrightness(128); // Brillo de la pantalla, de 0 (apagada) a 255 (máximo)

  tft.fillScreen(TFT_BLACK);              // Pinta toda la pantalla de negro
  tft.setCursor(0, 0);                    // Coloca el cursor de texto en la esquina superior izquierda (x, y en píxeles)
  tft.setTextSize(2);                     // Tamaño del texto (multiplicador del tamaño base)
  tft.setTextColor(TFT_GREEN, TFT_BLACK); // Color del texto (verde) y de su fondo (negro)

  if (!sht31.begin(0x44))
  {
    Serial.println("No se encontró el sensor SHT31");
    tft.println("ERROR SENSOR"); // También mostramos el error en la pantalla; println escribe y baja de línea
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

    tft.fillScreen(fondo); // Borramos la pantalla antes de dibujar los valores nuevos

    tft.setCursor(20, 20);         // Posición de la etiqueta de temperatura
    tft.setTextColor(gris, fondo); // Texto gris sobre el fondo actual
    tft.setTextSize(3.1);
    tft.print("Temp:"); // print escribe sin saltar de línea
    tft.println("C");

    tft.setCursor(10, 60); // Posición del valor de temperatura
    tft.setTextColor(violeta, fondo);
    tft.setTextSize(5.3); // Texto más grande para resaltar el valor
    tft.print(t, 1);      // Escribe la temperatura con 1 decimal

    tft.setCursor(20, 120);
    tft.setTextColor(gris, fondo);
    tft.setTextSize(3.1);
    tft.print("Hum:");
    tft.println("%");

    tft.setCursor(10, 160);
    tft.setTextColor(azul_claro, fondo);
    tft.setTextSize(5.3);
    tft.print(h, 1);
  }
  else
  {
    Serial.println("Error al leer el sensor SHT31");
    tft.fillScreen(TFT_BLACK); // Limpiamos la pantalla para mostrar solo el aviso
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.setTextColor(TFT_RED, TFT_BLACK); // Rojo para que el error se note
    tft.println("ERROR LECTURA");         // El usuario ve en la pantalla que la lectura falló
  }

  delay(2000);
}

////////////////////////////  Sección de funciones  ////////////////////////////////////////////

// Todavía no hay funciones propias en este paso.
