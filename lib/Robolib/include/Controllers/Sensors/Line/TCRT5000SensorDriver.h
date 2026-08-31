#ifndef TCRT5000_SENSOR_DRIVER_H
#define TCRT5000_SENSOR_DRIVER_H

#include "Interfaces/IDriverLineSensor.h"
#include <cstdint>

class TCRT5000SensorDriver : public IDriverLineSensor{
    private:
        uint8_t sensorPin;
        bool isDigital;
        bool is1onLine=false;
        int threshold;


    public:
        TCRT5000SensorDriver(uint8_t pin, bool sensorType);
        ~TCRT5000SensorDriver()=default;
        void begin();
        int getAnalogValue();
        bool isOnLine();
        void set1IsonLine();
        void setThreshold(int value);
};


#endif //TCRT5000_SENSOR_DRIVER_H

