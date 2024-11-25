#ifndef BLE_H
#define BLE_H

void ble_init(void);
void ble_publish(bool send, int device_id, sensor_type_t sensor_type);

#endif // BLE_H
