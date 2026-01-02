// src/ultrasonic.cpp
#include "ultrasonic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "rom/ets_sys.h"
#include <algorithm> 

#define FILTER_SAMPLES 5

UltrasonicSensor::UltrasonicSensor(gpio_num_t trig, gpio_num_t echo) 
    : trig_pin(trig), echo_pin(echo) {
    gpio_set_direction(trig_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(trig_pin, 0);
    gpio_set_direction(echo_pin, GPIO_MODE_INPUT);
}

UltrasonicSensor::~UltrasonicSensor() {
}

float UltrasonicSensor::single_measurement() {
    // --- Enviar el pulso de Trigger ---
    gpio_set_level(trig_pin, 0);
    esp_rom_delay_us(2); // <-- CORRECCIÓN APLICADA
    gpio_set_level(trig_pin, 1);
    esp_rom_delay_us(10); // <-- CORRECCIÓN APLICADA
    gpio_set_level(trig_pin, 0);

    // --- Esperar a que el pin de Echo se ponga en ALTO ---
    int64_t start_time = esp_timer_get_time();
    while (gpio_get_level(echo_pin) == 0) {
        if ((esp_timer_get_time() - start_time) > 500000) { 
            return -1.0; 
        }
    }

    // --- Medir la duración del pulso de Echo ---
    start_time = esp_timer_get_time();
    while (gpio_get_level(echo_pin) == 1) {
        if ((esp_timer_get_time() - start_time) > 500000) {
            return -1.0; 
        }
    }
    int64_t pulse_duration = esp_timer_get_time() - start_time;

    float distance = (pulse_duration * 0.0343) / 2.0;
    
    return distance;
}

float UltrasonicSensor::measure_distance() {
    float samples[FILTER_SAMPLES];
    
    for (int i = 0; i < FILTER_SAMPLES; ++i) {
        samples[i] = single_measurement();
        vTaskDelay(pdMS_TO_TICKS(10)); 
        if (samples[i] < 0) {
           i--; 
        }
    }

    std::sort(samples, samples + FILTER_SAMPLES);

    float sum = 0;
    for (int i = 1; i < FILTER_SAMPLES - 1; ++i) {
        sum += samples[i];
    }
    
    return sum / (FILTER_SAMPLES - 2);
}