#include "sensor_config.h"
#include "esp_log.h"      // Para a função de log ESP_LOGI
#include <string.h>       // Para funções como memcpy, memset, strlen
#include <stdint.h>       // Para o tipo uint8_t

// Definição de uma variável estática global para armazenar os dados do sensor
static sensor_data_t sensor_data;

// TAG para logs
static const char *TAG = "Sensor_config";


// Função para obter os dados do sensor
sensor_data_t get_sensor_data(void) {
    // Verifica se os dados do sensor já foram preenchidos
    if (sensor_data.description == NULL) {
        // Preenche os dados do sensor se ainda não estiverem preenchidos

        // Atualiza os valores dos dados do sensor
        sensor_data.sensor_id = SENSOR_ID;
        sensor_data.sensor_type = SENSOR_TYPE;

        // Aloca um buffer de memória para a descrição
        static char description_buffer[50]; // Ajuste o tamanho conforme necessário

        // Copia a descrição para o buffer com memcpy
        memset(description_buffer, 0, sizeof(description_buffer));
        memcpy(description_buffer, DESCRIPTION, strlen(DESCRIPTION) + 1); // +1 para incluir o caractere nulo

        sensor_data.description = description_buffer;  // Atribui o ponteiro do buffer
    }

    return sensor_data;
}


// Função para atualizar o valor medido do sensor
void update_measured_value(float measured_value) {
    
    // Aloca um buffer de memória para armazenar o valor da medição
    static uint8_t value_buffer[4]; // 4 bytes para armazenar um float (32 bits)
    
    // Copia o valor da medição para o buffer
    memcpy(value_buffer, &measured_value, sizeof(float));

    sensor_data.value = measured_value;  // Atribui o valor medido

    ESP_LOGI(TAG, "Valor medido atualizado: %.2f", measured_value);
}
