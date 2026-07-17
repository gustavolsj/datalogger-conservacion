/* 
Datalogger_conservación_v.1.0 escrito por Gustavo Lozano
https://github.com/gustavolsj/datalogger-conservacion

Este sketch acompaña a la charla 
https://www.youtube.com/watch?v=n_qbAQeKkKo&t=6162s
y la publicación "Tecnología Maker y su aplicación en conservación" 
donde se describe el proceso de fabricación de un datalogger de temperatura y humedad relativa
para ser usado en la conservación preventiva de un archivo histórico. 

El sketch se compone de tres partes, la sección de librerías y variables, 
la sección de configuración y la sección del ciclo el contenido, 
el codigo de las dos primeras se ejecuta solo una vez cuando encendemos el Arduino, 
mientras que las instrucciones de la tercera se repiten continuamente.

Las librerías SD, STCLib, Adafruit_Sensor y Adafruit_BME280 no estan incluidas y deben instalarse desde el menú
Archivo > Herramientas > Manejo de bibliotecas
*/

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////  Sección de librerías y variables  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

#include <SPI.h>              // SPI es el protocolo de comuniación que el módulo SD utiliza 
#include <SD.h>               //Esta es la biblioteca que contiene las funciones del módulo SD
#include "RTClib.h"           //Esta es la biblioteca que contiene las funciones del módulo de reloj 
#include <Wire.h>             //Wire o I2C es el protocolo de comuniación que el sensor BME280 utiliza
#include <Adafruit_Sensor.h>  //Esta es una librería que contiene las funciones del sensor BME280
#include <Adafruit_BME280.h>  //Esta es otra librería que contiene las funciones del sensor BME280

//Periodo de espera entre cada medición, descomentar sólo uno  
long Intervalo =      5000;   //5 SEGUNDOS
//long Intervalo =    60000;   //60 SEGUNDOS
//long Intervalo =   300000;   //5 MINUTOS 
//long Intervalo = 1800000;    //30 MINUTOS  a 'ul' or 'UL' to force the constant into an unsigned long constant. Example: 32767ul


float bmehr_0;                // Creamos la variable bmehr_0 para almacenar la humedad relativa y 
float bmet_0;                 // Creamos la variable bmet_0 para almacenar la temperatura 

int led = 13;                 // Creamos la variable led y almacenamos el puerto al que se conecta el led que encendera con cada medición 
int chipSelect = 10;          //Esta es una variable relaconada con la conexión SPI del módulo SD

File            myFile;       // Es una instancia para trabajar con el archivo en el que guardaremos las mediciones
RTC_DS1307      rtc;          // Es una instancia para trabajar con el módulo de reloj
Adafruit_BME280 bme_0;        // Es una instancia para trabajar con el sensor
                              // Fin de la importación de bibliotecas y la definición de variables


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////  Sección de configuración  ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void setup(){
  pinMode(led, OUTPUT);       //Indicamos que el puerto al que conectamos el led es una salida
  
  Serial.begin(57600);        // Iniciamos la comunicación con la computadora a traves del puerto serie a la velocidad especificada
  Serial.print("Iniciando tarjeta SD"); //Mostramos el mensaje de inicio

    if (!SD.begin(chipSelect)) {  // Si la tarjeta SD no funciona correctamente imprimimos el siguiente mensaje
    Serial.println("Error en la tarjeta SD");
    return;                   // Y el programa se detiene hasta aquí
  }
  Serial.println("Tarjeta lista"); // En cambio si la tarjeta SD funciona correctamente imprimimos estos tres mensajes
  Serial.println("BME280 Sensor de Humedad Relativa y temperatura\n");
  Serial.println("Fecha y Hora, Humedad Relativa %, Temperatura °C");
  
  if (! rtc.begin()) {        // Si el módulo de reloj no inicia correctamente imprimimos el siguiente mensaje
    Serial.println("Error en el inicio del reloj");
    while (1);                // Y el programa se detiene hasta aquí
  }

  if (! rtc.isrunning()) {    // Si el módulo de reloj inicia pero no se ejecuta correctamente imprimimos el siguiente mensaje
    Serial.println("Error en la ejecución del reloj");  
  }

