#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "distance.h"
#include "ble.h"
#include "esp_log.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "ble.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_bt_main.h"

static const char *TAG = "Main_Task";

void distance_task(void *pvParameters) {
    // Rotina para ver as distâncias medidas pelo sensor
    init_distance_sensor();

    while (1) {
        float distance = measure_distance();
        ESP_LOGI(TAG, "Distância: %.2f cm", distance);
        if (distance > 50){
            // Função para notificar o dispositivo central
            update_distance(distance);
        }
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void app_main() {
    //
    ble_init();
    ble_start_advertising();
    xTaskCreate(distance_task, "Distance Task", 2048, NULL, 5, NULL);
}
