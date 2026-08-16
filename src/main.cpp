#include <Arduino.h>

// ===== CONFIGURACIÓN DE PINES TB6612FNG =====
// Evitando pines I2C por defecto (GPIO 8=SDA, GPIO 9=SCL)
#define PIN_AIN1    0   // Motor A dirección 1
#define PIN_AIN2    1   // Motor A dirección 2
#define PIN_BIN1    2   // Motor B dirección 1
#define PIN_BIN2    3   // Motor B dirección 2

// ===== CONFIGURACIÓN LEDC (PWM) =====
#define PWM_FREQ_HZ     20000   // 20 kHz (fuera de rango audible)
#define PWM_RESOLUTION  8       // 8 bits = 0-255
#define PWM_MAX_DUTY    255

// Canales LEDC (0-3 para los 4 pines de dirección)
#define CH_AIN1   0
#define CH_AIN2   1
#define CH_BIN1   2
#define CH_BIN2   3

// ===== FUNCIONES =====

void motorSetup() {

  // Configurar 4 canales LEDC para PWM en pines de dirección
  ledcSetup(CH_AIN1, PWM_FREQ_HZ, PWM_RESOLUTION);
  ledcSetup(CH_AIN2, PWM_FREQ_HZ, PWM_RESOLUTION);
  ledcSetup(CH_BIN1, PWM_FREQ_HZ, PWM_RESOLUTION);
  ledcSetup(CH_BIN2, PWM_FREQ_HZ, PWM_RESOLUTION);

  // Adjuntar pines a canales
  ledcAttachPin(PIN_AIN1, CH_AIN1);
  ledcAttachPin(PIN_AIN2, CH_AIN2);
  ledcAttachPin(PIN_BIN1, CH_BIN1);
  ledcAttachPin(PIN_BIN2, CH_BIN2);

  // Inicializar motores detenidos
  ledcWrite(CH_AIN1, 0);
  ledcWrite(CH_AIN2, 0);
  ledcWrite(CH_BIN1, 0);
  ledcWrite(CH_BIN2, 0);
}

/**
 * Controla la velocidad de ambos motores.
 * @param leftSpeed  Velocidad motor izquierdo (-255 a 255). Negativo = reversa.
 * @param rightSpeed Velocidad motor derecho (-255 a 255). Negativo = reversa.
 */
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  // Limitar rango
  leftSpeed  = constrain(leftSpeed, -PWM_MAX_DUTY, PWM_MAX_DUTY);
  rightSpeed = constrain(rightSpeed, -PWM_MAX_DUTY, PWM_MAX_DUTY);

  // Motor Izquierdo (Motor A: AIN1, AIN2)
  if (leftSpeed >= 0) {
    ledcWrite(CH_AIN1, leftSpeed);   // Forward
    ledcWrite(CH_AIN2, 0);
  } else {
    ledcWrite(CH_AIN1, 0);
    ledcWrite(CH_AIN2, -leftSpeed);  // Reverse
  }

  // Motor Derecho (Motor B: BIN1, BIN2)
  if (rightSpeed >= 0) {
    ledcWrite(CH_BIN1, rightSpeed);  // Forward
    ledcWrite(CH_BIN2, 0);
  } else {
    ledcWrite(CH_BIN1, 0);
    ledcWrite(CH_BIN2, -rightSpeed); // Reverse
  }
}

// ===== SETUP / LOOP =====

void setup() {
  Serial.begin(115200);
  motorSetup();
  Serial.println("Control de motores TB6612FNG inicializado");
}

void loop() {
  // Ejemplo de prueba: avanza, gira, retrocede, gira, para
  static uint32_t lastChange = 0;
  static uint8_t state = 0;

  if (millis() - lastChange > 2000) {
    lastChange = millis();
    state = (state + 1) % 5;

    switch (state) {
      case 0: setMotorSpeeds(200, 200);  Serial.println("Adelante"); break;
      case 1: setMotorSpeeds(-100, 100); Serial.println("Giro izq"); break;
      case 2: setMotorSpeeds(-150, -150); Serial.println("Atrás"); break;
      case 3: setMotorSpeeds(100, -100); Serial.println("Giro der"); break;
      case 4: setMotorSpeeds(0, 0);      Serial.println("Parado"); break;
    }
  }
}