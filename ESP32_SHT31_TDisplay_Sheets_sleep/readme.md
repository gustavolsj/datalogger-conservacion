# 🚀 Guía paso a paso para usar ESP32 TTGO T‑Display V1 con Arduino IDE

---

## 🖥️ Paso 1: Identificar tu sistema operativo

Este paso es **para determinar qué versión de Arduino IDE necesitas instalar**.

- **Windows**:
  - Presiona `Win + R`, escribe `winver` y confirma la versión.
  - Si es **Windows 10/11**, instala **Arduino IDE moderno (2.x)**.
  - Si es **Windows 7/8**, instala **Arduino IDE Legacy (1.8.x)**.

- **macOS**:
  - Haz clic en Command → _Acerca de este Mac_ → revisa la versión.
  - Si es **Catalina o más nuevo**, instala **Arduino IDE moderno (2.x)**.
  - Si es **Mojave o anterior**, instala **Arduino IDE Legacy (1.8.x)**.

---

## 📥 Paso 2: Descargar e instalar Arduino IDE

- Página oficial: [Arduino IDE Download](https://www.arduino.cc/software)
- Descarga la versión adecuada según tu sistema operativo (moderno o legacy).
- **Windows**: abre el `.exe` y sigue el asistente.
- **macOS**: abre el `.dmg` y arrastra Arduino IDE a _Aplicaciones_.

---

## 🔌 Paso 3: Instalar drivers para TTGO T‑Display V1

La TTGO T‑Display necesita un **driver USB–Serial** para que la computadora la reconozca. Existen dos chips comunes: **CH340** y **CP2102**. Para saber cuál usa tu placa:

1. **Revisa físicamente la placa**
   - Mira cerca del conector USB‑C.
   - Si el chip tiene impreso **CH340** o **CH341**, necesitas el driver CH340.
   - Si el chip dice **CP2102** o **CP210x**, necesitas el driver CP210x.

2. **Conecta la placa y verifica en el sistema operativo**
   - **Windows**: abre _Administrador de dispositivos_ → _Puertos (COM & LPT)_.
     - Si aparece “USB‑SERIAL CH340”, es CH340.
     - Si aparece “Silicon Labs CP210x USB to UART Bridge”, es CP210x.
   - **macOS**:
     - Menú Command → _Acerca de este Mac_ → _Más información / Informe del sistema_ → _Hardware → USB_.
     - Ahí debe listarse el dispositivo como _CP210x_ o _CH340_.
     - En Arduino IDE, debe aparecer bajo _Herramientas → Puerto_ como `/dev/cu.usbserial-...` o `/dev/cu.wchusbserial-...`.

3. **Descarga el driver correcto**
   - **CH340 (Windows/macOS/Linux)**: CH340/CH341 Drivers – WCH [(wch.cn in Bing)](https://www.bing.com/search?q="http%3A%2F%2Fwww.wch.cn%2Fdownloads%2FCH341SER_EXE.html")
   - **CP210x (Windows/macOS/Linux)**: [CP210x VCP Drivers – Silicon Labs](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers)

4. **Instala y reinicia tu computadora** para que el sistema reconozca la placa.

---

## 🔗 Paso 4: Conectar la placa

1. Usa un **cable USB‑C de datos**.
2. Conecta la TTGO T‑Display.
3. Verifica que el sistema la reconoce en el puerto correcto (COM en Windows, USB en macOS).

---

## ⚙️ Paso 5: Instalar gestor de tarjetas ESP32

1. Abre Arduino IDE → _Archivo → Preferencias_.
2. En “Additional Boards Manager URLs” pega la URL oficial actual:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
3. Haz clic en **OK**.
4. Ve a _Herramientas → Placa → Gestor de tarjetas_.
5. Busca **ESP32 by Espressif Systems** → clic en **Instalar**.

---

## 📚 Paso 6: Instalar librerías necesarias

En _Programa → Incluir librería → Administrar librerías_ busca e instala:

- **Adafruit SHT31**
- **LovyanGFX**  
  (`WiFiClientSecure` ya viene incluido en el core ESP32).

---

## 💡 Paso 7: Subir un sketch de prueba con LovyanGFX

En la TTGO T‑Display, el LED visible suele ser solo el indicador de encendido/carga. Para una prueba visual real conviene usar la pantalla ST7789.

1. Abre Arduino IDE.

2. Ve a Archivo → Ejemplos → LovyanGFX → HowToUse 1_simple_use.

3. Selecciona la placa: ESP32 TTGO T‑Display.

4. Selecciona el puerto correcto.

5. Haz clic en Subir.

La pantalla ST7789 mostrará texto y gráficos básicos, confirmando que la librería funciona y que la placa está configurada correctamente. 🎉

## ⚠️ Detalles importantes

- Usa siempre un **cable USB‑C de datos**.
- Si el IDE moderno (2.x) es muy lento en tu PC, instala el **IDE Legacy (1.8.x)**.
- En macOS recientes, puede que tengas que dar permisos de seguridad para el driver.
- Si la placa no aparece en puertos, reinstala el driver y reinicia.
