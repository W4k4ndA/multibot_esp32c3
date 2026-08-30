#include "Controllers/Sensors/Distance/HCSR04SensorDriver.h"
#include <Arduino.h>

namespace robolib {

/**
 * @file HCSR04SensorDriver.cpp
 * @brief Implementación de driver para sensor ultrasónico HC-SR04/HC-SR04P.
 * 
 * Medición por Time of Flight (ToF) a 40 kHz.
 * Fórmula: distancia_cm = (tiempo_echo_us * 0.0343) / 2
 * 
 * @ingroup drivers
 */

HCSR04SensorDriver::HCSR04SensorDriver(uint8_t triggerPin, uint8_t echoPin)
    : trigPin(triggerPin), echoPin(echoPin) {}

void HCSR04SensorDriver::begin()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Asegurar estado bajo inicial en Trigger
    digitalWrite(trigPin, LOW);
}

float HCSR04SensorDriver::measureCM()
{
    // 1. Pulso de estabilización: LOW 2 µs
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // 2. Pulso de disparo: HIGH 10 µs (especificación HC-SR04)
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // 3. Medir ancho de pulso HIGH en Echo (timeout 30 ms ≈ 5 m)
    // pulseIn retorna 0 si timeout
    uint32_t duration = pulseIn(echoPin, HIGH, TIMEOUT_US);

    // 4. Manejar timeout / error
    if (duration == 0) {
        return -1.0f;  // Timeout: objeto fuera de rango o sin eco
    }

    // 5. Calcular distancia: (duración * velocidad_sonido) / 2
    // Dividimos por 2 porque el sonido viaja ida + vuelta
    float distance = duration * SOUND_SPEED_CM_US / 2.0f;

    // 6. Aplicar límites físicos del sensor
    if (distance < MIN_DISTANCE_CM) {
        return MIN_DISTANCE_CM;  // Zona ciega (< 2 cm)
    }
    if (distance > MAX_DISTANCE_CM) {
        return MAX_DISTANCE_CM;  // Fuera de rango especificado (> 400 cm)
    }

    return distance;
}

} // namespace robolib