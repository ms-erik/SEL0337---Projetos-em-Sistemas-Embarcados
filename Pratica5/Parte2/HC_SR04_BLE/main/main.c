#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensors/distance.h" // Biblioteca para o sensor de distância
// #include "sensors/temperature.h" // Exemplo: biblioteca para sensor de temperatura
// #include "sensors/humidity.h" // Exemplo: biblioteca para sensor de umidade
#include "sensor_config.h"
#include "ble.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "nvs_flash.h"

// TAG para logs
static const char *TAG = "Main_Task";

// Função para realizar medições
float measure_sensor(sensor_type_t sensor_type) {
    switch (sensor_type) {
        case SENSOR_DISTANCE:
            return measure_distance();
        case SENSOR_TEMPERATURE:
            // return measure_temperature(); // Exemplo
            break;
        case SENSOR_HUMIDITY:
            // return measure_humidity(); // Exemplo
            break;
        default:
            ESP_LOGE(TAG, "Tipo de sensor desconhecido para medição!");
            return -1;
    }
    return -1; // Retorno padrão para caso não implementado
}

// Função genérica para inicializar o sensor
void init_sensor(sensor_type_t sensor_type) {
    switch (sensor_type) {
        case SENSOR_DISTANCE:
            ESP_LOGI(TAG, "Inicializando sensor de distância...");
            init_distance_sensor();
            break;
        case SENSOR_TEMPERATURE:
            ESP_LOGI(TAG, "Inicializando sensor de temperatura...");
            // init_temperature_sensor(); // Exemplo
            break;
        case SENSOR_HUMIDITY:
            ESP_LOGI(TAG, "Inicializando sensor de umidade...");
            // init_humidity_sensor(); // Exemplo
            break;
        default:
            ESP_LOGE(TAG, "Tipo de sensor desconhecido!");
            break;
    }
}

// Task genérica para um sensor
void sensor_task(void *pvParameters) {
    sensor_type_t sensor_type = *(sensor_type_t *)pvParameters;

    // Inicializar o sensor específico
    init_sensor(sensor_type);

    while (1) {
        float measurement = measure_sensor(sensor_type);

        switch (sensor_type) {
            case SENSOR_DISTANCE:
                ESP_LOGI(TAG, "Distância: %.2f cm", measurement);
                if (measurement > 50) {
                    update_measured_value(measurement);
                    update_sensor_data(get_sensor_data());
                }
                break;

            case SENSOR_TEMPERATURE:
                ESP_LOGI(TAG, "Temperatura: %.2f °C", measurement);
                // update_temperature(measurement); // Exemplo
                break;

            case SENSOR_HUMIDITY:
                ESP_LOGI(TAG, "Umidade: %.2f %%", measurement);
                // update_humidity(measurement); // Exemplo
                break;

            default:
                ESP_LOGE(TAG, "Tipo de sensor desconhecido na task!");
                break;
        }

        // Aguarda 10 segundos
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void app_main() {

    sensor_data_t sensor = get_sensor_data();

    // Criar task baseada no tipo de sensor
    switch (sensor.sensor_type) {
        case SENSOR_DISTANCE:
        case SENSOR_TEMPERATURE:
        case SENSOR_HUMIDITY:
            xTaskCreate(sensor_task, "Sensor Task", 2048, &sensor.sensor_type, 5, NULL);
            break;
        default:
            ESP_LOGE(TAG, "Tipo de sensor não suportado!");
            break;
    }
    // Inicializa BLE
    ble_init();
    ble_start_advertising();
}
