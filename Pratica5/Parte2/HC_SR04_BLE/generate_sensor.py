import re

# Caminhos dos arquivos
HEADER_FILE = "main/sensor_config.h"
OUTPUT_FILE = "main/sensor_config.c"

def parse_sensor_types(header_file):
    """
    Lê o sensor_config.h e extrai os tipos de sensores do enum sensor_type_t.
    """
    sensor_types = []
    with open(header_file, "r") as file:
        content = file.read()
        match = re.search(r"typedef\s+enum\s*{([^}]*)}", content, re.DOTALL)
        if match:
            enum_body = match.group(1)
            sensor_types = [
                line.strip().rstrip(",")
                for line in enum_body.splitlines()
                if line.strip() and not line.strip().startswith("//")
            ]
    return sensor_types

def generate_sensor_config(sensor_id, sensor_type, description, output_file):
    """
    Gera o código do sensor_config.c com os dados fornecidos.
    """
    template = f"""\
#include "sensor_config.h"

// Dados do sensor configurados aqui
static const uint8_t SENSOR_ID = {sensor_id};
static sensor_type_t SENSOR_TYPE = {sensor_type};
static const char *DESCRIPTION = "{description}";

// Funcao que retorna os dados do sensor
sensor_data_t get_sensor_data(void) {{
    sensor_data_t data;
    data.sensor_id = SENSOR_ID;
    data.sensor_type = SENSOR_TYPE;
    data.description = DESCRIPTION;
    return data;
}}
"""
    with open(output_file, "w") as file:
        file.write(template)

def main():
    print("### Gerador de sensor_config.c ###")
    
    # Parseia o sensor_config.h para obter os tipos de sensores
    sensor_types = parse_sensor_types(HEADER_FILE)
    if not sensor_types:
        print(f"Erro: Não foi possível encontrar o enum 'sensor_type_t' no arquivo {HEADER_FILE}.")
        return

    # Mostra as opções de tipos de sensores
    print("\nTipos de sensores disponíveis:")
    for i, sensor_type in enumerate(sensor_types):
        print(f"{i + 1}. {sensor_type}")

    # Solicita ao usuário o tipo de sensor
    while True:
        try:
            choice = int(input("\nEscolha o tipo de sensor (número): "))
            if 1 <= choice <= len(sensor_types):
                sensor_type = sensor_types[choice - 1]
                break
            else:
                print("Escolha inválida. Tente novamente.")
        except ValueError:
            print("Entrada inválida. Tente novamente.")

    # Solicita a descrição do sensor
    description = input("Digite a descrição do sensor (ex.: Sensor da sala): ").strip()

    # Gera um ID incremental para o sensor
    # Em um sistema mais complexo, isso poderia ser baseado em um arquivo ou banco de dados
    sensor_id = 1  # Exemplo inicial: poderia ser incrementado de acordo com um registro externo

    # Gera o arquivo sensor_config.c
    generate_sensor_config(sensor_id, sensor_type, description, OUTPUT_FILE)
    print(f"\nArquivo {OUTPUT_FILE} gerado com sucesso!")

if __name__ == "__main__":
    main()
