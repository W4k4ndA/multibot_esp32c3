#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include "Interfaces/IDriverMotor.h"
#include "Interfaces/IDriverDistanceSensor.h"

class RobotBuilder;

class Robot
{
private:
    std::vector<IDriverMotor *> motors;
    std::vector<IDriverDistanceSensor *> distanceSensors;

    // Aqui se agregan los nuevos componentes (sensores y actuadores)

    Robot() = default;
    friend class RobotBuilder;

public:
    ~Robot();

    //--- Consultas ---

    size_t getMotorCount() const;
    size_t getDistanceSensorCount() const;

    bool hasMotors() const;
    bool hasDistanceSensors() const;

    // Control de Motores

    void moveMotor(size_t motorIndex, int16_t speed);
    void stopMotor(size_t motorIndex, bool stacked = false);
    void stopAllMotors(bool stacked = false);

    void moveDifferential(int16_t leftMotorSpeed, int16_t rigthMotorSpeed);

    // Control de Sensores de Distancia
    float getDistanceCM(size_t sensorIndex);
};

//--- BUILDER para el Robot

class RobotBuilder
{
private:
    std::vector<IDriverMotor *> tempMotors;
    std::vector<IDriverDistanceSensor *> tempDistanceSensors;

public:
    RobotBuilder() = default;

    RobotBuilder &addMotor(IDriverMotor *motor);
    RobotBuilder &addDistanceSensor(IDriverDistanceSensor *sensor);

    Robot *build();
};

#endif // ROBOT_H