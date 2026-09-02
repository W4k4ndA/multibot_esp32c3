#include "HBridgeMotorDriver.h"
#include <Arduino.h>

namespace robolib {

/**
 * @file HBridgeMotorDriver.cpp
 * @brief Implementación de driver para puente H genérico (L298N, TB6612, etc.).
 * 
 * Usa periférico LEDC del ESP32 para PWM de 2 canales.
 * Frecuencia: 25 kHz (fuera de rango audible, reduce vibración audible en motores).
 * Resolución: 8 bits (0-255).
 * 
 * @ingroup drivers
 */

HBridgeMotorDriver::HBridgeMotorDriver(uint8_t pinIN1, uint8_t pinIN2, uint8_t ch1, uint8_t ch2)
    : IN1(pinIN1), IN2(pinIN2), CH1(ch1), CH2(ch2) {}

void HBridgeMotorDriver::init()
{
    // Configurar pines de dirección como salidas
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    // Configurar canales LEDC: frecuencia 25 kHz, resolución 8 bits
    ledcSetup(CH1, PWM_FREQ, PWM_RES);
    ledcSetup(CH2, PWM_FREQ, PWM_RES);

    // Adjuntar pines GPIO a canales LEDC
    ledcAttachPin(IN1, CH1);
    ledcAttachPin(IN2, CH2);

    // Estado inicial: motor detenido (freno libre)
    stop(false);
}

void HBridgeMotorDriver::move(int16_t speed)
{
    // Saturar a rango PWM válido [0, 255]
    speed = constrain(speed, -255, 255);

    if (speed > 0) {
        // Adelante: CH1 = PWM, CH2 = 0
        ledcWrite(CH1, static_cast<uint32_t>(speed));
        ledcWrite(CH2, 0);
    }
    else if (speed < 0) {
        // Reversa: CH1 = 0, CH2 = |PWM|
        ledcWrite(CH1, 0);
        ledcWrite(CH2, static_cast<uint32_t>(-speed));
    }
    else {
        // Cero: detener con freno libre
        stop(false);
    }
}

void HBridgeMotorDriver::stop(bool stacked)
{
    if (stacked) {
        // Freno activo (brake): ambos canales a 255 -> cortocircuito en puente H
        // Detiene el motor rápidamente pero consume corriente de mantenimiento
        ledcWrite(CH1, 255);
        ledcWrite(CH2, 255);
    }
    else {
        // Freno libre (coasting): ambos canales a 0 -> motor gira libre
        ledcWrite(CH1, 0);
        ledcWrite(CH2, 0);
    }
}

} // namespace robolib