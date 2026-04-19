#include "system/msg/msg.h"

/**
 * @brief Imprime un mensaje estandarizado y formateado en el monitor serie.
 *
 * El mensaje aparece centrado entre dos líneas decorativas y acompañado
 * de información contextual como archivo, función, fecha y hora.
 *
 * @param message   Mensaje principal a mostrar.
 * @param file      Nombre del archivo origen.
 * @param function  Nombre de la función desde donde se llama.
 * @param date      Fecha de compilación o personalizada.
 * @param time      Hora de compilación o personalizada.
 */
void standardMessage(const char* message, 
                     const char* file, 
                     const char* function, 
                     const char* date, 
                     const char* time) {
    const uint8_t totalWidth = 200; // Total width of the decorative line
    int msgLen = strlen(message);   // Length of the message text
    int padding = (totalWidth - msgLen) / 2; // Spaces to add on the left to center the text

    Serial.println();

    // ─────────────────────────────
    // Top decorative line
    // ─────────────────────────────
    for (uint8_t i = 0; i < totalWidth; i++) Serial.write('-');
    Serial.println();

    // ─────────────────────────────
    // Centered message text
    // ─────────────────────────────
    for (int i = 0; i < padding; i++) Serial.write(' '); // Left padding
    Serial.println(message); // Main message

    // ─────────────────────────────
    // Bottom decorative line
    // ─────────────────────────────
    for (uint8_t i = 0; i < totalWidth; i++) Serial.write('-');
    Serial.println();

    // ─────────────────────────────
    // Detailed information below the main message
    // ─────────────────────────────
    Serial.print("[");
    Serial.print(date);
    Serial.print(" ");
    Serial.print(time);
    Serial.print("] ");
    Serial.print(file);
    Serial.print("::");
    Serial.print(function);
    Serial.print(" ➤ ");
    Serial.println(message);

    Serial.println();
}


/**
 * @brief Imprime un encabezado decorativo con el mensaje centrado.
 *
 * Genera una línea con caracteres decorativos a ambos lados del texto,
 * manteniendo un ancho total definido por el usuario.
 *
 * @param message Texto a centrar.
 * @param width   Ancho total de la línea.
 * @param deco    Carácter decorativo.
 */
void standardHeader(const char* message,
                    uint16_t width,
                    char deco) {
    int msgLen = strlen(message);
    int totalDeco = width - msgLen - 2; // Subtract 2 for spacing around the message
    if (totalDeco < 0) totalDeco = 0;

    int leftDeco = totalDeco / 2;
    int rightDeco = totalDeco - leftDeco;

    // 🔹 Blank line before the header
    Serial.println();

    // 🔹 Left decorative padding
    for (int i = 0; i < leftDeco; i++) Serial.write(deco);

    // 🔹 Centered message with spacing
    Serial.print(' ');
    Serial.print(message);
    Serial.print(' ');

    // 🔹 Right decorative padding
    for (int i = 0; i < rightDeco; i++) Serial.write(deco);

    // 🔹 Blank line after the header
    Serial.println();
    Serial.println();
}


/**
 * @brief Imprime un encabezado decorado con texto centrado y metadatos.
 *
 * Construye una línea única que incluye:
 * - mensaje principal
 * - archivo
 * - función
 * - fecha y hora
 *
 * Todo ello centrado y rodeado de caracteres decorativos.
 *
 * @param message   Mensaje principal.
 * @param file      Archivo origen.
 * @param function  Función origen.
 * @param date      Fecha de compilación.
 * @param time      Hora de compilación.
 * @param width     Ancho total de la línea.
 * @param deco      Carácter decorativo.
 */
