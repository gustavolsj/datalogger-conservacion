#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "Adafruit_SHT31.h" 

// Configuracion de la red Wi-Fi
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

// URL del servidor PHP
const char* serverName = "https://NOMBRE_SERVIDOR/insertar_datos.php";


// Intervalo de tiempo entre cada envio de datos (en milisegundos)
long interval = 30 * 60000; // MINUTOS x millis
//long interval = 30 * 6000; // SEGUNDOS x millis

unsigned long previousMillis = 0;
float t;
float h;

Adafruit_SHT31 sht31 = Adafruit_SHT31();   // Es una instancia para trabajar con el sensor

void setup() {
  Serial.begin(115200);
  sht31.begin(0x44);  // Iniciamos el sensor  

  // Conexion a la red Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("ESP32_PHP_SHT31_HTTP ------ Conectando a WiFi...");
  }
  Serial.println("Conectado a la red WiFi.");
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    enviarDatos();
  }
}

void enviarDatos() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Generar el dato numerico a enviar
    //int datoNumerico = random(0, 100); // Ejemplo de dato numerico aleatorio

  h = sht31.readHumidity(); // Solicitamos la humedad actual al sensor
  t = sht31.readTemperature();  // Solicitamos la temperatura actual al sensor
  Serial.print((String)"Temp:  " + t + "°C,  ");
  Serial.println((String)"Hum:  " + h + "%,  ");

    // Crear la URL con los parametros
    //http://NOMBRE_SERVIDOR/insertar_datos.php?temperatura=24&humedad=59
    String serverPath = serverName + String("?temperatura=") + String(t) + String("&humedad=") + String(h);

    // Enviar la solicitud HTTP GET
    http.begin(serverPath.c_str());
    Serial.println(serverPath);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("Respuesta del servidor: " + payload);
    } else {
      Serial.println("Error en la solicitud HTTP: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("Error: No conectado a WiFi.");
  }
}
