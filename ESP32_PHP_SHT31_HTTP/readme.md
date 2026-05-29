🌡️📡 Datalogger wifi + servidor web

Estos dos dispositivos están diseñado para medir temperatura y humedad en intervalos regulares, los datos se envían a un servidor web para su registro y visualización en línea. Más que un simple datalogger es un sistema completo de sensado, procesamiento, conectividad y almacenamiento remoto.

🧩 Datalogger
El núcleo del datalogger es un microcontrolador ESP32 a él se conecta un sensor SHT31, responsable de capturar las variables ambientales con buena precisión. El ESP32 toma estas mediciones, se conecta a la red Wi-Fi disponible y transmite la información al servidor web local donde se guarda en una base de datos.

Unidad de control: ESP32 con conectividad Wi-Fi integrada.
Sensor ambiental: SHT31 (temperatura + humedad) por bus I2C.
Alimentación: cargador micro USB
Red de comunicación: infraestructura Wi-Fi local.
Backend de almacenamiento: servidor web con script PHP para la recepción de datos y su almacenamiento en una base de datos Sqlite.

🌐 Servidor web de almacenamiento
Este servidor web se aloja de manera local en una Raspberry Pi 2B, que funciona como punto central de recepción y almacenamiento. En esta Raspberry se ejecuta un script PHP que recibe por HTTP los valores de temperatura y humedad enviados por el ESP32 en cada ciclo de medicion.

El flujo de trabajo es el siguiente:

1. El ESP32 toma la lectura del sensor SHT31 y genera una peticion HTTP con los parametros de temperatura y humedad.
2. El script PHP en la Raspberry Pi 2B valida la llegada de los datos y los inserta en la base de datos local.
3. Con cada insercion, el sistema también actualiza un archivo de datos.json con el registro mas reciente para.
4. Ese JSON se utiliza como fuente para la visualizacion historica en la web.

🌐 Servidor web de visualización
La visualizacion y publicacion en línea se despliega mediante GitHub Pages usando el archivo dataloggers.md. Esa pagina consume datos.json y los presenta en linea con:

- grafica temporal de temperatura y humedad;
- tabla de resumen con minimos, maximos, promedio, mediana y desviacion estandar;
- porcentaje de tiempo dentro y fuera de los rangos definidos;
- estado del sistema (en linea/fuera de linea) segun la fecha de la ultima medicion;
- calculo de TWPI para evaluar condiciones de conservacion en el tiempo.

Con esta arquitectura, el sistema separa claramente adquisicion, almacenamiento y visualizacion: el ESP32 mide y envia, la Raspberry Pi 2B recibe y procesa, y GitHub Pages publica la interfaz para consulta remota desde cualquier dispositivo con navegador.

✅ Ventajas prácticas del datalogger
Monitoreo continuo de condiciones ambientales.
Trazabilidad histórica para reportes y decisiones.
Bajo costo y fácil despliegue en proyectos personales.
Escalable: puede replicarse en varios nodos y centralizar datos.
