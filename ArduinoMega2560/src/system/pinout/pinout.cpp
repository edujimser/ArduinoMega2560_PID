// Include the necessary headers
#include <Arduino.h>
#include "system/pinout/pinout.h"


/**
 * @brief Ejecuta un diagnóstico completo de los pines digitales GPIO, PWM y analógicos.
 *
 * Esta función:
 * - Muestra un mensaje de inicio del diagnóstico.
 * - Llama a diagnoseAnalog(), diagnoseGPIO() y diagnosePWM().
 * - Muestra un mensaje de finalización.
 *
 * Útil como prueba inicial del hardware al arrancar el sistema.
 */
void fullDiagnosticsPins() {
    standardMessage("Starting PINOUT diagnostic", __FILE__, __FUNCTION__, __DATE__, __TIME__);

    diagnoseAnalog(); // ANALOG pin diagnostic
    diagnoseGPIO();   // GPIO pin diagnostic
    diagnosePWM();    // PWM pin diagnosticº

    standardHeaderFull("Full diagnostic complete.", __FILE__, __FUNCTION__, __DATE__, __TIME__);
};


/**
 * @brief Diagnostica el voltaje presente en los pines analógicos del sistema.
 *
 * Recorre todos los pines definidos en `Pins::ANALOG`, los configura como entrada,
 * estabiliza la señal y realiza una lectura analógica. Convierte el valor leído
 * a voltaje (asumiendo referencia de 5V) y lo muestra por Serial.
 *
 * @note Incluye pequeñas demoras para estabilizar la señal.
 * @warning Asegúrate de que el sistema use referencia de 5V para lecturas precisas.
 */
void diagnoseAnalog() {
    standardHeaderFull("⚡ Detecting external voltage on ANALOG pins:", __FILE__, __FUNCTION__, __DATE__, __TIME__);

    for (size_t i = 0; i < Pins::NUM_ANALOG; ++i) {
        const PinInfo& pin = Pins::ANALOG[i];

        //Forze the pin to OUTPUT for redux noise and stabily electric system
        pinMode(pin.number, OUTPUT); 
        digitalWrite(pin.number, LOW); 
        delay(20);                   

        //Read the pin as INPUT to detect voltage
        pinMode(pin.number, INPUT); 
        delay(20);                   

        int value = analogRead(pin.number); // Read analog value
        delay(20);
        float voltage = value * (5.0 / 1023.0); // Convert to voltage (assuming 5V reference)

        //Forze the pin to OUTPUT for redux noise and stabily electric system
        pinMode(pin.number, OUTPUT); 
        digitalWrite(pin.number, LOW);
        delay(20);

        //State for default pin for init system
        pinMode(pin.number, INPUT);
        delay(20);

        Serial.print("• ");
        Serial.print(pin.name);
        Serial.print(" [#");
        Serial.print(pin.number);
        Serial.print("] → Voltage: ");
        Serial.print(voltage,2);
        Serial.println(" V");
        
    };
};


/**
 * @brief Diagnóstico de pines GPIO configurados como entrada.
 *
 * Configura cada pin GPIO como INPUT, estabiliza la señal y lee su estado.
 * Informa si el pin tiene voltaje externo (HIGH) o está en LOW/GND.
 *
 * @note Útil para detectar si un pin está conectado a tierra o flotando.
 */
void diagnoseGPIO(){
    standardHeaderFull("⚡ Detecting external voltage on GPIO pins:", __FILE__, __FUNCTION__, __DATE__, __TIME__);

    for (size_t i = 0; i < Pins::NUM_GPIO; ++i) {
        const PinInfo& pin = Pins::GPIO[i];

        //Forze the pin to OUTPUT for redux noise and stabily electric system
        pinMode(pin.number, OUTPUT); 
        digitalWrite(pin.number, LOW); 
        delay(20); 

        pinMode(pin.number, INPUT);         // Set pin as input without pull-up
        delay(5);                           // Allow signal to stabilize

        int state = digitalRead(pin.number); // Read pin state

        Serial.print("• ");
        Serial.print(pin.name);
        Serial.print(" [#");
        Serial.print(pin.number);
        Serial.print("] → Voltage: ");

        if (state == HIGH) {
            Serial.println("⚡ External voltage detected (HIGH)");
        } else {
            Serial.println("🔻 No voltage (LOW or connected to GND)");
        }
    }

}


