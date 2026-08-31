#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include <cstdint>
#include "Interfaces/IDriverMotor.h"
#include "Interfaces/IDriverDistanceSensor.h"
#include "Interfaces/IDriverLineSensor.h"

namespace robolib
{

    // Forward declaration
    class RobotBuilder;

    /**
     * @file Robot.h
     * @brief Clase principal que representa un robot compuesto por drivers de hardware.
     *
     * Implementa el patrón **Composite** para agrupar múltiples actuadores y sensores
     * bajo una API unificada. Usa **inyección de dependencias** vía RobotBuilder
     * para desacoplar la configuración de hardware de la lógica de control.
     *
     * @ingroup core
     */

    /**
     * @brief Robot compuesto por motores y sensores de distancia.
     *
     * Responsabilidades:
     * - Gestiona ciclo de vida de drivers (ownership: los elimina en destructor)
     * - Expone API unificada para control diferencial y lecturas de sensores
     * - Valida índices y punteros nulos antes de delegar a drivers
     *
     * Diseño:
     * - Constructor privado + RobotBuilder: construcción fluida y segura
     * - Rule of Three: destructor, copy constructor/assignment = delete (no copiable)
     * - Drivers almacenados como punteros crudos (owner) por simplicidad embebida
     *   (migración futura: std::unique_ptr para Rule of Zero)
     *
     * Ejemplo de uso:
     * @code
     * auto robot = RobotBuilder()
     *     .addMotor(new HBridgeMotorDriver(5, 18, 0, 1))   // Motor izquierdo
     *     .addMotor(new HBridgeMotorDriver(19, 21, 2, 3))  // Motor derecho
     *     .addDistanceSensor(new HCSR04SensorDriver(4, 16)) // Sensor frontal
     *     .build();
     *
     * robot->moveDifferential(200, 200);  // Adelante
     * float dist = robot->getDistance(0); // Leer sensor 0
     * delete robot; // Libera drivers
     * @endcode
     *
     * @see RobotBuilder, IDriverMotor, IDriverDistanceSensor
     * @ingroup core
     */
    class Robot
    {
    private:
        std::vector<IDriverMotor *> motors;                   ///< Drivers de motores (owner)
        std::vector<IDriverDistanceSensor *> distanceSensors; ///< Drivers sensores distancia (owner)
        std::vector<IDriverLineSensor *> lineSensors;         ///< Drivers sensores de linea (owner)

        // Solo RobotBuilder puede construir
        Robot() = default;
        friend class RobotBuilder;

        // No copiable (ownership exclusiva)
        Robot(const Robot &) = delete;
        Robot &operator=(const Robot &) = delete;

    public:
        /**
         * @brief Destructor: libera todos los drivers inyectados.
         *
         * Itera ambos vectores y llama delete en cada puntero no nulo.
         * Luego limpia los vectores.
         *
         * @post Todos los drivers destruidos, memoria liberada.
         */
        ~Robot();

        // --- Consultas de estado ---

        /**
         * @brief Obtiene número de motores registrados.
         * @return Cantidad de motores (>= 0).
         */
        size_t getMotorCount() const;

        /**
         * @brief Obtiene número de sensores de linea registrados.
         * @return Cantidad de sensores de linea (>= 0).
         */
        size_t getLineSensorCount() const;

        /**
         * @brief Obtiene número de sensores de distancia registrados.
         * @return Cantidad de sensores (>= 0).
         */
        size_t getDistanceSensorCount() const;

        /**
         * @brief Verifica si hay al menos un motor.
         * @return true si motors no está vacío.
         */
        bool hasMotors() const;

        /**
         * @brief Verifica si hay al menos un sensor de distancia.
         * @return true si distanceSensors no está vacío.
         */
        bool hasDistanceSensors() const;

        /**
         * @brief Verifica si hay al menos un sensor de linea.
         * @return true si lineSensors no está vacío.
         */
        bool hasLineSensors() const;        


// -------------- Control de Motores ----------------------------------------//
        /**
         * @brief Mueve un motor específico por índice.
         *
         * @param motorIndex Índice del motor [0, getMotorCount()-1]
         * @param speed      Velocidad [-255, 255] (ver IDriverMotor::move)
         *
         * @note No hace nada si índice inválido o puntero nulo.
         */
        void moveMotor(size_t motorIndex, int16_t speed);

