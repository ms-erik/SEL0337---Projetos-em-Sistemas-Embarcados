#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

// Definições dos pinos
#define TRIGGER_PIN GPIO_NUM_4
#define ECHO_PIN GPIO_NUM_5

// Função para inicializar os pinos
void hcsr04_init() {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << TRIGGER_PIN) | (1ULL << ECHO_PIN),
        .mode = GPIO_MODE_OUTPUT, // Trigger como saída
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    io_conf.pin_bit_mask = (1ULL << ECHO_PIN);
    io_conf.mode = GPIO_MODE_INPUT; // Echo como entrada
    gpio_config(&io_conf);

    gpio_set_level(TRIGGER_PIN, 0); // Trigger inicial em LOW
}

// Função para obter a distância
float hcsr04_get_distance() {
    uint64_t start_time, end_time;

    // Envia o pulso de Trigger
    gpio_set_level(TRIGGER_PIN, 1);
    esp_rom_delay_us(10); // Pulso de 10 µs
    gpio_set_level(TRIGGER_PIN, 0);

    // Espera o Echo começar
    while (gpio_get_level(ECHO_PIN) == 0);

    start_time = esp_timer_get_time(); // Tempo de início do pulso Echo

    // Espera o Echo terminar
    while (gpio_get_level(ECHO_PIN) == 1);

    end_time = esp_timer_get_time(); // Tempo de término do pulso Echo

    // Calcula a distância em cm
    float distance = (end_time - start_time) / 58.0; // Fórmula do HC-SR04

    return distance;
}

// Task para leitura contínua
void hcsr04_task(void *pvParameters) {
    while (1) {
        float distance = hcsr04_get_distance();
        printf("Distância: %.2f cm\n", distance);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Leitura a cada 1 segundo
    }
}

void app_main() {
    hcsr04_init(); // Inicializa o sensor

    // Cria a task para leitura do sensor
    xTaskCreate(hcsr04_task, "hcsr04_task", 2048, NULL, 5, NULL);
}
