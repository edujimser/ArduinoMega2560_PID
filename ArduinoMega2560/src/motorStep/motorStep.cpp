#include "motorStep.h"

//
// =============================================================
//  CONSTRUCTOR
// =============================================================
//

/**
 * @brief Inicializa el motor paso a paso configurando los pines y el estado inicial.
 */
MotorPaso::MotorPaso(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD)
    : pinA(pinA), pinB(pinB), pinC(pinC), pinD(pinD),
      retardoPaso(2000), tiempoUltimoPaso(0), pasoActual(0),
      gradoActual(0), gradosObjetivo(0), gradosCorriendo(0),
      direccionAdelante(false), direccionAtras(false)
{
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    pinMode(pinC, OUTPUT);
    pinMode(pinD, OUTPUT);
}

//
// =============================================================
//  CONFIGURACIÓN GENERAL
// =============================================================
//

/**
 * @brief Establece la velocidad del motor en RPM calculando el retardo entre pasos.
 */
void MotorPaso::fijarVelocidad(float rpm) {
    this->retardoPaso = (60.0 * 1000000.0) / (rpm * this->pasosPorVuelta);
    this->rpm = rpm;
}

//
// =============================================================
//  CONTROL BÁSICO DE PASOS (SECUENCIA 0–7)
// =============================================================
//

/**
 * @brief Avanza un paso en la secuencia del motor.
 */
void MotorPaso::pasoAdelante() {
    this->pasoActual++;
    if (this->pasoActual > 7) this->pasoActual = 0;

    this->aplicarPaso(this->pasoActual);
    delayMicroseconds(this->retardoPaso);
}

/**
 * @brief Retrocede un paso en la secuencia del motor.
 */
void MotorPaso::pasoAtras() {
    this->pasoActual--;
    if (this->pasoActual < 0) this->pasoActual = 7;

    this->aplicarPaso(this->pasoActual);
    delayMicroseconds(this->retardoPaso);
}

/**
 * @brief Activa las bobinas según el índice de paso (0–7).
 */
void MotorPaso::aplicarPaso(int i) {
    switch (i) {
        case 0: digitalWrite(pinA, HIGH); digitalWrite(pinB, LOW);  digitalWrite(pinC, LOW);  digitalWrite(pinD, LOW);  break;
        case 1: digitalWrite(pinA, HIGH); digitalWrite(pinB, HIGH); digitalWrite(pinC, LOW);  digitalWrite(pinD, LOW);  break;
        case 2: digitalWrite(pinA, LOW);  digitalWrite(pinB, HIGH); digitalWrite(pinC, LOW);  digitalWrite(pinD, LOW);  break;
        case 3: digitalWrite(pinA, LOW);  digitalWrite(pinB, HIGH); digitalWrite(pinC, HIGH); digitalWrite(pinD, LOW);  break;
        case 4: digitalWrite(pinA, LOW);  digitalWrite(pinB, LOW);  digitalWrite(pinC, HIGH); digitalWrite(pinD, LOW);  break;
        case 5: digitalWrite(pinA, LOW);  digitalWrite(pinB, LOW);  digitalWrite(pinC, HIGH); digitalWrite(pinD, HIGH); break;
        case 6: digitalWrite(pinA, LOW);  digitalWrite(pinB, LOW);  digitalWrite(pinC, LOW);  digitalWrite(pinD, HIGH); break;
        case 7: digitalWrite(pinA, HIGH); digitalWrite(pinB, LOW);  digitalWrite(pinC, LOW);  digitalWrite(pinD, HIGH); break;
    }
}

//
// =============================================================
//  MOVIMIENTO CONTINUO
// =============================================================
//

/**
 * @brief Ejecuta movimiento continuo según la velocidad configurada.
 */
void MotorPaso::movimientoContinuo() {
    unsigned long ahora = micros();

    if (ahora - this->tiempoUltimoPaso >= this->retardoPaso) {
        this->tiempoUltimoPaso = ahora;
        this->pasoAdelante();
    }
}

