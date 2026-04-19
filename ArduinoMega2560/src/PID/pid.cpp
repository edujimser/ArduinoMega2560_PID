/**
 * @file pid.cpp
 * @brief Implementación del controlador PID con anti-windup, derivative-on-measurement
 *        y limitación de cambio de salida (slew-rate limit).
 */

#include "pid.h"

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
PID::PID(float Kp, float Ki, float Kd,
         float outputMin, float outputMax,
         float dt, float outputMaxChanged)
{
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;

    this->outputMin = outputMin;
    this->outputMax = outputMax;

    this->dt = statedt(dt);
    this->outputMaxChanged = outputMaxChanged;

    this->error = 0.0f;
    this->prevError = 0.0f;
    this->integral = 0.0f;
    this->derivative = 0.0f;
    this->output = 0.0f;

    this->prevFeedback = 0.0f;
    this->prevOutput = 0.0f;
}

/**
 * @brief Calcula la salida del controlador PID.
 *
 * Implementa:
 * - Proporcional
 * - Integral con anti-windup condicional
 * - Derivative-on-measurement (evita derivative kick)
 * - Slew-rate limit (limitación de cambio por ciclo)
 * - Saturación final
 *
 * @param setpoint Valor deseado.
 * @param feedback Valor medido.
 * @return Salida del controlador PID.
 */
float PID::calculoPID(float setpoint, float feedback, bool debugMSG)
{
    // 1) Error actual
    this->error = setpoint - feedback;

    // 2) Proporcional
    this->proportional = this->Kp * this->error;

    // Protección dt
    if (this->dt <= 0.0f)
        return this->output;

    // 3) Derivada (derivative-on-measurement)
    this->derivative = -(feedback - this->prevFeedback) / this->dt;

    // 4) Anti-windup condicional
    this->pidNoIntegral = this->proportional + this->Kd * this->derivative;
    this->saturated = (this->pidNoIntegral > this->outputMax) || (this->pidNoIntegral < this->outputMin);

    if (!this->saturated)
    {
        this->integral += this->error * this->dt;

        if (this->integral > this->outputMax) this->integral = this->outputMax;
        if (this->integral < this->outputMin) this->integral = this->outputMin;
    }

    // 5) PID completo sin limitación de cambio
    this->rawOutput = this->proportional +
                      (this->Ki * this->integral) +
                      (this->Kd * this->derivative);

    // 6) Slew-rate limit
    this->deltaOutput = this->rawOutput - this->prevOutput;

    if (this->deltaOutput > this->outputMaxChanged)
        this->output = this->prevOutput + this->outputMaxChanged;
    else if (this->deltaOutput < -this->outputMaxChanged)
        this->output = this->prevOutput - this->outputMaxChanged;
    else
        this->output = this->rawOutput;

    // 7) Saturación final
    if (this->output > this->outputMax) this->output = this->outputMax;
    if (this->output < this->outputMin) this->output = this->outputMin;

    // 8) Guardar estado
    this->prevError = this->error;
    this->prevFeedback = feedback;
    this->prevOutput = this->output;


    // 9) Debug
    if (debugMSG) printDebugPID(setpoint, feedback);

    return this->output;

};

/**
 * @brief Establece nuevas ganancias PID.
 *
 * @param Kp Nueva ganancia proporcional.
 * @param Ki Nueva ganancia integral.
 * @param Kd Nueva ganancia derivativa.
 */
void PID::setKPID(float Kp, float Ki, float Kd)
{
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}

/**
 * @brief Establece los límites de salida del PID.
 *
 * @param outputMin Nuevo límite mínimo.
 * @param outputMax Nuevo límite máximo.
 */
void PID::setOutputLimits(float outputMin, float outputMax)
{
    this->outputMin = outputMin;
    this->outputMax = outputMax;
}

/**
 * @brief Establece un nuevo tiempo de muestreo.
 *
 * @param dt Nuevo dt.
 */
void PID::setDt(float dt)
{
    this->dt = statedt(dt);
}

/**
 * @brief Reinicia el estado interno del PID.
 */
void PID::resetPID()
{
    this->error = 0.0f;
    this->prevError = 0.0f;
    this->integral = 0.0f;
    this->derivative = 0.0f;
    this->output = 0.0f;
    this->prevFeedback = 0.0f;
    this->prevOutput = 0.0f;
}

/**
 * @brief Valida el tiempo de muestreo.
 *
 * @param dt Tiempo de muestreo solicitado.
 * @return dt válido (mínimo 0.1f).
 */
float PID::statedt(float dt)
{
    if (dt <= 0.0f)
        return 0.1f;

    return dt;
}


/**
 * @brief Muestra en consola el estado interno del PID en formato tabla.
 *
 * @param setpoint Valor deseado.
 * @param feedback Valor medido.
 */
void PID::printDebugPID(float setpoint, float feedback)
{
    Serial.println(F("--------------------------------------------------------------------------"));
    Serial.println(F("|  SP  |  FB  | Error |    P     |    I     |    D     | RawOut |  Out  |"));
    Serial.println(F("--------------------------------------------------------------------------"));

    Serial.print(F("| "));
    Serial.print(setpoint, 3);
    Serial.print(F(" | "));

    Serial.print(feedback, 3);
    Serial.print(F(" | "));

    Serial.print(this->error, 3);
    Serial.print(F(" | "));

    Serial.print(this->proportional, 3);
    Serial.print(F(" | "));

    Serial.print(this->integral, 3);
    Serial.print(F(" | "));

    Serial.print(this->derivative, 3);
    Serial.print(F(" | "));

    Serial.print(this->rawOutput, 3);
    Serial.print(F(" | "));

    Serial.print(this->output, 3);
    Serial.println(F(" |"));

    Serial.println(F("--------------------------------------------------------------------------"));
}