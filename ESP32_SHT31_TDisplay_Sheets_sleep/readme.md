Aquí tienes la **guía completa y corregida** para iniciar con la TTGO T‑Display
(ESP32) en Arduino IDE, incluyendo el **Paso 3 mejorado** para identificar qué
chip USB–Serial usa tu placa: --- # 🚀 Guía paso a paso para usar ESP32 TTGO
T‑Display V1 con Arduino IDE --- ## 🖥️ Paso 1: Identificar tu sistema operativo
Este paso es **para determinar qué versión de Arduino IDE necesitas instalar**.
- **Windows**: - Presiona `Win + R`, escribe `winver` y confirma la versión. -
Si es **Windows 10/11**, instala **Arduino IDE moderno (2.x)**. - Si es
**Windows 7/8**, instala **Arduino IDE Legacy (1.8.x)**. - **macOS**: - Haz clic
en  → *Acerca de este Mac* → revisa la versión. - Si es **Catalina o más
nuevo**, instala **Arduino IDE moderno (2.x)**. - Si es **Mojave o anterior**,
instala **Arduino IDE Legacy (1.8.x)**. --- ## 📥 Paso 2: Descargar e instalar
Arduino IDE - Página oficial: [Arduino IDE
Download](https://www.arduino.cc/software) - Descarga la versión adecuada según
tu sistema operativo (moderno o legacy). - **Windows**: abre el `.exe` y sigue
el asistente. - **macOS**: abre el `.dmg` y arrastra Arduino IDE a
*Aplicaciones*. --- ## 🔌 Paso 3: Instalar drivers para TTGO T‑Display V1 La
TTGO T‑Display necesita un **driver USB–Serial** para que la computadora la
reconozca. Existen dos chips comunes: **CH340** y **CP2102**. Para saber cuál
usa tu placa: 1. **Revisa físicamente la placa** - Mira cerca del conector
USB‑C. - Si el chip tiene impreso **CH340** o **CH341**, necesitas el driver
CH340. - Si el chip dice **CP2102** o **CP210x**, necesitas el driver CP210x. 2.
**Conecta la placa y verifica en el sistema operativo** - **Windows**: abre
*Administrador de dispositivos* → *Puertos (COM & LPT)*. - Si aparece
“USB‑SERIAL CH340”, es CH340. - Si aparece “Silicon Labs CP210x USB to UART
Bridge”, es CP210x. - **macOS (forma gráfica infalible)**: - Menú  → *Acerca de
este Mac* → *Más información / Informe del sistema* → *Hardware → USB*. - Ahí
debe listarse el dispositivo como *CP210x* o *CH340*. - En Arduino IDE, debe
aparecer bajo *Herramientas → Puerto* como `/dev/cu.usbserial-...` o
`/dev/cu.wchusbserial-...`. 3. **Descarga el driver correcto** - **CH340
(Windows/macOS/Linux)**: CH340/CH341 Drivers – WCH [(wch.cn in
Bing)](https://www.bing.com/search?q="http%3A%2F%2Fwww.wch.cn%2Fdownloads%2FCH341SER_EXE.html")
- **CP210x (Windows/macOS/Linux)**: [CP210x VCP Drivers – Silicon
Labs](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers) 4.
**Instala y reinicia tu computadora** para que el sistema reconozca la placa.
--- ## 🔗 Paso 4: Conectar la placa 1. Usa un **cable USB‑C de datos**. 2.
Conecta la TTGO T‑Display. 3. Verifica que el sistema la reconoce en el puerto
correcto (COM en Windows, USB en macOS). --- ## ⚙️ Paso 5: Instalar gestor de
tarjetas ESP32 1. Abre Arduino IDE → *Archivo → Preferencias*. 2. En “Additional
Boards Manager URLs” pega la URL oficial actual: ```
https://espressif.github.io/arduino-esp32/package_esp32_index.json ``` 3. Haz
clic en **OK**. 4. Ve a *Herramientas → Placa → Gestor de tarjetas*. 5. Busca
**ESP32 by Espressif Systems** → clic en **Instalar**. --- ## 📚 Paso 6:
Instalar librerías necesarias En *Programa → Incluir librería → Administrar
librerías* busca e instala: - **Adafruit SHT31** - **LovyanGFX**
(`WiFiClientSecure` ya viene incluido en el core ESP32). --- ## 💡 Paso 7: Subir
un sketch de prueba En la TTGO T‑Display, el LED visible suele ser solo el
indicador de encendido/carga. Para una prueba visual real conviene usar la
pantalla ST7789. 1. Abre Arduino IDE. 2. Ve a *Archivo → Ejemplos → 01.Basics →
Blink* (sirve para verificar compilación y carga, aunque no verás el LED
parpadear). 3. Para una prueba visual en la pantalla, usa un sketch mínimo con
**LovyanGFX** que dibuje texto o figuras en el ST7789. --- ## ⚠️ Detalles
importantes - Usa siempre un **cable USB‑C de datos**. - Si el IDE moderno (2.x)
es muy lento en tu PC, instala el **IDE Legacy (1.8.x)**. - En macOS recientes,
puede que tengas que dar permisos de seguridad para el driver. - Si la placa no
aparece en puertos, reinstala el driver y reinicia. - Para pruebas visuales en
TTGO T‑Display, usa la pantalla ST7789 en lugar del LED. --- 👉 Con esta versión
completa y corregida, tu guía queda lista para principiantes y talleres.
¿Quieres que ahora te prepare un **sketch gráfico de demostración con
LovyanGFX** (texto y colores en pantalla) para que sea tu ejemplo inicial
práctico?
