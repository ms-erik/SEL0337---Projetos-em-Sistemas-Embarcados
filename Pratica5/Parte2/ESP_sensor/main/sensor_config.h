#ifndef SENSOR_CONFIG_H
#define SENSOR_CONFIG_H

#include <stdint.h>

// Definições das configurações do sensor como macros
#define SENSOR_ID          1
#define SENSOR_TYPE        SENSOR_DISTANCE
#define DESCRIPTION        "Sensor da bicicleta"

// Enum para os tipos de sensores
typedef enum {
    SENSOR_DISTANCE,
    SENSOR_TEMPERATURE,
    SENSOR_HUMIDITY,
    SENSOR_UNKNOWN
} sensor_type_t;

// Estrutura para armazenar os dados do sensor
typedef struct {
    uint8_t sensor_id;
    sensor_type_t sensor_type;
    char description[60];
    float value;  // Valor medido pelo sensor
} sensor_data_t;

// Função para obter os dados do sensor
sensor_data_t get_sensor_data(void);

// Função para atualizar o valor medido do sensor
void update_measured_value(float measured_value);

#endif // SENSOR_CONFIG_H