void standardHeaderFull(const char* message,
                        const char* file,
                        const char* function,
                        const char* date,
                        const char* time,
                        uint16_t width,
                        char deco) {
    char buffer[256];
    // Build the full message with metadata
    snprintf(buffer, sizeof(buffer), "%s | %s | %s | %s %s",
             message, file, function, date, time);

    int msgLen = strlen(buffer);
    int totalDeco = width - msgLen - 2; // Subtract 2 for spacing around the message
    if (totalDeco < 0) totalDeco = 0;   // Prevent negative padding

    int leftDeco = totalDeco / 2;
    int rightDeco = totalDeco - leftDeco;

    // 🔹 Blank line before the header
    Serial.println();

    // 🔹 Left decorative padding
    for (int i = 0; i < leftDeco; i++) Serial.write(deco);

    // 🔹 Centered message with spacing
    Serial.print(' ');
    Serial.print(buffer);
    Serial.print(' ');

    // 🔹 Right decorative padding
    for (int i = 0; i < rightDeco; i++) Serial.write(deco);

    // 🔹 Blank lines after the header
    Serial.println();
    Serial.println();
}


/**
 * @brief Imprime un mensaje de error enriquecido con información contextual.
 *
 * Incluye:
 * - fecha y hora
 * - archivo
 * - función
 * - número de línea
 * - mensaje de error
 *
 * @param message   Mensaje de error.
 * @param file      Archivo origen.
 * @param function  Función origen.
 * @param date      Fecha de compilación.
 * @param time      Hora de compilación.
 * @param line      Número de línea donde se invoca.
 */
void standardErrorMessage(const char* message, const char* file, const char* function, const char* date, const char* time, int line) {
  Serial.println();
  Serial.print("[");
  Serial.print(date);
  Serial.print(" ");
  Serial.print(time);
  Serial.print("] ");
  Serial.print(file);
  Serial.print("::");
  Serial.print(function);
  Serial.print(" (Line ");
  Serial.print(line);
  Serial.print(") ❌ ERROR ➤ ");
  Serial.println(message);
  Serial.println();
}


/**
 * @brief Muestra la configuración actual del sistema.
 *
 * Presenta los valores de la estructura configuracionMain
 * con un formato visual claro y amigable.
 *
 * @param configuration Estructura de configuración del sistema.
 */
void showConfigurationMessage(const configuracionMain& configuration) {
  standardMessage("Current System Configuration",
                     __FILE__,
                     __FUNCTION__,
                     __DATE__,
                     __TIME__);
  Serial.print("🔧 Debug mode: ");
  Serial.println(configuration.debugMode ? "Enabled" : "Disabled");

  standardHeaderFull("End of Configuration",
                     __FILE__,
                     __FUNCTION__,
                     __DATE__,
                     __TIME__,
                     120,
                     '-');
}


/**
 * @brief Imprime una línea compuesta por un carácter repetido.
 *
 * @param c       Carácter a imprimir.
 * @param length  Número de repeticiones.
 */
void showMsgPrinLine(char c, int length) {
   for (int i = 0; i < length; i++) {
        Serial.print(c);
    }
    Serial.println();
}


/**
 * @brief Imprime la información de versión del firmware.
 *
 * Muestra:
 * - versión del firmware
 * - nombre
 * - fecha
 * - autor
 * - versión de la aplicación
 * - nombre de la aplicación
 * - fecha de la aplicación
 */
void printVersion(const char* fwVersion,
                  const char* fwName,
                  const char* fwDate,
                  const char* fwAuthor,
                  const char* appVersion,
                  const char* appName,
                  const char* appDate) {
    standardMessage("Firmware Version Information",
                     __FILE__,
                     __FUNCTION__,
                     __DATE__,
                     __TIME__);
    Serial.print("Firmware Version: "); Serial.println(fwVersion);
    Serial.print("Firmware Name: ");    Serial.println(fwName);
    Serial.print("Firmware Date: ");    Serial.println(fwDate);
    Serial.print("Firmware Author: ");  Serial.println(fwAuthor);
    Serial.print("App Version: ");      Serial.println(appVersion);
    Serial.print("App Name: ");         Serial.println(appName);
    Serial.print("App Date: ");         Serial.println(appDate);
}
