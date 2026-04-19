#ifndef PID_H
#define PID_H

#include <Arduino.h>

/**
 * @class PID
 * @brief Controlador PID con anti-windup, derivative-on-measurement
 *        y limitación de cambio de salida (slew-rate limit).
 *
 * Este controlador está diseñado para sistemas sensibles como motores paso a paso,
 * actuadores lentos o sistemas donde se requiere suavidad y estabilidad.
 */
class PID
{
public:

    /**
     * @brief Constructor del controlador PID.
     *
     * @param Kp Ganancia proporcional.
     * @param Ki Ganancia integral.
     * @param Kd Ganancia derivativa.
     * @param outputMin Límite mínimo de salida.
     * @param outputMax Límite máximo de salida.
     * @param dt Tiempo de muestreo en segundos.
     * @param outputMaxChanged Máximo cambio permitido por ciclo (slew-rate).
     */
    PID(float Kp, float Ki, float Kd,
        float outputMin, float outputMax,
        float dt, float outputMaxChanged);

    /**
     * @brief Calcula la salida del controlador PID.
     *
     * @param setpoint Valor deseado.
     * @param feedback Valor medido.
     * @param debugMSG Si es true, imprime información de depuración del PID.
     * @return Salida del PID.
     */
    float calculoPID(float setpoint, float feedback, bool debugMSG = false);

    /**
     * @brief Establece nuevas ganancias PID.
     *
     * @param Kp Nueva ganancia proporcional.
     * @param Ki Nueva ganancia integral.
     * @param Kd Nueva ganancia derivativa.
     */
    void setKPID(float Kp, float Ki, float Kd);

    /**
     * @brief Establece los límites de salida del PID.
     *
     * @param outputMin Nuevo límite mínimo.
     * @param outputMax Nuevo límite máximo.
     */
    void setOutputLimits(float outputMin, float outputMax);

    /**
     * @brief Establece un nuevo tiempo de muestreo.
     *
     * @param dt Nuevo dt.
     */
    void setDt(float dt);

    /**
     * @brief Reinicia el estado interno del PID.
     */
    void resetPID();

    /**
     * @brief Valida y corrige el tiempo de muestreo.
     *
     * @param dt Tiempo de muestreo solicitado.
     * @return dt válido (mínimo 0.1f).
     */
    float statedt(float dt);
    
    /**
     * @brief Imprime información de depuración del PID.
     *
     * @param setpoint Valor deseado.
     * @param feedback Valor medido.
     */
    void printDebugPID(float setpoint, float feedback);

private:

    // Ganancias del controlador PID
    float Kp; ///< Ganancia proporcional
    float Ki; ///< Ganancia integral
    float Kd; ///< Ganancia derivativa

    // Límites de salida
    float outputMin; ///< Límite mínimo de salida
    float outputMax; ///< Límite máximo de salida

    // Intervalo de tiempo
    float dt; ///< Tiempo de muestreo

    // Estado interno
    float error;        ///< Error actual
    float prevError;    ///< Error previo
    float proportional; ///< Término proporcional
    float integral;     ///< Acumulador integral
    float pidNoIntegral;///< PID sin término integral
    bool  saturated;    ///< Indica si el PID está saturado
    float derivative;   ///< Término derivativo
    float output;       ///< Salida final del PID
    float rawOutput;    ///< Salida sin limitación de cambio
    float prevOutput;   ///< Salida previa
    float deltaOutput;  ///< Cambio entre salidas
    float prevFeedback; ///< Feedback previo para derivative-on-measurement

    // Limitación de cambio por ciclo
    float outputMaxChanged; ///< Máximo cambio permitido por ciclo
};

#endif // PID_H
