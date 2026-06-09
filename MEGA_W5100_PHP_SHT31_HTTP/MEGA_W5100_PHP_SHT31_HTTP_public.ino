#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

// Ejemplo publico para Arduino Mega + Ethernet Shield W5100 + sensor SHT31.
// Antes de usarlo, reemplaza la MAC, las IP y la ruta HTTP por los valores de tu red.

// Usa una MAC unica en tu red local. Evita reutilizar la MAC de otro dispositivo.
byte mac[] = { 0x02, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE };

// IP estatica de respaldo si el DHCP no responde.
IPAddress fallbackIp(192, 168, 1, 200);
IPAddress dns(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Cambia esta IP por la del servidor que recibe los datos.
IPAddress serverIp(192, 168, 1, 100);
const uint16_t serverPort = 80;

// Cambia esta ruta por el endpoint real de tu servidor.
const char serverPath[] = "/api/temperatura";

// Intervalo de envio: 3 minutos.
const unsigned long interval = 3UL * 60UL * 1000UL;

unsigned long previousMillis = 0;
float temperature = NAN;
float humidity = NAN;

EthernetClient client;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void inicializarEthernet();
void enviarDatos();
bool leerSensor();
void imprimirRespuesta();
void imprimirDiagnosticoRed();

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }

  Wire.begin();

  // El sensor SHT31 suele responder en la direccion I2C 0x44.
  if (!sht31.begin(0x44)) {
    Serial.println(F("No se encontro el sensor SHT31."));
    while (true) {
      delay(1000);
    }
  }

  inicializarEthernet();
  Serial.println(F("MEGA_W5100_SHT31_HTTP_public listo."));
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    enviarDatos();
  }
}

void inicializarEthernet() {
  // En Arduino Mega, el pin 53 debe permanecer como salida para conservar SPI en modo maestro.
  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);

  // En muchos shields W5100, la SD comparte el bus SPI. La dejamos desactivada para evitar conflictos.
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  // El chip select del W5100 suele estar en el pin 10.
  Ethernet.init(10);

  Serial.println(F("Inicializando Ethernet por DHCP..."));
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("DHCP fallo. Se usara IP estatica de respaldo."));
    Ethernet.begin(mac, fallbackIp, dns, gateway, subnet);
  }

  delay(1000);

  Serial.print(F("IP local: "));
  Serial.println(Ethernet.localIP());
  imprimirDiagnosticoRed();
}

void enviarDatos() {
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println(F("No se detecto el hardware Ethernet."));
    return;
  }

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println(F("Sin enlace Ethernet. Revisa cable o switch."));
    return;
  }

  if (!leerSensor()) {
    Serial.println(F("Lectura invalida del SHT31."));
    return;
  }

  Serial.print(F("Temp: "));
  Serial.print(temperature, 2);
  Serial.print(F(" C, Hum: "));
  Serial.print(humidity, 2);
  Serial.println(F(" %"));

  // Cierra cualquier sesion TCP previa antes de iniciar una nueva solicitud.
  client.stop();

  Serial.print(F("Conectando a "));
  Serial.print(serverIp);
  Serial.print(F(":"));
  Serial.println(serverPort);

  if (!client.connect(serverIp, serverPort)) {
    Serial.println(F("No se pudo conectar al servidor HTTP."));
    imprimirDiagnosticoRed();
    return;
  }

  // Envia una peticion GET simple con temperatura y humedad en la query string.
  client.print(F("GET "));
  client.print(serverPath);
  client.print(F("?temperatura="));
  client.print(temperature, 2);
  client.print(F("&humedad="));
  client.print(humidity, 2);
  client.println(F(" HTTP/1.1"));
  client.print(F("Host: "));
  client.println(serverIp);
  client.println(F("Connection: close"));
  client.println();

  Serial.println(F("Solicitud enviada. Respuesta:"));
  imprimirRespuesta();
  client.stop();
}

bool leerSensor() {
  humidity = sht31.readHumidity();
  temperature = sht31.readTemperature();

  // Si alguna lectura es NaN, se descarta el envio para no subir datos corruptos.
  if (isnan(humidity) || isnan(temperature)) {
    return false;
  }

  return true;
}

void imprimirRespuesta() {
  unsigned long timeout = millis();

  // Lee la respuesta del servidor durante un maximo de 5 segundos.
  while (client.connected() && millis() - timeout < 5000UL) {
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
      timeout = millis();
    }
  }

  Serial.println();
}

void imprimirDiagnosticoRed() {
  Serial.print(F("Hardware Ethernet: "));
  switch (Ethernet.hardwareStatus()) {
    case EthernetNoHardware:
      Serial.println(F("no detectado"));
      break;
    case EthernetW5100:
      Serial.println(F("W5100"));
      break;
    case EthernetW5200:
      Serial.println(F("W5200"));
      break;
    case EthernetW5500:
      Serial.println(F("W5500"));
      break;
    default:
      Serial.println(F("desconocido"));
      break;
  }

  Serial.print(F("Enlace Ethernet: "));
  switch (Ethernet.linkStatus()) {
    case LinkON:
      Serial.println(F("activo"));
      break;
    case LinkOFF:
      Serial.println(F("sin enlace"));
      break;
    default:
      Serial.println(F("desconocido"));
      break;
  }

  Serial.print(F("IP local: "));
  Serial.println(Ethernet.localIP());
  Serial.print(F("Gateway: "));
  Serial.println(Ethernet.gatewayIP());
  Serial.print(F("DNS: "));
  Serial.println(Ethernet.dnsServerIP());
  Serial.print(F("Servidor HTTP: "));
  Serial.print(serverIp);
  Serial.print(F(":"));
  Serial.println(serverPort);
}