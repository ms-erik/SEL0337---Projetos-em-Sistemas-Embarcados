#include "esp_log.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "ble.h"
#include "nvs_flash.h"
#include <string.h>

static const char *TAG = "BLE_Module";
uint16_t characteristic_handle = 0;
esp_gatt_if_t gatts_if = 0;    // Variável global para gatts_if
uint16_t conn_id = 0;          // Variável global para conn_id
uint16_t service_handle = 0;

#define SERVICE_UUID "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-210987654321"

void ble_start_advertising();

// Configuração dos dados de advertising
static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = false,
    .min_interval = 0x20,
    .max_interval = 0x40,
    .appearance = 0x00,
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 16,
    .p_service_uuid = (uint8_t *)SERVICE_UUID,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

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

    ESP_ERROR_CHECK(esp_ble_gatts_register_callback(ble_gatts_event_handler));
    ESP_ERROR_CHECK(esp_ble_gap_register_callback(ble_gap_event_handler));
    ESP_ERROR_CHECK(esp_ble_gatts_app_register(0));
}

void ble_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if_param, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
        case ESP_GATTS_REG_EVT: {
            ESP_LOGI(TAG, "GATT registrado. Criando serviço...");
            gatts_if = gatts_if_param;

            // Configurando o serviço
            esp_gatt_srvc_id_t service_id = {
                .is_primary = true,
                .id = {
                    .uuid = {
                        .len = ESP_UUID_LEN_128,
                        .uuid.uuid128 = SERVICE_UUID,
                    },
                    .inst_id = 0,
                },
            };

            ESP_ERROR_CHECK(esp_ble_gatts_create_service(gatts_if, &service_id, 4));
            break;
        }
        case ESP_GATTS_CREATE_EVT: {
            ESP_LOGI(TAG, "Serviço criado com handle: %d", param->create.service_handle);
            service_handle = param->create.service_handle;

            // Adiciona uma característica ao serviço
            esp_gatt_char_prop_t char_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
            esp_attr_value_t char_val = {
                .attr_max_len = 20,
                .attr_len = strlen("Hello"),
                .attr_value = (uint8_t *)"Hello",
            };

            ESP_ERROR_CHECK(esp_ble_gatts_add_char(
                service_handle,                                // Handle do serviço
                &((esp_bt_uuid_t){
                    .len = ESP_UUID_LEN_128,
                    .uuid.uuid128 = CHARACTERISTIC_UUID,
                }),
                ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,     // Permissões de leitura e escrita
                char_property,                                // Propriedades da característica
                &char_val,                                    // Valor inicial
                NULL                                          // Controle adicional (NULL)
            ));
            break;
        }
        case ESP_GATTS_ADD_CHAR_EVT: {
            ESP_LOGI(TAG, "Característica registrada com handle: %d", param->add_char.attr_handle);
            characteristic_handle = param->add_char.attr_handle;

            // Inicia o serviço
            ESP_ERROR_CHECK(esp_ble_gatts_start_service(service_handle));

            // Configura dados de advertising
            ESP_ERROR_CHECK(esp_ble_gap_config_adv_data(&adv_data));
            break;
        }
        case ESP_GATTS_CONNECT_EVT: {
            ESP_LOGI(TAG, "Dispositivo central conectado");
            conn_id = param->connect.conn_id;
            break;
        }
        case ESP_GATTS_DISCONNECT_EVT: {
            ESP_LOGI(TAG, "Dispositivo central desconectado. Reiniciando advertising.");
            ble_start_advertising();
            break;
        }
        default:
            break;
    }
}

void ble_gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    if (event == ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT) {
        ble_start_advertising();
    }
}

void ble_start_advertising() {
    esp_ble_adv_params_t adv_params = {
        .adv_int_min = 0x20,
        .adv_int_max = 0x40,
        .adv_type = ADV_TYPE_IND,
        .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
        .channel_map = ADV_CHNL_ALL,
        .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
    };

    ESP_ERROR_CHECK(esp_ble_gap_start_advertising(&adv_params));
    ESP_LOGI(TAG, "Advertising inicializado!");
}

void update_distance(float distance){
    uint8_t distance_value[4];
    memcpy(distance_value, &distance, sizeof(float));
    
    // Atualiza o valor da característica
    esp_ble_gatts_set_attr_value(characteristic_handle, sizeof(distance_value), distance_value);
    
    // Envia a notificação/indicação para o dispositivo central
    if (gatts_if != 0 && conn_id != 0) {
        esp_ble_gatts_send_indicate(gatts_if, conn_id, characteristic_handle,
                                    sizeof(distance_value), distance_value, false);
        ESP_LOGI(TAG, "Distância atualizada e notificada: %.2f", distance);
    } else {
        ESP_LOGW(TAG, "Não há cliente conectado. Notificação não enviada.");
    }
}
