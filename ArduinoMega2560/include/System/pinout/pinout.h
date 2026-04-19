#ifndef PINS_H
#define PINS_H

#include <Arduino.h>
#include "system/msg/msg.h"

/**
 * @struct PinInfo
 * @brief Representa un pin del sistema con nombre simbólico, número físico y familia funcional.
 *
 * Esta estructura permite describir cada pin de forma clara y tipada,
 * facilitando diagnósticos, documentación y uso en tiempo de compilación.
 *
 * @param name   Nombre simbólico del pin (ej. "PIN_PWM_10").
 * @param number Número físico del pin en la placa Arduino.
 * @param family Familia funcional (ej. "GPIO", "PWM", "ANALOG", "UART_RX").
 */
struct PinInfo {
    const char* name;       // Symbolic name of the pin
    int number;             // Physical pin number on the board
    const char* family;     // Pin family or category (e.g., "PWM", "GPIO")

    /// Constructor constexpr para permitir uso en evaluaciones de tiempo de compilación
    constexpr PinInfo(const char* n, int num, const char* fam) : name(n), number(num), family(fam) {}
};


/**
 * @namespace Pins
 * @brief Contiene todos los arrays de pines organizados por familia funcional.
 *
 * Este namespace agrupa:
 * - UART RX/TX
 * - SPI
 * - I2C
 * - INTERRUPTS
 * - ANALOG
 * - PWM
 * - GPIO
 *
 * Además, define constantes con el número total de pines por familia.
 */
namespace Pins {

    /**
     * @brief Pines UART RX (recepción serie asíncrona).
     */
    inline constexpr PinInfo UART_RX[] = {
        {"PIN_RX0", 0,  "UART_RX"},
        {"PIN_RX1", 19, "UART_RX"},
        {"PIN_RX2", 17, "UART_RX"},
        {"PIN_RX3", 15, "UART_RX"},
    };

    /**
     * @brief Pines UART TX (transmisión serie asíncrona).
     */
    inline constexpr PinInfo UART_TX[] = {
        {"PIN_TX0", 1,  "UART_TX"},
        {"PIN_TX1", 18, "UART_TX"},
        {"PIN_TX2", 16, "UART_TX"},
        {"PIN_TX3", 14, "UART_TX"},
    };

    /**
     * @brief Pines SPI (comunicación síncrona maestro-esclavo).
     */
    inline constexpr PinInfo SPI[] = {
        {"PIN_MISO", 50, "SPI"},
        {"PIN_MOSI", 51, "SPI"},
        {"PIN_SCK",  52, "SPI"},
        {"PIN_SS",   53, "SPI"},
    };

    /**
     * @brief Pines I2C / TWI (comunicación de dos hilos).
     */
    inline constexpr PinInfo I2C[] = {
        {"PIN_SDA", 20, "I2C"},
        {"PIN_SCL", 21, "I2C"},
    };

    /**
     * @brief Pines de interrupciones externas.
     */
    inline constexpr PinInfo INTERRUPTS[] = {
        {"INT_0", 2,  "INTERRUPTS"},
        {"INT_1", 3,  "INTERRUPTS"},
        {"INT_2", 21, "INTERRUPTS"},
        {"INT_3", 20, "INTERRUPTS"},
        {"INT_4", 19, "INTERRUPTS"},
        {"INT_5", 18, "INTERRUPTS"},
    };

    /**
     * @brief Pines analógicos (ADC).
     */
    inline constexpr PinInfo ANALOG[] = {
        {"PIN_ANALOG_A00", 54, "ANALOG"},
        {"PIN_ANALOG_A01", 55, "ANALOG"},
        {"PIN_ANALOG_A02", 56, "ANALOG"},
        {"PIN_ANALOG_A03", 57, "ANALOG"},
        {"PIN_ANALOG_A04", 58, "ANALOG"},
        {"PIN_ANALOG_A05", 59, "ANALOG"},
        {"PIN_ANALOG_A06", 60, "ANALOG"},
        {"PIN_ANALOG_A07", 61, "ANALOG"},
        {"PIN_ANALOG_A08", 62, "ANALOG"},
        {"PIN_ANALOG_A90", 63, "ANALOG"},
        {"PIN_ANALOG_A10", 64, "ANALOG"},
        {"PIN_ANALOG_A11", 65, "ANALOG"},
        {"PIN_ANALOG_A12", 66, "ANALOG"},
        {"PIN_ANALOG_A13", 67, "ANALOG"},
        {"PIN_ANALOG_A14", 68, "ANALOG"},
        {"PIN_ANALOG_A15", 69, "ANALOG"},
    };