        /**
         * @brief Detiene un motor específico.
         *
         * @param motorIndex Índice del motor
         * @param stacked    Modo freno (ver IDriverMotor::stop)
         *
         * @note No hace nada si índice inválido o puntero nulo.
         */
        void stopMotor(size_t motorIndex, bool stacked = false);

        /**
         * @brief Detiene todos los motores.
         *
         * @param stacked Modo freno aplicado a todos (default: false = libre)
         */
        void stopAllMotors(bool stacked = false);

        /**
         * @brief Control diferencial para robot de tracción diferencial (2 ruedas).
         *
         * Asume:
         * - motors[0] = motor izquierdo
         * - motors[1] = motor derecho
         *
         * @param leftMotorSpeed  Velocidad rueda izquierda [-255, 255]
         * @param rightMotorSpeed Velocidad rueda derecha [-255, 255]
         *
         * @note No hace nada si hay menos de 2 motores.
         * @note Convención: positivo = adelante para ambas ruedas.
         *       Para girar: moveDifferential(200, -200) -> giro sobre eje.
         */
        void moveDifferential(int16_t leftMotorSpeed, int16_t rightMotorSpeed);
//-------------------------------------------------------------------------------






//---------------- Control de Sensores de Distancia -----------------------------------

        /**
         * @brief Obtiene medición de un sensor de distancia.
         *
         * @param sensorIndex Índice del sensor [0, getDistanceSensorCount()-1]
         * @return Distancia en cm, o -1.0f si índice inválido / puntero nulo / error.
         */
        float getDistance(size_t sensorIndex);

//-------------------------------------------------------------------------------







//---------------- Control de Sensores de Linea -----------------------------------

        /**
         * @brief Obtiene medición de un sensor de linea.
         *
         * @param sensorIndex Índice del sensor [0, getLineSensorCount()-1]
         * @return true o false dependiendo de si esta o no sobre la linea.
         */
        bool getLine(size_t sensorIndex);


        void setOnLineIs1(size_t sensorIndex);

        int getRawValue(size_t sensorIndex);

        void updateTreshold(size_t sensorIndex, int newTreshold);
        

    };





    /**
     * @file Robot.h
     * @brief Builder para construcción fluida de Robot (Builder Pattern).
     *
     * Permite configurar el robot paso a paso antes de construirlo:
     * - Encadena llamadas: builder.addMotor(...).addSensor(...).build()
     * - Transfiere ownership al Robot final (std::move)
     * - Reutilizable: tras build(), el builder queda vacío para otro robot
     *
     * @ingroup core
     */
    class RobotBuilder
    {
    private:
        std::vector<IDriverMotor *> motors;
        std::vector<IDriverDistanceSensor *> distanceSensors;
        std::vector<IDriverLineSensor *> lineSensors;        


    public:
        /**
         * @brief Constructor por defecto.
         *
         * Inicializa vectores vacíos.
         */
        RobotBuilder() = default;

        /**
         * @brief Añade un driver de motor.
         *
         * @param motor Puntero a implementación de IDriverMotor (owner transferido).
         *              Debe ser puntero válido (no nullptr) y no estar ya en otro Robot.
         * @return Referencia a *this para encadenamiento.
         *
         * @note El RobotBuilder toma ownership del puntero.
         *       No eliminar manualmente tras añadir.
         */
        RobotBuilder &addMotor(IDriverMotor *motor);

        /**
         * @brief Añade un driver de sensor de distancia.
         *
         * @param sensor Puntero a implementación de IDriverDistanceSensor (owner transferido).
         * @return Referencia a *this para encadenamiento.
         *
         * @note El RobotBuilder toma ownership del puntero.
         */
        RobotBuilder &addDistanceSensor(IDriverDistanceSensor *sensor);

        /**
         * @brief Construye el Robot transfiriendo ownership de drivers.
         *
         * @return Puntero a Robot nuevo (heap). Owner: llamador debe delete.
         *
         * @post Vectores internos del builder quedan vacíos (std::move).
         *       Builder reutilizable para otro robot.
         *
         * @note Responsabilidad del llamador: delete robot cuando termine.
         */
        Robot *build();
    };

} // namespace robolib

#endif