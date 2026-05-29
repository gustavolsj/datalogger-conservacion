<?php
// Configuración de la base de datos
$servername = "DB_HOST_PLACEHOLDER";
$dbname = "DB_NAME_PLACEHOLDER";
$username = "DB_USER_PLACEHOLDER";
$password = "DB_PASSWORD_PLACEHOLDER";
$tablename = "DB_TABLE_PLACEHOLDER";

$espacio = ", ";

// Crear conexión
$conn = new mysqli($servername, $username, $password, $dbname);

// Revisar conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Obtener datos de la URL
$temperatura = $_GET['temperatura'];
$humedad = $_GET['humedad'];

// Insertar datos en la base de datos
$sql = "INSERT INTO $tablename (temperatura, humedad) VALUES ('$temperatura', '$humedad')";
//$sql = "INSERT INTO datos_ambientales (temperatura, humedad) VALUES ('$temperatura', '$humedad')";

if ($conn->query($sql) === TRUE) {
    echo "Datos insertados correctamente en mysql";
	echo "<br>";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

  // test----------------------        Crea un archivo de texto con el texto enviado
// Consulta para obtener los últimos registros
//$sql = "SELECT fecha_hora, temperatura, humedad FROM $tablename ORDER BY fecha_hora DESC LIMIT 1";
$sql = "SELECT fecha_hora, temperatura, humedad FROM $tablename ORDER BY fecha_hora DESC LIMIT 1";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // Salida de datos de cada fila
    while($row = $result->fetch_assoc()) {
        // echo "<br>" . $row["fecha_hora"]. ", " . $row["temperatura"]. ", " . $row["humedad"]. "<br>";
		$fecha_hora = $row["fecha_hora"];
        $temperatura = $row["temperatura"];
        $humedad = $row["humedad"];
	
		$nueva_fecha = substr($fecha_hora, 2);
		$nueva_fecha2 = substr($nueva_fecha, 0, strlen($nueva_fecha) - 3);
		echo $nueva_fecha2;	
		echo "<br>";
		echo $temperatura;
		echo "<br>";
		echo $humedad;		
		echo "<br>";
		
/*         $archivo = fopen("datos.csv", "a");
        fwrite($archivo, $nueva_fecha2 . ", " . $temperatura . ", " . $humedad);
        fwrite($archivo, "\n");
        fclose($archivo);
		echo "Datos insertados correctamente en datos.csv<br>"; */

        // Formatear fecha y hora como dd/mm/aa/ hh:mm
        $dt = new DateTime($fecha_hora);
        $fecha_formateada = $dt->format("d/m/y/ H:i");

        // Crear array asociativo con los datos
        $datos_json = array(
            "fecha_hora" => $fecha_formateada,
            "temperatura" => $temperatura,
            "humedad" => $humedad
        );

        // Leer contenido actual del archivo JSON (si existe)
        $archivo_json = "datos.json";
        $contenido_actual = [];

        if (file_exists($archivo_json)) {
            $contenido_actual = json_decode(file_get_contents($archivo_json), true);
            if (!is_array($contenido_actual)) {
                $contenido_actual = [];
            }
        }

        // Agregar nuevo registro al array
        $contenido_actual[] = $datos_json;

        // Guardar el array actualizado en el archivo JSON
 
        if (file_put_contents($archivo_json, json_encode($contenido_actual, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE)) === false) {
            echo "❌ Error al escribir en datos.json<br>";
        } else {
            echo "✅ Datos guardados en datos.json<br>";
        }
    }
} else {
    echo "0 resultados";
}



$conn->close();
?>
