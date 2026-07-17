# sketches_datalogger

![Datalogger compacto](/imagenes/TTGO_LCD.jpg)

Este repositorio reúne una colección de sketches desarrollados para proyectos de datalogging, monitoreo remoto y visualización de variables ambientales en plataformas Arduino y ESP. El enfoque principal está en la medición de temperatura, humedad, voltaje y otras señales asociadas al funcionamiento de sistemas de monitoreo, con distintas estrategias de almacenamiento, transmisión y presentación de datos. A lo largo del repositorio se exploran soluciones tanto para prototipos de laboratorio como para despliegues más cercanos a uso real, combinando sensores, pantallas, conectividad de red y servicios en la nube.

## Objetivo general

El objetivo de este conjunto de proyectos es experimentar, comparar y consolidar distintas arquitecturas de adquisición y envío de datos desde microcontroladores. En lugar de limitarse a una única placa o servicio, el repositorio muestra variantes que permiten adaptar la solución a diferentes necesidades: desde registros locales en tarjeta SD, hasta publicación en plataformas web o en aplicaciones móviles. Esto convierte al repositorio en una base útil para prototipado, documentación técnica y reutilización de código en futuros desarrollos de monitoreo ambiental.

## PLACAS

La categoría **PLACAS** agrupa proyectos organizados según el tipo de tarjeta o microcontrolador utilizado. Aquí se encuentran implementaciones pensadas para Arduino UNO, MEGA, ESP8266, ESP32, D1 mini, NodeMCU y otras variantes, así como combinaciones de placas con módulos adicionales como pantallas LCD, eInk, Ethernet Shield o circuitos para medición de batería. Esta organización permite adaptar la lógica del datalogger al hardware disponible y facilita comparar cómo cambia el comportamiento del sistema entre distintas arquitecturas. También es una categoría especialmente útil para estudiar limitaciones y ventajas de cada plataforma, como consumo energético, capacidad de memoria, conectividad integrada y compatibilidad con librerías.

Desde el punto de vista tecnológico, esta carpeta refleja una gran diversidad de soluciones: algunos sketches priorizan la conectividad WiFi, otros usan Ethernet, otros exploran modos de bajo consumo como deep sleep, y varios incorporan mecanismos de depuración o visualización local. En conjunto, esta categoría representa el núcleo de experimentación del repositorio, porque reúne la adaptación del mismo problema —capturar y mostrar datos— a múltiples placas y configuraciones.

## PLATAFORMAS

La categoría **PLATAFORMAS** contiene sketches orientados a la integración con servicios externos y entornos de visualización remota. En este grupo se incluyen proyectos que envían datos a Blynk, Google Sheets, IFTTT, servidores PHP o soluciones basadas en HTTP, además de variantes que incorporan RTC para mantener una referencia temporal confiable. Su propósito es convertir la lectura del sensor en información accesible a distancia, ya sea mediante paneles móviles, hojas de cálculo en la nube o endpoints web propios. Esto permite registrar datos de forma continua y consultarlos desde diferentes dispositivos o interfaces.

Tecnológicamente, esta categoría combina comunicación de red, temporización por software y formatos de envío simples como peticiones GET. También aparecen implementaciones con Blynk 1.0 y Blynk 2.0, uso de `millis()` para control de intervalos, y esquemas donde la placa actúa como cliente que publica datos a un servidor. En conjunto, es la sección que muestra con mayor claridad la orientación IoT del repositorio, porque conecta la adquisición de datos con ecosistemas remotos de almacenamiento y monitoreo.

## SENSORES

La categoría **SENSORES** agrupa proyectos cuyo foco principal es validar y explotar el comportamiento de los sensores ambientales. Aquí predominan pruebas con SHT31, BME280 y DHT22, normalmente acompañadas por pantallas LCD o DataShield, con el objetivo de confirmar lecturas, calibrar el sistema y entender la respuesta del hardware antes de integrarlo en un flujo completo de transmisión o almacenamiento. Esta sección es especialmente útil como laboratorio de prueba, ya que permite aislar el comportamiento del sensor del resto del sistema y depurar problemas de lectura, alimentación o conexión.

