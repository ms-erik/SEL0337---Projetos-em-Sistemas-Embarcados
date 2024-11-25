#include "TRUTH.h"

const char *sensor_type_to_string(sensor_type_t type) {
    switch (type) {
        case SENSOR_DISTANCE:    return "Sensor de Distância";
        case SENSOR_PRESENCE:    return "Sensor de Presença";
        case SENSOR_TEMPERATURE: return "Sensor de Temperatura";
        case SENSOR_HUMIDITY:    return "Sensor de Umidade";
        case SENSOR_LIGHT:       return "Sensor de Luz";
        default:                 return "Sensor Desconhecido";
    }
}