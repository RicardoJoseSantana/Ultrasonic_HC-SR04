// src/UltrasonicSensor.h
#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include "driver/gpio.h"

class UltrasonicSensor {
public:
    // Constructor: inicializa el sensor con los pines Trig y Echo
    UltrasonicSensor(gpio_num_t trig_pin, gpio_num_t echo_pin);
    
    // Destructor: libera los recursos
    ~UltrasonicSensor();

    // Mide la distancia en centímetros. Aplica filtrado.
    float measure_distance();

private:
    gpio_num_t trig_pin;
    gpio_num_t echo_pin;
    
    // Función interna para realizar una única medición sin filtrar
    float single_measurement();
};

#endif // ULTRASONIC_SENSOR_H