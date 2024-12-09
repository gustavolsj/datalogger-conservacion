/* 
Datalogger_conservación_v.1.0 escrito por Gustavo Lozano gustavolsj@gmail.com
https://github.com/gustavolsj/datalogger-conservacion

El sketch se compone de tres partes, la sección de librerías y variables, 
la sección de configuración y la sección del ciclo el contenido, 
el codigo de las dos primeras se ejecuta solo una vez cuando encendemos el Arduino, 
mientras que las instrucciones de la tercera se repiten continuamente.

Las librerías SD, RTCLib, Adafruit_SHT31 Y LiquidCrystal_I2C no estan incluidas y deben instalarse desde el menú
Archivo > Herramientas > Manejo de bibliotecas
*/

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////  Sección de librerías y variables  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

#include <SPI.h>              // SPI es el protocolo de comuniación que el módulo SD utiliza 
#include <SD.h>               //Esta es la biblioteca que contiene las funciones del módulo SD
#include "RTClib.h"           //Esta es la biblioteca que contiene las funciones del módulo de reloj 
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27,16,2);  // set the Serial address to 0x27 for a 16 chars and 2 line display

//Periodo de espera entre cada medición, descomentar sólo uno  
long Intervalo =      5000;   //5 SEGUNDOS
//long Intervalo =    60000;   //60 SEGUNDOS
//long Intervalo =   300000;   //5 MINUTOS 
//long Intervalo = 1800000;    //30 MINUTOS  a 'ul' or 'UL' to force the constant into an unsigned long constant. Example: 32767ul


float h;                // Creamos la variable h para almacenar la humedad relativa y 
float t;                 // Creamos la variable t para almacenar la temperatura 

int led = 13;                 // Creamos la variable led y almacenamos el puerto al que se conecta el led que encendera con cada medición 
int chipSelect = 10;          //Esta es una variable relaconada con la conexión SPI del módulo SD

File            myFile;       // Es una instancia para trabajar con el archivo en el que guardaremos las mediciones
RTC_DS1307      rtc;          // Es una instancia para trabajar con el módulo de reloj
Adafruit_SHT31 sht31 = Adafruit_SHT31();   // Es una instancia para trabajar con el sensor
                              // Fin de la importación de bibliotecas y la definición de variables


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////  Sección de configuración  ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void setup(){
  pinMode(led, OUTPUT);       //Indicamos que el puerto al que conectamos el led es una salida
  
  Serial.begin(9600);        // Iniciamos la comunicación con la computadora a traves del puerto serie a la velocidad especificada
  Serial.print("Iniciando tarjeta SD."); //Mostramos el mensaje de inicio

    if (!SD.begin(chipSelect)) {  // Si la tarjeta SD no funciona correctamente imprimimos el siguiente mensaje
    Serial.println("Error en la tarjeta SD");
    return;                   // Y el programa se detiene hasta aquí
  }
  Serial.println(" Tarjeta lista"); // En cambio si la tarjeta SD funciona correctamente imprimimos estos tres mensajes
  Serial.println("SHT31 Sensor de Humedad Relativa y temperatura\n");
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
    lcd.init();                      // initialize the lcd 
    // Print a message to the lcd.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Iniciando SDCard");
  lcd.setCursor(0,1);
  lcd.print("Iniciando Sensor");

  
  sht31.begin(0x44);  // Iniciamos el sensor  
  delay(2000);//Wait before accessing Sensors
lcd.clear(); 


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
h = sht31.readHumidity(); // Solicitamos la humedad actual al sensor
t = sht31.readTemperature();  // Solicitamos la temperatura actual al sensor


Serial.print(h);                // imprimimos la humedad actual al sensor
Serial.print(" %");                   // imprimimos el simbolo de porcentaje
Serial.print(", ");                   // Imprimimos una coma y un espacio

Serial.print(t);                 // Imprimimos la temperatura actual
Serial.print(" C");                   // Imprimimos el simbolo de grados centigrados
Serial.print(" ");                    // Imprimimos un espacio
 
myFile = SD.open("SHT31.txt", FILE_WRITE); // Le indicamos al módulo SD crear o abrir el archivo llamado SHT31.txt en modo escritura
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
    myFile.print(h);            // imprimimos la humedad actual al sensor
    myFile.print(",");                // Imprimimos una coma y un espacio
    
    myFile.print(t);             // Imprimimos la temperatura actual
    myFile.println();
 
    myFile.close();                   // Le indicamos al módulo SD cerrar el archivo
    Serial.println("    listo!.");    //Imprimimos el mensaje listo

lcd.clear(); 
  lcd.setCursor(0,0);
  
    lcd.print(now.year(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.day(), DEC);
    lcd.print("  ");
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
     
lcd.setCursor(0,1);
// SHT-31
lcd.print(h);
lcd.print(" %");
lcd.print(", ");

lcd.print(t); 
lcd.print(" C");

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
