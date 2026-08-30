#ifndef GESTOR_HARDWARE_H
#define GESTOR_HARDWARE_H

#include "interfaces/IDriverMotor.h"
#include "interfaces/IDriverSensorDistancia.h"

class GestorHardware
{
private:
    IDriverMotor *motorDriver_ = nullptr;
    IDriverSensorDistancia *sensorDistanciaDriver_ = nullptr;

public:
    ~GestorHardware()
    {
        if (motorDriver_)
            delete motorDriver_;
        if (sensorDistanciaDriver_)
            delete sensorDistanciaDriver_;
    }

    // --- Asignación y Control de Hardware (MOTORES)---
    void setMotorDriver(IDriverMotor *nuevoDriver)
    {
        if (motorDriver_)
            delete motorDriver_;
        motorDriver_ = nuevoDriver;
        if (motorDriver_)
            motorDriver_->iniciar();
    }

    void moverMotores(int velocidadIzq, int velocidadDer)
    {
        if (motorDriver_)
            motorDriver_->mover(velocidadIzq, velocidadDer);
    }

    void frenarMotores()
    {
        if (motorDriver_)
            motorDriver_->detener();
    }







    // --- Asignación y Contol de Hardware (SENSOR DE DISTANCIA)---
    void setSensorDistancia(IDriverSensorDistancia *nuevoSensorDist)
    {
        if (sensorDistanciaDriver_)
            delete sensorDistanciaDriver_;
        sensorDistanciaDriver_ = nuevoSensorDist;
        if (sensorDistanciaDriver_)
            sensorDistanciaDriver_->iniciar();
    }

    float medirDistanciaCM(){
        if (sensorDistanciaDriver_)
            return sensorDistanciaDriver_->medirCM();
        return -1.0f;
    }
};

#endif