#include "esp_log.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "ble.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include <string.h>

static const char *TAG = "BLE_Module";
uint16_t characteristic_handle = 0;

#define SERVICE_UUID "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-210987654321"

void ble_init() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    ESP_LOGI(TAG, "BLE inicializado!");
}

void ble_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
        case ESP_GATTS_ADD_CHAR_EVT:
            if (param->add_char.status == ESP_GATT_OK) {
                characteristic_handle = param->add_char.attr_handle;
                ESP_LOGI(TAG, "Característica registrada com handle: %d", characteristic_handle);
            }
            break;
        default:
            break;
    }
}

void ble_start_advertising() {
    esp_ble_adv_params_t adv_params = {
        .adv_int_min        = 0x20,
        .adv_int_max        = 0x40,
        .adv_type           = ADV_TYPE_IND,
        .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
        .channel_map        = ADV_CHNL_ALL,
        .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
    };
    esp_ble_gap_start_advertising(&adv_params);
}

void update_distance(float distance){
    uint8_t distance_value[4];
    memcpy(distance_value,&distance, sizeof(float));
    esp_ble_gatts_set_attr_value(characteristic_handle , sizeof(distance_value), distance_value);
}

/*
void app_main(){
    ble_init();
    ble_start_advertising();
    esp_ble_gatts_register_callback(ble_gatts_event_handler);
}
 */
