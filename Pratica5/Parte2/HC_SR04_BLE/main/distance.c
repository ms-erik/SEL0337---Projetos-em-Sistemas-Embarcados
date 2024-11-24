#include <stdio.h>
#include "driver/gpio.h"
#include "esp_timer.h"
#include "distance.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Configuração dos pinos
#define TRIG_PIN GPIO_NUM_5
#define ECHO_PIN GPIO_NUM_18
#define SOUND_SPEED 0.034 // Velocidade do som (cm/us)

void init_distance_sensor() {
    gpio_set_direction(TRIG_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);
}

float measure_distance() {
    gpio_set_level(TRIG_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(2));
    gpio_set_level(TRIG_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(TRIG_PIN, 0);

    int64_t start_time = 0, end_time = 0;

    while (gpio_get_level(ECHO_PIN) == 0) {
        start_time = esp_timer_get_time();
    }
    while (gpio_get_level(ECHO_PIN) == 1) {
        end_time = esp_timer_get_time();
    }

    float duration = (float)(end_time - start_time);
    return (duration * SOUND_SPEED) / 2.0;
}
