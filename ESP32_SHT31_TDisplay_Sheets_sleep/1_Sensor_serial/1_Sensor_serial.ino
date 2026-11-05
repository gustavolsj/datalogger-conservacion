//  PASO 1: Sensor + Serial
//  Concepto nuevo: leer el sensor SHT31 (temperatura y humedad) por I2C y mostrar
//  los valores en el monitor serial de la computadora.

////////////////////////////  Sección de librerías y variables  ////////////////////////////

#include <Wire.h>           // Librería para la comunicación I2C (el sensor se conecta por este protocolo)
#include "Adafruit_SHT31.h" // Librería del fabricante con las funciones para leer el sensor SHT31

Adafruit_SHT31 sht31 = Adafruit_SHT31(); // Creamos el objeto "sht31" que representa a nuestro sensor

float t; // Variable de tipo flotante (con decimales) para guardar en memoria la lectura de la temperatura
float h; // Variable de tipo flotante para guardar en memoria la lectura de la humedad

////////////////////////////  Sección de configuración  ////////////////////////////////////

void setup(void)
{                       // setup() se ejecuta una sola vez, al encender o reiniciar la placa
  Serial.begin(115200); // Inicia la comunicación serial con la computadora a 115200 baudios

  if (!sht31.begin(0x44))
  {                                                   // Intenta iniciar el sensor en la dirección I2C 0x44; si falla, entra al if
    Serial.println("No se encontró el sensor SHT31"); // Avisamos del error en el monitor serial
    while (1)
      delay(1000); // Ciclo infinito: detiene el programa porque sin sensor no hay nada que hacer
  }
}

////////////////////////////  Sección de ciclo  ////////////////////////////////////////////

void loop()
{                              // loop() se repite indefinidamente después de setup()
  h = sht31.readHumidity();    // Pedimos al sensor la humedad relativa (%) y la guardamos en h
  t = sht31.readTemperature(); // Pedimos al sensor la temperatura (°C) y la guardamos en t

  if (!isnan(t) && !isnan(h))
  {                                                      // isnan() detecta lecturas inválidas ("Not a Number"); entramos si ambas son válidas
    Serial.printf("Temp: %.1f C, Hum: %.1f %%\n", t, h); // Imprime los valores con 1 decimal (%.1f); \n es salto de línea
  }
  else
  {
    Serial.println("Error al leer el sensor SHT31"); // Si alguna lectura es inválida, lo avisamos
  }

  delay(2000); // Esperamos 2000 ms (2 s) antes de volver a medir
}

////////////////////////////  Sección de funciones  ////////////////////////////////////////////

// Todavía no hay funciones propias en este paso.
