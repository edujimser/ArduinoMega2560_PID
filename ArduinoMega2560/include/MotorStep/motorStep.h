#ifndef MOTOR_PASO_H
#define MOTOR_PASO_H

#include <Arduino.h>

#define PASOS_POR_VUELTA 4096

/**
 * @class MotorPaso
 * @brief Controlador para motores paso a paso 28BYJ-48 con driver ULN2003.
 *
 * Esta clase permite:
 * - Controlar pasos individuales (adelante/atrás)
 * - Establecer velocidad en RPM
 * - Realizar movimientos relativos en grados
 * - Obtener información del progreso del movimiento
 * - Depurar el comportamiento mediante mensajes por consola
 *
 * La clase está organizada por familias funcionales para facilitar
 * la lectura, mantenimiento y generación de documentación.
 */
class MotorPaso {
public:

    // =========================================================
    //  CONSTRUCTOR
    // =========================================================

    /**
     * @brief Constructor del motor paso a paso.
     * @param pinA Pin conectado a la bobina A.
     * @param pinB Pin conectado a la bobina B.
     * @param pinC Pin conectado a la bobina C.
     * @param pinD Pin conectado a la bobina D.
     */
    MotorPaso(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD);

    // =========================================================
    //  CONFIGURACIÓN GENERAL
    // =========================================================

    /**
     * @brief Establece la velocidad del motor en RPM.
     * @param rpm Revoluciones por minuto.
     */
    void fijarVelocidad(float rpm);

    // =========================================================
    //  CONTROL BÁSICO DE PASOS
    // =========================================================

    /**
     * @brief Avanza un paso en la secuencia del motor.
     */
    void pasoAdelante();

    /**
     * @brief Retrocede un paso en la secuencia del motor.
     */
    void pasoAtras();

    /**
     * @brief Aplica un paso específico de la secuencia (0–7).
     * @param indicePaso Índice de la secuencia.
     */
    void aplicarPaso(int indicePaso);

    // =========================================================
    //  MOVIMIENTO CONTINUO
    // =========================================================

    /**
     * @brief Ejecuta movimiento continuo según la velocidad configurada.
     */
    void movimientoContinuo();

    // =========================================================
    //  MOVIMIENTO RELATIVO (ÁNGULOS)
    // =========================================================

    /**
     * @brief Mueve el motor hacia un ángulo relativo usando el camino más corto.
     * @param grados Ángulo objetivo en grados.
     * @param debugMSG Si es true, imprime información detallada del movimiento.
     */
    void movimientoPosicionRelativo(float grados, bool debugMSG = false);

    // =========================================================
    //  MATEMÁTICAS DE ÁNGULOS Y PASOS
    // =========================================================

    /**
     * @brief Calcula la diferencia circular entre dos ángulos.
     * @param gradosActuales Ángulo actual.
     * @param gradosObjetivo Ángulo objetivo.
     * @return Diferencia circular entre -180 y +180 grados.
     */
    float diferenciaCircular(float gradosActuales, float gradosObjetivo);

    /**
     * @brief Normaliza un ángulo al rango 0–360 grados.
     * @param grados Ángulo a normalizar.
     * @return Ángulo normalizado.
     */
    float normalizarGrados(float grados);

    /**
     * @brief Convierte grados a pasos del motor.
     * @param grados Ángulo en grados.
     * @return Número de pasos equivalentes.
     */
    int convertirGradosAPasos(float grados);

    /**
     * @brief Convierte pasos del motor a grados.
     * @param pasos Número de pasos.
     * @return Ángulo equivalente en grados.
     */
    float convertirPasosAGrados(int pasos);

    // =========================================================
    //  PROGRESO DURANTE EL MOVIMIENTO
    // =========================================================

    /**
     * @brief Actualiza y devuelve los grados recorridos durante el movimiento.
     * @param movimientoPasos Pasos ejecutados hasta el momento.
     * @return Grados recorridos.
     */
    float movimientoDePosicionGradosCorriendo(int movimientoPasos);

    // =========================================================
    //  DEPURACIÓN
    // =========================================================

    /**
     * @brief Imprime información detallada del movimiento relativo.
     * @param pasoActualDelBucle Paso actual dentro del bucle de movimiento.
     */
    void debugMovimientoRelativo(int pasoActualDelBucle);

private:

    // =========================================================
    //  HARDWARE (PINES DEL MOTOR)
    // =========================================================

    uint8_t pinA;   ///< Bobina A
    uint8_t pinB;   ///< Bobina B
    uint8_t pinC;   ///< Bobina C
    uint8_t pinD;   ///< Bobina D

    // =========================================================
    //  CONTROL DE VELOCIDAD Y TIEMPO
    // =========================================================

    unsigned long retardoPaso;      ///< Microsegundos entre pasos (determina la velocidad)
    unsigned long tiempoUltimoPaso; ///< Marca temporal del último paso ejecutado
    short rpm;                      ///< Velocidad configurada en RPM

    // =========================================================
    //  ESTADO DEL MOTOR (SECUENCIA DE PASOS)
    // =========================================================

    int pasoActual;                 ///< Índice de la secuencia (0–7)
    int pasosPorVuelta = PASOS_POR_VUELTA; ///< Pasos necesarios para 360°

    // =========================================================
    //  MOVIMIENTO RELATIVO (DATOS DEL MOVIMIENTO)
    // =========================================================

    int pasos;                      ///< Pasos totales a ejecutar en el movimiento actual
    float gradoActual;              ///< Ángulo actual del motor (0–360 normalizado)
    float gradosObjetivo;           ///< Ángulo objetivo solicitado
    float gradosCorriendo;          ///< Ángulo recorrido durante el movimiento (progreso)
    float diferencia;               ///< Diferencia circular entre gradoActual y gradosObjetivo (–180 a +180)
    bool direccionAdelante;         ///< Dirección: true = adelante
    bool direccionAtras;            ///< Dirección: true = atrás
};

#endif // MOTOR_PASO_H
