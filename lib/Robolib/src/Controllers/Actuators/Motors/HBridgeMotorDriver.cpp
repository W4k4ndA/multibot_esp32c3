
#include "Controllers\Actuators\Motors\HBridgeMotorDriver.h"
#include <Arduino.h>


#define PWM_FREQ 25000   // Frecuencia PWM en Hz
#define PWM_RESOLUTION 8 // Resolución PWM (8 bits)

class HBridgeMotorDriver : public IDriverMotor
{
private:
    uint8_t IN1;
    uint8_t IN2;
    uint8_t CH1;
    uint8_t CH2;

public:
    HBridgeMotorDriver(uint8_t pinIN1, uint8_t pinIN2, uint8_t ch1, uint8_t ch2)
        : IN1(pinIN1), IN2(pinIN2), CH1(ch1), CH2(ch2) {}

    void HBridgeMotorDriver::~IDriverMotor(){};

    void HBridgeMotorDriver::begin() override
    {
        pinMode(IN1, OUTPUT);
        pinMode(IN2, OUTPUT);

        ledcSetup(CH1, PWM_FREQ, PWM_RESOLUTION);
        ledcSetup(CH2, PWM_FREQ, PWM_RESOLUTION);

        ledcAttachPin(IN1, CH1);
        ledcAttachPin(IN2, CH2);

        stop();
    }

    void HBridgeMotorDriver::move(int16_t speed) override
    {
        speed = constrain(speed, -255, 255);
        if (speed > 0)
        {
            ledcWrite(CH1, speed);
            ledcWrite(CH2, 0);
        }
        else if (speed < 0)
        {
            ledcWrite(CH1, 0);
            ledcWrite(CH2, speed);
        }
        else if (speed == 0)
            stop();
    }

    void HBridgeMotorDriver::stop(bool stacked = false) override
    {
        if (stacked)
        {
            ledcWrite(CH1, 255);
            ledcWrite(CH2, 255);
        }
        else
        {
            ledcWrite(CH1, 0);
            ledcWrite(CH2, 0);
        }
    }
};
