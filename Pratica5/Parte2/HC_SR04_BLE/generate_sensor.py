import re

# Caminhos do arquivo de cabeçalho
HEADER_FILE = "main/sensor_config.h"

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

def update_sensor_config(sensor_id, sensor_type, description, header_file):
    """
    Atualiza o arquivo sensor_config.h com os novos valores do sensor.
    """
    with open(header_file, "r") as file:
        content = file.read()

    # Atualiza os valores das macros no arquivo de cabeçalho
    content = re.sub(r"#define SENSOR_ID \d+", f"#define SENSOR_ID {sensor_id}", content)
    content = re.sub(r"#define SENSOR_TYPE \S+", f"#define SENSOR_TYPE {sensor_type}", content)
    content = re.sub(r'#define DESCRIPTION ".*"', f'#define DESCRIPTION "{description}"', content)

    with open(header_file, "w") as file:
        file.write(content)

def main():
    print("### Configuração do sensor ###")
    
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
    sensor_id = 1  # Exemplo inicial: poderia ser incrementado de acordo com um registro externo

    # Atualiza o arquivo sensor_config.h
    update_sensor_config(sensor_id, sensor_type, description, HEADER_FILE)
    print(f"\nArquivo {HEADER_FILE} atualizado com sucesso!")

if __name__ == "__main__":
    main()
