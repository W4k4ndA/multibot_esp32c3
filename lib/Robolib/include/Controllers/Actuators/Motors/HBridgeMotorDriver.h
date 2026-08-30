#ifndef HBRIDGE_DRIVER_H
#define HBRIDGE_DRIVER_H

#include "Interfaces\IDriverMotor.h"

class HBridgeMotorDriver : public IDriverMotor
{
private:
    uint8_t IN1;
    uint8_t IN2;
    uint8_t CH1;
    uint8_t CH2;

public:
    HBridgeMotorDriver(uint8_t pinIN1, uint8_t pinIN2, uint8_t ch1, uint8_t ch2);

    ~HBridgeMotorDriver() override = default;

    void begin() override;

    void move(int16_t speed) override;

    void stop(bool stacked = false) override;
};

#endif