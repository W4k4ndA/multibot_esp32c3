#include "Robot.h"

namespace robolib
{

    /**
     * @file Robot.cpp
     * @brief Implementación de Robot y RobotBuilder.
     *
     * Gestión de ciclo de vida de drivers con ownership explícita.
     * Validación defensiva de índices y punteros nulos.
     *
     * @ingroup core
     */

    // ============================================================================
    // Robot
    // ============================================================================


    void Robot::init(){
            for(auto m:modules){
                m->init();
            }
        }

    Robot::~Robot()
    {
        // Liberar drivers de motores (owner)
        for (auto *motor : motors)
        {
            delete motor;
        }
        motors.clear();

        // Liberar drivers de sensores de distancia (owner)
        for (auto *sensor : distanceSensors)
        {
            delete sensor;
        }
        distanceSensors.clear();

        // Liberar drivers de sensores de linea (owner)
        for (auto *sensor : lineSensors)
        {
            delete sensor;
        }
        lineSensors.clear();
    }

    //-------SECCION PARA MOTORES----------------------
    size_t Robot::getMotorCount() const
    {
        return motors.size();
    }

    bool Robot::hasMotors() const
    {
        return !motors.empty();
    }

    void Robot::moveMotor(size_t motorIndex, int16_t speed)
    {
        // Validación defensiva: índice en rango y puntero no nulo
        if (motorIndex < motors.size() && motors[motorIndex] != nullptr)
        {
            motors[motorIndex]->move(speed);
        }
        // Silencio si inválido: evita excepciones en sistemas embebidos
    }

    void Robot::stopMotor(size_t motorIndex, bool stacked)
    {
        if (motorIndex < motors.size() && motors[motorIndex] != nullptr)
        {
            motors[motorIndex]->stop(stacked);
        }
    }

    void Robot::stopAllMotors(bool stacked)
    {
        for (auto *motor : motors)
        {
            if (motor != nullptr)
            {
                motor->stop(stacked);
            }
        }
    }

    void Robot::moveDifferential(int16_t leftMotorSpeed, int16_t rightMotorSpeed)
    {
        // Requiere al menos 2 motores: [0] = izquierdo, [1] = derecho
        if (motors.size() >= 2)
        {
            if (motors[0] != nullptr)
            {
                motors[0]->move(leftMotorSpeed);
            }
            if (motors[1] != nullptr)
            {
                motors[1]->move(rightMotorSpeed);
            }
        }
    }

    //-------SECCION PARA SENSORES DE DISTANCIA----------------------
    size_t Robot::getDistanceSensorCount() const
    {
        return distanceSensors.size();
    }

    bool Robot::hasDistanceSensors() const
    {
        return !distanceSensors.empty();
    }

    float Robot::getDistance(size_t sensorIndex)
    {
        if (sensorIndex < distanceSensors.size() && distanceSensors[sensorIndex] != nullptr)
        {
            return distanceSensors[sensorIndex]->measureCM();
        }
        return -1.0f; // Error: índice inválido, sensor nulo, o error de medición
    }

    //-------SECCION PARA SENSORES DE LINEA----------------------
    size_t Robot::getLineSensorCount() const
    {
        return lineSensors.size();
    }

    bool Robot::hasLineSensors() const
    {
        return !lineSensors.empty();
    }

    bool Robot::getLine(size_t sensorIndex)
    {
        if (sensorIndex < lineSensors.size() && lineSensors[sensorIndex] != nullptr)
        {
            return lineSensors[sensorIndex]->isOnLine();
        }
        return false; // Error: índice inválido o sensor nulo
    }

    void Robot::setOnLineIs1(size_t sensorIndex)
    {
        if (sensorIndex < lineSensors.size() && lineSensors[sensorIndex] != nullptr)
        {
            lineSensors[sensorIndex]->set1IsonLine();
        }
    }

    void Robot::setOnLineIs0(size_t sensorIndex)
    {
        if (sensorIndex < lineSensors.size() && lineSensors[sensorIndex] != nullptr)
        {
            lineSensors[sensorIndex]->set0IsonLine();
        }
    }

    int Robot::getAnalogValue(size_t sensorIndex)
    {
        if (sensorIndex < lineSensors.size() && lineSensors[sensorIndex] != nullptr)
        {
            return lineSensors[sensorIndex]->getAnalogRawValue();
        }
        return -1; // error de lectura del sensor
    }

    void Robot::updateTreshold(size_t sensorIndex, int newTreshold)
    {
        if (sensorIndex < lineSensors.size() && lineSensors[sensorIndex] != nullptr)
        {
            lineSensors[sensorIndex]->setThreshold(newTreshold);
        }
    }

    // ============================================================================
    // RobotBuilder
    // ============================================================================

    RobotBuilder &RobotBuilder::addMotor(IDriverMotor *motor)
    {
        if (motor != nullptr)
        {
            motors.push_back(motor);
            modules.push_back(motor);
        }
        // Silencio si nullptr: permite encadenamiento condicional
        return *this;
    }

    RobotBuilder &RobotBuilder::addDistanceSensor(IDriverDistanceSensor *sensor)
    {
        if (sensor != nullptr)
        {
            distanceSensors.push_back(sensor);
            modules.push_back(sensor);
        }
        return *this;
    }

    RobotBuilder &RobotBuilder::addLineSensor(IDriverLineSensor *sensor)
    {
        if (sensor != nullptr)
        {
            lineSensors.push_back(sensor);
            modules.push_back(sensor);
        }
        return *this;
    }

    Robot *RobotBuilder::build()
    {
        // Crear Robot en heap (owner: llamador)
        Robot *robot = new Robot();

        // Transferir ownership: std::move vacía los vectores del builder
        robot->motors = std::move(motors);
        robot->distanceSensors = std::move(distanceSensors);
        robot->lineSensors = std::move(lineSensors);

        // Builder queda en estado válido (vacío) para reutilización
        return robot;
    }

} // namespace robolib