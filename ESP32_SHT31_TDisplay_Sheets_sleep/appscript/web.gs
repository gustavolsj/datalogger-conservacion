const SHEET_NAME = "Hoja 1"; // Cambia por el nombre de tu hoja

function doGet() {
  return HtmlService.createHtmlOutputFromFile("index4").setTitle(
    "Últimos datos",
  );
}

function getUltimosDatos() {
  const ss = SpreadsheetApp.getActiveSpreadsheet(); // Si el script es independiente: SpreadsheetApp.openById('TU_ID')
  const sheet = ss.getSheetByName(SHEET_NAME);
  const lastRow = sheet.getLastRow();
  if (lastRow < 1) return [];

  const headers = sheet.getRange(1, 1, 1, 4).getValues()[0];
  const values = sheet.getRange(1, 1, lastRow, 4).getValues();

  const resultado = [];
  for (let col = 0; col < 4; col++) {
    let ultimo = "";
    let fila = null;
    for (let r = values.length - 1; r >= 1; r--) {
      if (values[r][col] !== "" && values[r][col] !== null) {
        ultimo = values[r][col];
        fila = r + 1;
        break;
      }
    }
    resultado.push({
      columna: String.fromCharCode(65 + col),
      encabezado: headers[col],
      valor: ultimo instanceof Date ? ultimo.toLocaleString() : ultimo,
      fila: fila,
    });
  }
  return resultado;
}