    /**
     * @brief Pines PWM (modulación por ancho de pulso).
     */
    inline constexpr PinInfo PWM[] = {
        {"PIN_PWM_02", 2,  "PWM"},
        {"PIN_PWM_03", 3,  "PWM"},
        {"PIN_PWM_04", 4,  "PWM"},
        {"PIN_PWM_05", 5,  "PWM"},
        {"PIN_PWM_06", 6,  "PWM"},
        {"PIN_PWM_07", 7,  "PWM"},
        {"PIN_PWM_08", 8,  "PWM"},
        {"PIN_PWM_09", 9,  "PWM"},
        {"PIN_PWM_10", 10, "PWM"},
        {"PIN_PWM_11", 11, "PWM"},
        {"PIN_PWM_12", 12, "PWM"},
        {"PIN_PWM_44", 44, "PWM"},
        {"PIN_PWM_45", 45, "PWM"},
        {"PIN_PWM_46", 46, "PWM"}
    };

    /**
     * @brief Pines GPIO (entrada/salida digital general).
     */
    inline constexpr PinInfo GPIO[] = {
        {"PIN_GPIO_13", 13, "GPIO"},
        {"PIN_GPIO_22", 22, "GPIO"},
        {"PIN_GPIO_23", 23, "GPIO"},
        {"PIN_GPIO_24", 24, "GPIO"},
        {"PIN_GPIO_25", 25, "GPIO"},
        {"PIN_GPIO_26", 26, "GPIO"},
        {"PIN_GPIO_27", 27, "GPIO"},
        {"PIN_GPIO_28", 28, "GPIO"},
        {"PIN_GPIO_29", 29, "GPIO"},
        {"PIN_GPIO_30", 30, "GPIO"},
        {"PIN_GPIO_31", 31, "GPIO"},
        {"PIN_GPIO_32", 32, "GPIO"},
        {"PIN_GPIO_33", 33, "GPIO"},
        {"PIN_GPIO_34", 34, "GPIO"},
        {"PIN_GPIO_35", 35, "GPIO"},
        {"PIN_GPIO_36", 36, "GPIO"},
        {"PIN_GPIO_37", 37, "GPIO"},
        {"PIN_GPIO_38", 38, "GPIO"},
        {"PIN_GPIO_39", 39, "GPIO"},
        {"PIN_GPIO_40", 40, "GPIO"},
        {"PIN_GPIO_41", 41, "GPIO"},
        {"PIN_GPIO_42", 42, "GPIO"},
        {"PIN_GPIO_43", 43, "GPIO"},
        {"PIN_GPIO_44", 44, "GPIO"},
        {"PIN_GPIO_45", 45, "GPIO"},
        {"PIN_GPIO_46", 46, "GPIO"},
        {"PIN_GPIO_47", 47, "GPIO"},
        {"PIN_GPIO_48", 48, "GPIO"},
        {"PIN_GPIO_49", 49, "GPIO"},
        {"PIN_GPIO_50", 50, "GPIO"},
        {"PIN_GPIO_51", 51, "GPIO"},
        {"PIN_GPIO_52", 52, "GPIO"},
        {"PIN_GPIO_53", 53, "GPIO"},
    };

    /// @brief Número total de pines por familia
    constexpr size_t NUM_ANALOG     = sizeof(ANALOG) / sizeof(ANALOG[0]);
    constexpr size_t NUM_GPIO       = sizeof(GPIO) / sizeof(GPIO[0]);
    constexpr size_t NUM_PWM        = sizeof(PWM) / sizeof(PWM[0]);
    constexpr size_t NUM_UART_RX    = sizeof(UART_RX) / sizeof(UART_RX[0]);
    constexpr size_t NUM_UART_TX    = sizeof(UART_TX) / sizeof(UART_TX[0]);
    constexpr size_t NUM_SPI        = sizeof(SPI) / sizeof(SPI[0]);
    constexpr size_t NUM_I2C        = sizeof(I2C) / sizeof(I2C[0]);
    constexpr size_t NUM_INTERRUPTS = sizeof(INTERRUPTS) / sizeof(INTERRUPTS[0]);

}


/**
 * @brief Devuelve el número físico del pin.
 */
uint8_t pinNumber(const PinInfo pin);

/**
 * @brief Devuelve el nombre simbólico del pin.
 */
const char* pinName(const PinInfo pin);

/**
 * @brief Devuelve la familia funcional del pin.
 */
const char* pinFamily(const PinInfo pin);

/**
 * @brief Comprueba si un pin pertenece a la familia GPIO.
 */
bool isValidGPIO(const PinInfo& pin);

/**
 * @brief Comprueba si un pin pertenece a la familia PWM.
 */
bool isValidPWM(const PinInfo& pin);

/**
 * @brief Comprueba si un pin pertenece a la familia ANALOG.
 */
bool isValidAnalog(const PinInfo& pin);


/**
 * @brief Diagnóstico de pines analógicos.
 */
void diagnoseAnalog();

/**
 * @brief Diagnóstico de pines GPIO.
 */
void diagnoseGPIO();

/**
 * @brief Diagnóstico de pines PWM.
 */
void diagnosePWM();

/**
 * @brief Ejecuta todos los diagnósticos de pines.
 */
void fullDiagnosticsPins();

#endif  // PINS_H
