#ifndef TRUTH_H
#define TRUTH_H

#include <stdint.h>

// Enum para definir os tipos de sensores
typedef enum {
    SENSOR_DISTANCE,
    SENSOR_PRESENCE,
    SENSOR_TEMPERATURE,
    SENSOR_HUMIDITY,
    SENSOR_LIGHT
} sensor_type_t;

// Estrutura para organizar as mensagens BLE
typedef struct {
    int device_id;           // ID do dispositivo
    sensor_type_t type;      // Tipo do sensor
    char description[50];    // Descrição opcional
} sensor_info_t;

// Funções utilitárias
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Converte um tipo de sensor em uma string legível.
 * @param type Tipo do sensor (sensor_type_t).
 * @return Ponteiro para string representando o tipo.
 */
const char *sensor_type_to_string(sensor_type_t type);

#ifdef __cplusplus
}
#endif

#endif // TRUTH_H