En términos tecnológicos, esta carpeta está muy ligada a la comunicación I2C, al uso de librerías específicas de Adafruit y a la lectura periódica de variables como temperatura y humedad relativa. También se observa una preocupación por presentar los datos de forma legible, ya sea en el monitor serial o en una pantalla conectada al montaje. Por eso, esta categoría funciona como base de validación: antes de construir un datalogger completo, aquí se confirma que el sensor responde correctamente y que la cadena de adquisición es estable.

## TRANSPORTE

La categoría **TRANSPORTE** reúne proyectos dedicados al canal mediante el cual los datos viajan desde el dispositivo hasta la plataforma destino. En lugar de concentrarse solo en la lectura o en la visualización, aquí el interés está en cómo se transmiten los datos, ya sea por Ethernet, WiFi, o mecanismos específicos como ESP-NOW o peticiones HTTP. Esta organización ayuda a separar claramente la lógica de medición de la lógica de comunicación, lo que es muy útil al desarrollar sistemas modulares y más fáciles de mantener.

A nivel tecnológico, esta categoría suele involucrar protocolos de red, clientes HTTP, configuraciones de IP estática, y librerías como `Ethernet.h`, `WiFi.h` o `esp_now.h`. También aparecen ejemplos donde la placa actúa como emisor y otra como receptor, lo que permite estudiar arquitecturas distribuidas o pasarelas entre microcontroladores. En resumen, esta carpeta no trata tanto del sensor en sí, sino del camino que recorren los datos para llegar a su destino final.

## DESPLIEGUE

La categoría **DESPLIEGUE** está orientada a los montajes pensados para uso en campo o para presentación directa de la información en el dispositivo. Aquí aparecen soluciones con pantallas eInk, buzzer, lectura de batería y gestión de ciclos de actualización, lo que sugiere una intención clara de construir sistemas autónomos, de bajo consumo y visualmente simples. Esta es la parte del repositorio más cercana a una aplicación final, porque no solo adquiere datos, sino que también los hace visibles y útiles para un operador o usuario final.

Desde el punto de vista tecnológico, esta carpeta destaca por el uso de displays de tinta electrónica, librerías específicas como `Adafruit_ThinkInk`, y mecanismos para operar con eficiencia energética. También aparecen mediciones de batería y señales acústicas que ayudan a confirmar el estado del sistema sin depender necesariamente de una conexión permanente a internet. Es una categoría muy representativa del paso entre prototipo y despliegue funcional.

## ifDEBUG

La carpeta **ifDEBUG** contiene sketches pequeños de apoyo para depuración y prueba de comportamiento. Su función es ayudar a controlar la salida por serial sin afectar la lógica principal del sistema, permitiendo activar o desactivar mensajes de diagnóstico mediante macros de compilación. Aunque parece una categoría menor, cumple una función importante en el mantenimiento del repositorio, porque facilita pruebas rápidas y reduce la necesidad de modificar código central solo para depurar.

En términos tecnológicos, se basa en una técnica simple pero efectiva: compilación condicional con `#define DEBUG` y macros que deciden si imprimir o no por `Serial`. Esto hace que el mismo sketch pueda servir tanto para desarrollo como para ejecución más limpia, sin necesidad de mantener versiones duplicadas. Es una utilidad práctica para revisar estados, errores y comportamiento básico del hardware.

## Características generales del repositorio

En conjunto, el repositorio se caracteriza por su enfoque modular, su variedad de hardware soportado y su orientación a proyectos de monitoreo ambiental e IoT. Predominan los sketches escritos en Arduino C/C++, con uso frecuente de sensores de temperatura y humedad, comunicaciones WiFi y Ethernet, pantallas de bajo consumo, registro local en SD y envío de datos a plataformas remotas. También se aprecia un interés claro por la experimentación técnica, ya que muchas carpetas parecen representar iteraciones, pruebas o variantes de una misma idea principal.

## Conclusión

Este repositorio puede entenderse como una base práctica de trabajo para construir dataloggers ambientales en distintas condiciones de hardware y conectividad. Su valor principal está en mostrar, de forma organizada, cómo resolver el mismo problema con enfoques diferentes: lectura local, almacenamiento, transmisión, visualización y monitoreo remoto. Por eso resulta útil tanto como referencia técnica como repositorio de soluciones reutilizables.
