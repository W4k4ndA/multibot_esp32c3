#include "Robot.h"

Robot::~Robot()
{
    for (auto m : motors)
    {
        delete m;
    }

    for (auto s : distanceSensors)
    {
        delete s;
    }
    motors.clear();
    distanceSensors.clear();
}

//--- CONSULTAS ---
size_t Robot::getDistanceSensorCount() const { return distanceSensors.size(); }
size_t Robot::getMotorCount() const { return motors.size(); }
bool Robot::hasDistanceSensors() const { return distanceSensors.size() > 0; }
bool Robot::hasMotors() const { return !motors.empty(); }

//--- CONTROL DE MOTORES ---
void Robot::moveMotor(size_t motorIndex, int16_t speed)
{
    if (motorIndex >= motors.size() && motors[motorIndex] == nullptr)
    {
        return;
    }
    motors[motorIndex]->move(speed);
}

void Robot::stopMotor(size_t motorIndex, bool stacked = false)
{
    if (motorIndex >= motors.size())
    {
        return;
    }
    motors[motorIndex]->stop(stacked);
}

void Robot::stopAllMotors(bool stacked = false)
{
    for (auto m : motors)
    {
        if (m)
            m->stop(stacked);
    }
}

void Robot::moveDifferential(int16_t leftMotorSpeed, int16_t rigthMotorSpeed)
{
    if (motors.size() < 2)
        return;
    if (motors[0])
        motors[0]->move(leftMotorSpeed);
    if (motors[1])
        motors[1]->move(rigthMotorSpeed);
}

//--- SENSOR DE DISTANCIA ---
float Robot::getDistanceCM(size_t sensorIndex)
{
    if (sensorIndex >= distanceSensors.size())
    {
        return 0.0f;
    }

    return distanceSensors[sensorIndex]->measureCM();
}

// ==========================================
// --- IMPLEMENTACIÓN DE ROBOTBUILDER ---
// ==========================================

RobotBuilder &RobotBuilder::addMotor(IDriverMotor *motor)
{
    if (motor)
        tempMotors.push_back(motor);
    return *this;
}

RobotBuilder &RobotBuilder::addDistanceSensor(IDriverDistanceSensor *sensor)
{
    if (sensor)
        tempDistanceSensors.push_back(sensor);
    return *this;
}

Robot *RobotBuilder::build()
{
    Robot *robot = new Robot();

    for (auto m : tempMotors)
    {
        m->begin();
        robot->motors.push_back(m);
    }

    for (auto s : tempDistanceSensors)
    {
        s->begin();
        robot->distanceSensors.push_back(s);
    }
    
    return robot;
}