//
// =============================================================
//  MOVIMIENTO RELATIVO (ÁNGULOS)
// =============================================================
//

/**
 * @brief Mueve el motor hacia un ángulo relativo usando el camino más corto.
 */
void MotorPaso::movimientoPosicionRelativo(float grados, bool debugMSG) {

    this->diferencia = this->diferenciaCircular(this->gradoActual, grados);
    this->pasos = convertirGradosAPasos(abs(this->diferencia));

    this->direccionAdelante = (this->diferencia > 0);
    this->direccionAtras    = (this->diferencia < 0);

    if (this->direccionAtras) {
        for (int i = 0; i < this->pasos; i++) {
            this->pasoAtras();
            movimientoDePosicionGradosCorriendo(i);
            if (debugMSG) debugMovimientoRelativo(i);
        }
    }

    if (this->direccionAdelante) {
        for (int i = 0; i < this->pasos; i++) {
            this->pasoAdelante();
            movimientoDePosicionGradosCorriendo(i);
            if (debugMSG) debugMovimientoRelativo(i);
        }
    }

    this->gradoActual = normalizarGrados(grados);
}

//
// =============================================================
//  PROGRESO DURANTE EL MOVIMIENTO
// =============================================================
//

/**
 * @brief Actualiza y devuelve los grados recorridos durante el movimiento.
 */
float MotorPaso::movimientoDePosicionGradosCorriendo(int movimientoPasos) {
    this->gradosCorriendo = convertirPasosAGrados(movimientoPasos);
    return this->gradosCorriendo;
}

//
// =============================================================
//  MATEMÁTICAS DE ÁNGULOS Y PASOS
// =============================================================
//

/**
 * @brief Convierte grados a pasos del motor.
 */
int MotorPaso::convertirGradosAPasos(float grados) {
    return (int)(abs(grados) * this->pasosPorVuelta / 360.0f);
}

/**
 * @brief Convierte pasos del motor a grados.
 */
float MotorPaso::convertirPasosAGrados(int pasos) {
    return (float)pasos * 360.0f / this->pasosPorVuelta;
}

/**
 * @brief Calcula la diferencia circular entre dos ángulos.
 */
float MotorPaso::diferenciaCircular(float actual, float objetivo) {
    float diff = objetivo - actual;

    if (diff > 180.0f)  diff -= 360.0f;
    if (diff < -180.0f) diff += 360.0f;

    return diff;
}

/**
 * @brief Normaliza un ángulo al rango 0–360 grados.
 */
float MotorPaso::normalizarGrados(float grados) {
    float g = fmod(grados, 360.0f);
    return (g < 0) ? g + 360.0f : g;
}

//
// =============================================================
//  DEPURACIÓN
// =============================================================
//

/**
 * @brief Imprime información detallada del movimiento relativo.
 */
void MotorPaso::debugMovimientoRelativo(int pasoActualDelBucle) {

    Serial.println("====================================");
    Serial.print("Paso actual: ");
    Serial.print(pasoActualDelBucle);
    Serial.print(" / ");
    Serial.println(this->pasos);

    Serial.print("Diferencia circular: ");
    Serial.print(this->diferencia);
    Serial.println(" grados");

    Serial.print("Pasos totales calculados: ");
    Serial.println(this->pasos);

    Serial.print("Dirección: ");
    if (this->direccionAdelante) Serial.println("Adelante (+)");
    else if (this->direccionAtras) Serial.println("Atrás (–)");
    else Serial.println("Sin movimiento");

    Serial.print("Grados recorridos hasta ahora: ");
    Serial.print(this->gradosCorriendo);
    Serial.println(" grados");

    Serial.print("Grado actual (antes de normalizar): ");
    Serial.println(this->gradoActual);

    Serial.println("====================================");
}
