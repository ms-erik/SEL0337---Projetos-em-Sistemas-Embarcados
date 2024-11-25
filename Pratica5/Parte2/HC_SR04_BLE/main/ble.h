#ifndef BLE_H
#define BLE_H

#include "esp_log.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "ble.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_bt_main.h"

void ble_init();
void ble_start_advertising();
void update_distance(float distance);

#endif // BLE_H
