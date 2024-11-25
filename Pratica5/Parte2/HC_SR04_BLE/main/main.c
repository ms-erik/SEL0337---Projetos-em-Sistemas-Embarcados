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
    init_distance_sensor();

    while (1) {
        float distance = measure_distance();
        ESP_LOGI(TAG, "Distância: %.2f cm", distance);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main() {
    ble_init();
    ble_start_advertising();
    esp_ble_gatts_register_callback(ble_gatts_event_handler);
    xTaskCreate(distance_task, "Distance Task", 2048, NULL, 5, NULL);
}
