#ifndef DRIVER_MOTOR_H
#define DRIVER_MOTOR_H

#include <cstdint>

/**
 * @file IDriverMotor.h
 * @brief Interfaz abstracta para controladores de motores.
 * 
 * Define el contrato que deben implementar todos los drivers de motores
 * compatibles con el sistema (L298N, TB6612, controladores brushless, etc.).
 * 
 * @ingroup drivers
 */

namespace robolib {

/**
 * @brief Interfaz base para drivers de motores DC con puente H.
 * 
 * Esta interfaz estandariza el control de motores permitiendo intercambiar
 * implementaciones (L298N, TB6612, BTS7960, etc.) sin cambiar la lógica
 * de alto nivel. Cada motor se controla independientemente.
 * 
 * Convención de velocidad:
 * - Rango: -255 a 255 (8 bits PWM)
 * - Positivo: dirección adelante
 * - Negativo: dirección reversa
 * - Cero: detenido (freno o libre según implementación)
 * 
 * @see HBridgeMotorDriver
 * @ingroup drivers
 */
class IDriverMotor {
public:
    /**
     * @brief Destructor virtual por defecto.
     * 
     * Necesario para destrucción correcta de objetos derivados mediante
     * puntero a la interfaz (polimorfismo).
     */
    virtual ~IDriverMotor() = default;

    /**
     * @brief Inicializa el hardware del driver de motor.
     * 
     * Configura pines GPIO (dirección, PWM), timers LEDC, frecuencia
     * y resolución PWM. Debe llamarse exactamente una vez antes de
     * cualquier operación de movimiento.
     * 
     * @post El motor queda en estado detenido y listo para recibir comandos.
     */
    virtual void begin() = 0;

    /**
     * @brief Establece la velocidad y dirección del motor.
     * 
     * @param speed Velocidad objetivo (-255 a 255).
     *              - >0: adelante con duty cycle = speed
     *              - <0: reversa con duty cycle = |speed|
     *              - 0:  detenido (ver stop())
     * 
     * @note La implementación debe saturar valores fuera de rango a [-255, 255].
     * @note No bloquea; el cambio de velocidad es inmediato.
     */
    virtual void move(int16_t speed) = 0;

    /**
     * @brief Detiene el motor.
     * 
     * @param stacked Modo de frenado:
     *                - false (default): freno libre (coasting), duty = 0 en ambos canales
     *                - true: freno activo (brake), duty = 255 en ambos canales (cortocircuito en puente H)
     * 
     * @note Con stacked=true el motor frena más rápido pero consume más corriente.
     *       Útil para paradas de emergencia o posicionamiento preciso.
     */
    virtual void stop(bool stacked = false) = 0;
};

} // namespace robolib

#endif