#include "Interfaces\IDriverDistanceSensor.h"
#include <cstdint>

class HCSR04SensorDriver : public IDriverDistanceSensor
{
private:
    int trigPin;
    int echoPin;

public:
    HCSR04SensorDriver(uint8_t trigPin, uint8_t echoPin);
    ~HCSR04SensorDriver();

    void init();
    float measureCM();
};