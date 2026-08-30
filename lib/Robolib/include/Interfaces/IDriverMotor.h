#ifndef DRIVER_MOTOR_H
#define DRIVER_MOTOR_H

#include <cstdint>

/**
 * @brief Interfaz abstracta para controladores de motores.
 * 
 * Define el contrato que deben implementar todos los drivers de motores
 * compatibles con el sistema (L298N, TB6612, controladores brushless, etc.).
 */
class IDriverMotor {
public:
    /**
     * @brief Destructor virtual por defecto.
     * 
     * Necesario para destrucción correcta de objetos derivados mediante
     * puntero a la interfaz.
     */
    virtual ~IDriverMotor() = default;

    /**
     * @brief Inicializa el driver de motores.
     * 
     * Configura el hardware del driver (pines PWM, dirección, habilitación,
     * frecuencia PWM, etc.). Debe llamarse antes de cualquier movimiento.
     */
    virtual void begin() = 0;

    /**
     * @brief Establece la velocidad del motor.
     * 
     * @param speed Velocidad del motor (-255 a 255).
     *                     Negativo = reversa, positivo = adelante.
     */
    virtual void move(int16_t speed) = 0;

    /**
     * @brief Detiene el motor inmediatamente.
     * 
     * Frena el motor (cortocircuito en puente H) o establece velocidad 0.
     */
    virtual void stop(bool stacked = false) = 0;
};

#endif