/**
 * @brief Detecta la presencia de voltaje externo en pines PWM.
 *
 * Configura cada pin PWM como entrada, estabiliza la señal y lee su estado digital.
 * Informa si hay voltaje (HIGH) o no (LOW/GND).
 *
 * @note No mide voltaje real, solo estado lógico.
 * @note Para medir voltaje real, usar pines analógicos.
 */
void diagnosePWM() {
    standardHeaderFull("⚡ Detecting voltage on PWM pins:", __FILE__, __FUNCTION__, __DATE__, __TIME__);

    for (size_t i = 0; i < Pins::NUM_PWM; ++i) {
        const PinInfo& pin = Pins::PWM[i];

        //Forze the pin to OUTPUT for redux noise and stabily electric system
        pinMode(pin.number, OUTPUT); 
        analogWrite(pin.number, 0); 
        delay(20); 

        pinMode(pin.number, INPUT);         // Set pin as input (no pull-up)
        delay(5);                           // Brief delay to stabilize reading

        int state = digitalRead(pin.number); // Read the electrical state of the pin

        Serial.print("• ");
        Serial.print(pin.name);
        Serial.print(" [Pin ");
        Serial.print(pin.number);
        Serial.print("] → Voltage: ");

        if (state == HIGH) {
            Serial.println("⚡ Voltage detected (HIGH)");
        } else {
            Serial.println("🔻 No voltage (LOW or connected to GND)");
        };
    };
};


/**
 * @brief Devuelve el número físico del pin.
 *
 * @param pin Estructura PinInfo.
 * @return Número del pin.
 */
uint8_t pinNumber(const PinInfo pin) {
    return pin.number;
}


/**
 * @brief Devuelve el nombre del pin.
 *
 * @param pin Estructura PinInfo.
 * @return Nombre del pin.
 */
const char* pinName(const PinInfo pin) {
    return pin.name;
}


/**
 * @brief Devuelve la familia del pin (GPIO, PWM, ANALOG, etc.).
 *
 * @param pin Estructura PinInfo.
 * @return Familia del pin.
 */
const char* pinFamily(const PinInfo pin) {
    return pin.family;
}


/**
 * @brief Verifica si un pin pertenece a la familia GPIO.
 *
 * @param pin Pin a validar.
 * @return true si es GPIO válido, false si no.
 */
bool isValidGPIO(const PinInfo& pin) {
    for (size_t i = 0; i < Pins::NUM_GPIO; ++i) {
        if (Pins::GPIO[i].number == pin.number &&
            Pins::GPIO[i].family == pin.family) 
        {
            return true;
        }
    }

    return false;
};


/**
 * @brief Verifica si un pin pertenece a la familia PWM.
 *
 * @param pin Pin a validar.
 * @return true si es PWM válido, false si no.
 */
bool isValidPWM(const PinInfo& pin) {
    for (size_t i = 0; i < Pins::NUM_PWM; ++i) {
        if (Pins::PWM[i].number == pin.number &&
            Pins::PWM[i].family == pin.family) 
        {
            return true;
        }
    }

    return false;
};


/**
 * @brief Verifica si un pin pertenece a la familia ANALOG.
 *
 * @param pin Pin a validar.
 * @return true si es ANALOG válido, false si no.
 */
bool isValidAnalog(const PinInfo& pin) {
    for (size_t i = 0; i < Pins::NUM_ANALOG; ++i) {
        if (Pins::ANALOG[i].number == pin.number &&
            Pins::ANALOG[i].family == pin.family) 
        {
            return true;
        }
    }

    return false;
};
