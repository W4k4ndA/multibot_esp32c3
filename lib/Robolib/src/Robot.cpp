#include "Robot.h"

namespace robolib {

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

Robot::~Robot()
{
    // Liberar drivers de motores (owner)
    for (auto* motor : motors) {
        delete motor;
    }
    motors.clear();

    // Liberar drivers de sensores de distancia (owner)
    for (auto* sensor : distanceSensors) {
        delete sensor;
    }
    distanceSensors.clear();
}

size_t Robot::getMotorCount() const
{
    return motors.size();
}

size_t Robot::getDistanceSensorCount() const
{
    return distanceSensors.size();
}

bool Robot::hasMotors() const
{
    return !motors.empty();
}

bool Robot::hasDistanceSensors() const
{
    return !distanceSensors.empty();
}

void Robot::moveMotor(size_t motorIndex, int16_t speed)
{
    // Validación defensiva: índice en rango y puntero no nulo
    if (motorIndex < motors.size() && motors[motorIndex] != nullptr) {
        motors[motorIndex]->move(speed);
    }
    // Silencio si inválido: evita excepciones en sistemas embebidos
}

void Robot::stopMotor(size_t motorIndex, bool stacked)
{
    if (motorIndex < motors.size() && motors[motorIndex] != nullptr) {
        motors[motorIndex]->stop(stacked);
    }
}

void Robot::stopAllMotors(bool stacked)
{
    for (auto* motor : motors) {
        if (motor != nullptr) {
            motor->stop(stacked);
        }
    }
}

void Robot::moveDifferential(int16_t leftMotorSpeed, int16_t rightMotorSpeed)
{
    // Requiere al menos 2 motores: [0] = izquierdo, [1] = derecho
    if (motors.size() >= 2) {
        if (motors[0] != nullptr) {
            motors[0]->move(leftMotorSpeed);
        }
        if (motors[1] != nullptr) {
            motors[1]->move(rightMotorSpeed);
        }
    }
}

float Robot::getDistance(size_t sensorIndex)
{
    if (sensorIndex < distanceSensors.size() && distanceSensors[sensorIndex] != nullptr) {
        return distanceSensors[sensorIndex]->measureCM();
    }
    return -1.0f;  // Error: índice inválido, sensor nulo, o error de medición
}

// ============================================================================
// RobotBuilder
// ============================================================================

RobotBuilder& RobotBuilder::addMotor(IDriverMotor* motor)
{
    if (motor != nullptr) {
        motors.push_back(motor);
    }
    // Silencio si nullptr: permite encadenamiento condicional
    return *this;
}

RobotBuilder& RobotBuilder::addDistanceSensor(IDriverDistanceSensor* sensor)
{
    if (sensor != nullptr) {
        distanceSensors.push_back(sensor);
    }
    return *this;
}

Robot* RobotBuilder::build()
{
    // Crear Robot en heap (owner: llamador)
    Robot* robot = new Robot();

    // Transferir ownership: std::move vacía los vectores del builder
    robot->motors = std::move(motors);
    robot->distanceSensors = std::move(distanceSensors);

    // Builder queda en estado válido (vacío) para reutilización
    return robot;
}

} // namespace robolib