#include "esp_log.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "nvs_flash.h"
#include <string.h>

// Definições de UUID
#define SERVICE_UUID        "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-210987654321"

static const char *TAG = "BLE_Module";

// Handles para o serviço e a característica
uint16_t characteristic_handle = 0;
esp_gatt_if_t gatts_if = 0;    // Variável global para gatts_if
uint16_t conn_id = 0;          // Variável global para conn_id
uint16_t service_handle = 0;

// Estrutura para dados do sensor
typedef struct {
    int sensor_id;
    int sensor_type;
    float value;
    char description[64];
} sensor_data_t;

// Funções de evento e inicialização
void ble_start_advertising();
void ble_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if_param, esp_ble_gatts_cb_param_t *param);
void ble_gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
void ble_init();
void update_sensor_data(sensor_data_t data);

// Função para inicializar o advertising
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

// Função para inicializar a comunicação BLE
void ble_init() {
    // Inicializaçãa da partição NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    // Configuração do controlador bluetooth
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());
    ESP_ERROR_CHECK(esp_ble_gap_set_device_name("Sensor de distancia"));

    ESP_LOGI(TAG, "BLE inicializado!");

    ESP_ERROR_CHECK(esp_ble_gatts_register_callback(ble_gatts_event_handler));
    ESP_ERROR_CHECK(esp_ble_gap_register_callback(ble_gap_event_handler));
    ESP_ERROR_CHECK(esp_ble_gatts_app_register(0));
}

void ble_gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    if (event == ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT) {
        ble_start_advertising();
    }
}

// Callback para eventos GATTS
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

            // Cria o serviço
            ESP_ERROR_CHECK(esp_ble_gatts_create_service(gatts_if, &service_id, 1));  // Somente uma característica
            break;
        }

        case ESP_GATTS_CREATE_EVT: {
            ESP_LOGI(TAG, "Serviço criado com handle: %d", param->create.service_handle);
            service_handle = param->create.service_handle;

            // Adiciona uma característica simples ao serviço
            ESP_ERROR_CHECK(esp_ble_gatts_add_char(
                service_handle,                                // Handle do serviço
                &(esp_bt_uuid_t){
                    .len = ESP_UUID_LEN_16,
                    .uuid = { .uuid16 = CHARACTERISTIC_UUID },
                },
                ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,     // Permissões de leitura e escrita
                ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY,  // Propriedades: leitura, escrita e notificação
                &(esp_attr_value_t){
                    .attr_max_len = 256,                      // Tamanho máximo do valor da característica
                    .attr_len = 0,                             // Inicialmente vazio
                    .attr_value = NULL,                        // Valor inicial da característica
                },
                NULL                                           // Controle adicional (NULL)
            ));
            break;
        }

        case ESP_GATTS_ADD_CHAR_EVT: {
            ESP_LOGI(TAG, "Característica registrada com handle: %d", param->add_char.attr_handle);
            characteristic_handle = param->add_char.attr_handle;
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

// Função para atualizar os dados do sensor
void update_sensor_data(sensor_data_t data) {
    // Cria um buffer para enviar os dados
    uint8_t message_buffer[sizeof(sensor_data_t)];

    // Copia os dados do sensor para o buffer
    memcpy(message_buffer, &data, sizeof(sensor_data_t));

    // Atualiza o valor da característica com os novos dados (valor do sensor + descrição)
    esp_ble_gatts_set_attr_value(characteristic_handle, sizeof(message_buffer), message_buffer);

    // Envia a notificação/indicação para o dispositivo central
    if (gatts_if != 0 || conn_id != 0) {
        esp_ble_gatts_send_indicate(gatts_if, conn_id, characteristic_handle,
                                    sizeof(message_buffer), message_buffer, false);
        ESP_LOGI(TAG, "Sensor ID %d do tipo %d atualizado e notificado: Valor: %f, Descrição: %s", 
                 data.sensor_id, data.sensor_type, data.value, data.description);
    } else {
        ESP_LOGW(TAG, "Não há cliente conectado. Notificação não enviada.");
    }
}
