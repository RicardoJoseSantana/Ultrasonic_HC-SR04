// src/main.cpp
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "Ultrasonic.h"

// --- DEFINE AQUÍ TUS PINES ---
#define TRIGGER_PIN GPIO_NUM_5
#define ECHO_PIN    GPIO_NUM_6

// Crear un objeto para nuestro sensor
UltrasonicSensor sensor1(TRIGGER_PIN, ECHO_PIN);

extern "C" void app_main(void)
{
    printf("Inicializando sensor ultrasónico...\n");
    
    while (1) {
        // Medir la distancia usando nuestra clase
        float distance = sensor1.measure_distance();
        
        if (distance >= 0) {
            printf("Distancia: %.2f cm\n", distance);
        } else {
            printf("Error al leer el sensor.\n");
        }
        
        // Esperar 1 segundo antes de la siguiente medición
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}