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
        // Trigger como saída
        .mode = GPIO_MODE_OUTPUT, 
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    io_conf.pin_bit_mask = (1ULL << ECHO_PIN);
    // Echo como entrada
    io_conf.mode = GPIO_MODE_INPUT;
    gpio_config(&io_conf);
    // Trigger inicial em LOW
    gpio_set_level(TRIGGER_PIN, 0);
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
    // Tempo de início do pulso Echo
    start_time = esp_timer_get_time();

    // Espera o Echo terminar
    while (gpio_get_level(ECHO_PIN) == 1);
    // Tempo de término do pulso Echo
    end_time = esp_timer_get_time(); 

    // Calcula a distância em cm
    //// Fórmula do HC-SR04
    float distance = (end_time - start_time) / 58.0; 

    return distance;
}

// Task para leitura contínua
void hcsr04_task(void *pvParameters) {
    while (1) {
        float distance = hcsr04_get_distance();
        printf("Distância: %.2f cm\n", distance);
        // Leitura a cada 1 segundo
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}

void app_main() {
    // Inicializa o sensor
    hcsr04_init(); 

    // Cria a task para leitura do sensor
    xTaskCreate(hcsr04_task, "hcsr04_task", 2048, NULL, 5, NULL);
}
