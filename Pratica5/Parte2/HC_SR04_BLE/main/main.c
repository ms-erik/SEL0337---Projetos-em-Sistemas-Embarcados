#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "distance.h"
#include "ble.h"
#include "esp_log.h"

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
    xTaskCreate(distance_task, "Distance Task", 2048, NULL, 5, NULL);
}
