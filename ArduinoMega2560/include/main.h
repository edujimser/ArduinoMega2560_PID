/**
 * @file main.h
 * @brief Archivo principal de cabecera del firmware. Contiene metadatos del firmware,
 *        includes del sistema, configuración de pines, diagnósticos, motor paso a paso
 *        y controlador PID.
 *
 * Este archivo centraliza todas las dependencias necesarias para el funcionamiento
 * del firmware en Arduino Mega 2560.
 */

#ifndef MAIN_H
#define MAIN_H

/**
 * @brief Includes del sistema y librerías necesarias para el firmware.
 *
 * - pinout.h: Definición de pines del sistema
 * - avr8-stub.h: Soporte para depuración con GDB
 * - HardwareSerial.h: Comunicación serie
 * - diagnosticsUART.h: Diagnóstico de UART
 * - diagnosticsEEPROM.h: Diagnóstico de EEPROM
 * - config.h: Parámetros de configuración del sistema
 * - motorStep.h: Control del motor paso a paso
 * - pid.h: Controlador PID
 */
#include "system/pinout/pinout.h"
#include <avr8-stub.h>                                              // AVR8 debugging stub for GDB
#include <HardwareSerial.h>                                         // Serial communication support
#include "system/diagnostics/diagnosticsUART.h"                     // UART diagnostics functions
#include "system/diagnostics/diagnosticsEEPROM.h"
#include "system/config/config.h"                                   // System configuration parameters
#include "system/pinout/pinout.h"                                   // Pinout definitions
#include "MotorStep/motorStep.h"
#include "PID/pid.h"

/**
 * @brief Metadatos del firmware.
 *
 * Incluye información sobre:
 * - versión del firmware
 * - nombre del firmware
 * - fecha de compilación
 * - autor
 * - metadatos de la aplicación asociada
 */
#define FIRMWARE_VERSION                 "1.0.B"                                    ///< Firmware version
#define FIRMWARE_NAME                    "ArduinoMega2560"                          ///< Firmware name
#define FIRMWARE_DATE                    "17/12/2025"                               ///< Firmware compilation date
#define FIRMWARE_AUTHOR                  "Eduardo Jimenez Serrato"                  ///< Firmware author
#define FIRMWARE_VERSION_APP             "N.A"                                      ///< Firmware version for application
#define FIRMWARE_NAME_APP                "N.A"                                      ///< Firmware name
#define FIRMWARE_DATE_APP                "N.A"                                      ///< Firmware compilation date

#endif  // MAIN_H
