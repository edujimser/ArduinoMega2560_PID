#include "system/diagnostics/diagnosticsUART.h" // Header for UART diagnostics

/**
 * @brief Ejecuta diagnósticos en todos los puertos UART disponibles.
 *
 * Esta función:
 * - Muestra un mensaje de inicio del diagnóstico.
 * - Ejecuta diagnoseSerial1() y diagnoseSerial2().
 * - Muestra un mensaje de finalización del diagnóstico.
 *
 * Actualmente incluye:
 * - UART1 (Serial1)
 * - UART2 (Serial2)
 */
void diagnoseAllUART() {
    standardMessage("Starting UART communication diagnostic", __FILE__, __FUNCTION__, __DATE__, __TIME__);

    diagnoseSerial1();
    diagnoseSerial2();

    standardHeaderFull("Completed UART diagnostic", __FILE__, __FUNCTION__, __DATE__, __TIME__);
}

/**
 * @brief Realiza un diagnóstico sobre un puerto UART específico.
 *
 * El procedimiento:
 * - Inicia el puerto UART a 9600 baudios.
 * - Espera durante 3 segundos la llegada de datos.
 * - Si recibe datos, los imprime por consola.
 * - Si no recibe nada, informa que no hubo actividad.
 *
 * @param serialPort Referencia al objeto HardwareSerial (por ejemplo, Serial1 o Serial2).
 * @param portName Nombre del puerto UART para mostrar en consola.
 */
void diagnoseUARTStream(HardwareSerial& serialPort, const char* portName) {
    Serial.println("📡 Diagnosing " + String(portName));

    serialPort.begin(9600);
    delay(100);

    Serial.println("⌛ Waiting for data on " + String(portName) + " for 3 seconds...");

    unsigned long start = millis();
    bool received = false;

    while (millis() - start < 3000) {
        if (serialPort.available()) {
            received = true;
            Serial.print("📥 Data received on ");
            Serial.print(portName);
            Serial.print(": ");
            while (serialPort.available()) {
                char c = serialPort.read();
                Serial.print(c);
            }
            Serial.println();
            break;
        }
    }

    if (!received) {
        Serial.println("🔻 No data detected on " + String(portName));
    }

    // Do not call serialPort.end() to keep the port active
    Serial.println("✅ End of diagnostic.\n");
}

/**
 * @brief Ejecuta diagnóstico sobre Serial1 (UART1), si está disponible.
 *
 * Esta función solo se compila si HAVE_HWSERIAL1 está definido.
 */
void diagnoseSerial1() {
#if defined(HAVE_HWSERIAL1)
    diagnoseUARTStream(Serial1, "Serial1");
#endif
}

/**
 * @brief Ejecuta diagnóstico sobre Serial2 (UART2), si está disponible.
 *
 * Esta función solo se compila si HAVE_HWSERIAL2 está definido.
 */
void diagnoseSerial2() {
#if defined(HAVE_HWSERIAL2)
    diagnoseUARTStream(Serial2, "Serial2");
#endif
}
