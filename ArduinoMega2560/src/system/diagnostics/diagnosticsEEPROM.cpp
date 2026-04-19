#include "system/diagnostics/diagnosticsEEPROM.h"

/**
 * @brief Ejecuta un test de diagnóstico sobre la EEPROM en la dirección indicada.
 *
 * El procedimiento realiza:
 * - Escritura de un valor de prueba en la EEPROM.
 * - Lectura del valor almacenado.
 * - Comparación entre valor escrito y leído.
 * - Informe del estado de memoria libre y usada.
 * - Limpieza de la dirección probada.
 *
 * @param address Dirección de EEPROM a probar (por defecto 0).
 * @return true si la EEPROM responde correctamente, false en caso contrario.
 */
bool DiagnosticsEEPROM::runTest(int address) {
    standardMessage("🧪 Starting EEPROM diagnostic", __FILE__, __FUNCTION__, __DATE__, __TIME__);

    // Write test value
    EEPROM.write(address, testValue);
    delay(10);

    // Read stored value
    byte readValue = EEPROM.read(address);
    Serial.print("📥 Value read from EEPROM[");
    Serial.print(address);
    Serial.print("]: ");
    Serial.println(readValue);

    // Show memory status
    int freeMem = getFreeMemory();
    Serial.print("📊 Estimated free memory: ");
    Serial.print(freeMem);
    Serial.println(" bytes");

    Serial.print("📦 Estimated used memory: ");
    Serial.print(RAMEND - freeMem);
    Serial.println(" bytes");

    // Check if value matches
    if (readValue == testValue) {
        Serial.println("✅ EEPROM is responding correctly.");
        clearEEPROM(address);
        standardHeaderFull("EEPROM diagnostic completed", __FILE__, __FUNCTION__, __DATE__, __TIME__);
        return true;
    } else {
        Serial.println("❌ EEPROM error: value mismatch.");
        clearEEPROM(address);
        standardHeaderFull("EEPROM diagnostic failed", __FILE__, __FUNCTION__, __DATE__, __TIME__);
        return false;
    }
}


/**
 * @brief Limpia la EEPROM en la dirección indicada escribiendo 0xFF.
 *
 * Tras limpiar, lee nuevamente el valor para verificar la operación.
 *
 * @param address Dirección de EEPROM a limpiar.
 */
void DiagnosticsEEPROM::clearEEPROM(int address) {
    EEPROM.write(address, 0xFF);
    delay(10);

    byte clearedValue = EEPROM.read(address);
    Serial.print("🧹 EEPROM cleared. New value at [");
    Serial.print(address);
    Serial.print("]: ");
    Serial.println(clearedValue);
}


/**
 * @brief Estima la cantidad de memoria RAM libre disponible en el sistema.
 *
 * Este método funciona en placas basadas en AVR como Arduino Mega 2560.
 * Calcula la memoria libre comparando la posición actual de la pila con el heap.
 *
 * @return Número de bytes libres en RAM.
 */
int DiagnosticsEEPROM::getFreeMemory() {
    extern unsigned int __heap_start;
    extern void *__brkval;
    int freeMemory;

    if ((int)__brkval == 0) {
        freeMemory = ((int)&freeMemory) - ((int)&__heap_start);
    } else {
        freeMemory = ((int)&freeMemory) - ((int)__brkval);
    }

    return freeMemory;
}
