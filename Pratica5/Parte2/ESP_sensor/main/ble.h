#ifndef BLE_H
#define BLE_H

#include "ble.h"
#include "sensor_config.h"

void ble_init();
void ble_start_advertising();
void update_sensor_data(sensor_data_t data);

#endif // BLE_H
