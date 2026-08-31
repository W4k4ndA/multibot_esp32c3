#include "TCRT5000SensorDriver.h"
#include <Arduino.h>

namespace robolib
{
    TCRT5000SensorDriver::TCRT5000SensorDriver(uint8_t pin, bool sensorType) : sensorPin(pin), isDigital(sensorType) {}

    void TCRT5000SensorDriver::begin()
    {
        pinMode(sensorPin, INPUT);
    }

    void TCRT5000SensorDriver::set1IsonLine()
    {
        is1onLine = true;
    }

    int TCRT5000SensorDriver::getAnalogValue()
    {
        return analogRead(sensorPin);
    }

    bool TCRT5000SensorDriver::isOnLine()
    {
        if (isDigital)
        {
            if (is1onLine == digitalRead(sensorPin))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        else
        {
            return analogRead(sensorPin) >= threshold; // Umbral para sensor analógico
        }
    }

    void TCRT5000SensorDriver::setThreshold(int value)
    {
        threshold = value;
    }
}