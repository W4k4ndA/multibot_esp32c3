#ifndef SENSOR_DISTANCIA_H
#define SENSOR_DISTANCIA_H

/**
 * @brief Interfaz abstracta para sensores de distancia.
 * 
 * Define el contrato que deben implementar todos los sensores de distancia
 * compatibles con el sistema (ultrasonido, láser, infrarrojo, etc.).
 */
class IDriverSensorDistancia {
public:
    /**
     * @brief Destructor virtual por defecto.
     * 
     * Necesario para destrucción correcta de objetos derivados mediante
     * puntero a la interfaz.
     */
    virtual ~IDriverSensorDistancia() = default;

    /**
     * @brief Inicializa el sensor de distancia.
     * 
     * Configura el hardware del sensor (pines, registros, calibración inicial).
     * Debe llamarse antes de cualquier medición.
     */
    virtual void iniciar() = 0;

    /**
     * @brief Realiza una medición de distancia.
     * 
     * @return Distancia medida en centímetros.
     *         Retorna valor negativo en caso de error o fuera de rango.
     */
    virtual float medirCM() = 0;
};

#endif