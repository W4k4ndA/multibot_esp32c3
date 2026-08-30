#include "Controllers\Sensors\Distance\HCSR04SensorDriver.h"
#include <Arduino.h>

HCSR04SensorDriver::HCSR04SensorDriver(uint8_t triggerPin, uint8_t echoPin)
    : trigPin(triggerPin), echoPin(echoPin) {}



void HCSR04SensorDriver::init() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

float HCSR04SensorDriver::measureCM() {
    // Send a 10 microsecond pulse to trigger the sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the duration of the echo pulse
    float duration = pulseIn(echoPin, HIGH);

    // Calculate the distance in centimeters
    float distance = duration * 0.034 / 2;

    // Ensure the distance is within a reasonable range (e.g., 2cm to 400cm)
    if (distance < 2.0f) {
        distance = 2.0f;
    } else if (distance > 400.0f) {
        distance = 400.0f;
    }
    return distance;
}