// La siguientes lineas deben permanecer siempre comentadas a menos que se quiera configurar la fecha y hora del reloj por unica vez
// Para configurar la siguiente fecha: 2020, Junio 17, a las 12 con 40 minutos y 0 segundos
//rtc.adjust(DateTime(2020, 6, 17, 12, 40, 0)); // Aqui sustituir los datos correpondientes 
//rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Esta linea hace lo mismo que la anterior usando la fecha y hora de la computadora
     
  delay(2000);              // Finalmente esperamos dos segundos o dos mil milisegundos
  bme_0.begin();            // Iniciamos el sensor
}                           // Fin de la configuración


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////  Sección de ciclo  ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void loop(){
   DateTime now = rtc.now();          // Solictamos la fecha y hora actual al reloj
    
    Serial.print(now.year(), DEC);    // Imprimimos el año
    Serial.print('/');                // Imprimimos una diagonal
    Serial.print(now.month(), DEC);   // Imprimimos el mes
    Serial.print('/');                // Imprimimos una diagonal
    Serial.print(now.day(), DEC);     // Imprimimos el día
    Serial.print("  ");               // Imprimimos una diagonal
    Serial.print(now.hour(), DEC);    // Imprimimos la hora
    Serial.print(':');                // Imprimimos una diagonal
    Serial.print(now.minute(), DEC);  // Imprimimos el minuto
    Serial.print(':');                // Imprimimos una diagonal
    Serial.print(now.second(), DEC);  // Imprimimos el segundo
    Serial.print(",  ");              // Imprimimos una coma y un espacio

//BME
bmehr_0 = bme_0.readHumidity();     // Solictamos la humedad actual al sensor
bmet_0 = bme_0.readTemperature();   // Solictamos la temperatura actual al sensor

Serial.print(bmehr_0);                // imprimimos la humedad actual al sensor
Serial.print(" %");                   // imprimimos el simbolo de porcentaje
Serial.print(", ");                   // Imprimimos una coma y un espacio

Serial.print(bmet_0);                 // Imprimimos la temperatura actual
Serial.print(" C");                   // Imprimimos el simbolo de grados centigrados
Serial.print(" ");                    // Imprimimos un espacio
 
myFile = SD.open("BME280.txt", FILE_WRITE); // Le indicamos al módulo SD crear o abrir el archivo llamado BME280.txt en modo escritura
if (myFile) {                         // Si el archivo abre correctamente imprimimos el siguiente mensaje
    Serial.print("Escribiendo en archivo...");
    
    myFile.print(now.year(), DEC);// Imprimimos el año
    myFile.print('/');                // Imprimimos una diagonal
    myFile.print(now.month(), DEC);   // Imprimimos el mes
    myFile.print('/');                // Imprimimos una diagonal
    myFile.print(now.day(), DEC);     // Imprimimos el día
    myFile.print("  ");               // Imprimimos una diagonal
    myFile.print(now.hour(), DEC);    // Imprimimos la hora
    myFile.print(':');                // Imprimimos una diagonal
    myFile.print(now.minute(), DEC);  // Imprimimos el minuto
    myFile.print(':');                // Imprimimos una diagonal
    myFile.print(now.second(), DEC);  // Imprimimos el segundo
    myFile.print(",  ");              // Imprimimos una coma y un espacio

//BME
    myFile.print(bmehr_0);            // imprimimos la humedad actual al sensor
    myFile.print(",");                // Imprimimos una coma y un espacio
    
    myFile.print(bmet_0);             // Imprimimos la temperatura actual
    myFile.println();
 
    myFile.close();                   // Le indicamos al módulo SD cerrar el archivo
    Serial.println("    listo!.");    //Imprimimos el mensaje listo

   digitalWrite(led, HIGH);   // Encendemos el led
   delay(600);                // Esperamos un momento
   digitalWrite(led, LOW);    // Apagamos el led
   delay(600);                // Esperamos un momento
   digitalWrite(led, HIGH);   // Encendemos el led
   delay(600);                // Esperamos un momento
   digitalWrite(led, LOW);    // Apagamos el led
   delay(600);
  
 } else {                  // Si el archivo no abrió correctamente imprimimos el siguiente mensaje
    Serial.println("Error abriendo archivo");
}

delay(Intervalo);       //Aqui esperamos el tiempo definido al inicio antes de la siguiente medición.
}                       // Fin del ciclo
