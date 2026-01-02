// src/main.cpp

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "ultrasonic.h" // Asegúrate de que este es el nombre de tu archivo .h

// --- DEFINE AQUÍ TUS PINES ---
#define NUM_SENSORS 4
#define TRIGGER_PIN_COMMON  GPIO_NUM_2 // Pin de Trigger único para todos

// Un pin de Echo para cada sensor
#define ECHO_PIN_SENSOR_1   GPIO_NUM_4
#define ECHO_PIN_SENSOR_2   GPIO_NUM_5
#define ECHO_PIN_SENSOR_3   GPIO_NUM_6
#define ECHO_PIN_SENSOR_4   GPIO_NUM_7

// Creamos un array para almacenar los objetos de nuestros sensores
UltrasonicSensor* sensors[NUM_SENSORS];

extern "C" void app_main(void)
{
    printf("Inicializando %d sensores ultrasónicos con Trigger compartido...\n", NUM_SENSORS);

    // Array con los pines de Echo para facilitar la inicialización
    gpio_num_t echo_pins[NUM_SENSORS] = {
        ECHO_PIN_SENSOR_1,
        ECHO_PIN_SENSOR_2,
        ECHO_PIN_SENSOR_3,
        ECHO_PIN_SENSOR_4
    };

    // --- Bucle de Inicialización ---
    // Creamos dinámicamente cada objeto sensor y lo guardamos en el array
    for (int i = 0; i < NUM_SENSORS; i++) {
        sensors[i] = new UltrasonicSensor(TRIGGER_PIN_COMMON, echo_pins[i]);
        printf("Sensor %d inicializado en Trigger=%d, Echo=%d\n", i + 1, TRIGGER_PIN_COMMON, echo_pins[i]);
    }
    
    printf("\n--- Comenzando mediciones ---\n");

    while (1) {
        // --- Bucle de Medición ---
        // Recorremos cada sensor para medir la distancia
        for (int i = 0; i < NUM_SENSORS; i++) {
            
            float distance = sensors[i]->measure_distance();
            
            if (distance >= 0) {
                printf("Sensor %d: Distancia = %.2f cm\n", i + 1, distance);
            } else {
                printf("Sensor %d: Error al leer.\n", i + 1);
            }
            
            // Esperar un poco entre la medición de cada sensor para evitar que el
            // "ping" de uno sea captado por el "echo" de otro.
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        
        printf("--------------------------------\n");

        // Esperar un segundo antes de repetir el ciclo completo de mediciones
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}