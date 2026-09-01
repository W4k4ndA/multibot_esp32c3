#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

/**
 * @interface IDriverLineSensor
 * @brief Interfaz abstracta para controladores de sensores de línea.
 *
 * Esta interfaz define el contrato para las implementaciones de sensores de línea.
 * Los sensores de línea detectan si un robot está sobre una línea (típicamente una línea
 * oscura sobre superficie clara o viceversa) y pueden proporcionar lecturas analógicas
 * para un posicionamiento más preciso.
 */
class IDriverLineSensor
{
private:
  bool is1onLine;

public:
  /**
   * @brief Destructor virtual para limpieza adecuada de clases derivadas.
   */
  virtual ~IDriverLineSensor() = default;

  /**
   * @brief Verifica si el sensor está actualmente sobre una línea.
   * @return true si el sensor detecta una línea, false en caso contrario.
   */
  virtual bool isOnLine() = 0;

  /**
   * @brief Establece el estado interno indicando que un valor de 1 significa que
   * el sensor esta sobre la linea.
   *
   * Usalo si tu sensor regresa 1 (true) cuando detecta negro.
   * Si detecta 0 (false) cuando esta sobre la linea, no lo uses.
   *
   */
  virtual void set1IsonLine() = 0;


  /**
   * @brief Establece el estado interno indicando que un valor de 0 significa que
   * el sensor esta sobre la linea.
   *
   * Usalo si tu sensor regresa 0 (false) cuando detecta negro.
   * Si detecta 1 (true) cuando esta sobre la linea, no lo uses.
   *
   */
  virtual void set0IsonLine() = 0;


  /**
   * @brief Inicializa el hardware del sensor de línea.
   *
   * Este método debe configurar pines, ADC o cualquier otra inicialización
   * específica del hardware requerida para que funcione el sensor de línea.
   */
  virtual void begin() = 0;

  /**
   * @brief Obtiene el valor analógico crudo del sensor de línea.
   *
   * @return La lectura analógica del sensor. Valores más altos típicamente
   *         indican superficies más brillantes (fuera de línea), valores más bajos
   *         indican superficies más oscuras (sobre línea), pero esto depende del sensor específico.
   */
  virtual int getAnalogRawValue() = 0;

  virtual void setThreshold(int value) = 0;
};

#endif