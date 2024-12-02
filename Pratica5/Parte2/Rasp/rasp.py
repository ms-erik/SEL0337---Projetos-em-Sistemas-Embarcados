import asyncio
from bleak import BleakClient
from notify import notify_user
import time

# Endereço do dispositivo ESP32 
# ESP32_ADDRESS = "e0:5a:1b:5f:d7:16"  # Substitua pelo endereço MAC do ESP32
ESP32_ADDRESS = "24:6f:28:16:6e:0a"  # Substitua pelo endereço MAC do ESP32

# Variável de controle para garantir que o e-mail seja enviado a cada 15 minutos
last_notified_time = 0

# Função para processar os dados recebidos
def process_sensor_data(data):
    """
    Processa os dados recebidos via Bluetooth e notifica o usuário se necessário.
    """
    global last_notified_time
    try:
        # Converte os dados recebidos em string (assumindo que sejam enviados como string)
        data_str = data.decode('utf-8')  # Converte de bytes para string (UTF-8)

        # Exemplo de string recebida: "1,0,23.5,Sensor de Temperatura"
        # Divida a string usando vírgula ou outro delimitador, dependendo de como os dados são enviados
        data_parts = data_str.split(',')  # Ajuste o delimitador conforme necessário

        # Verifica se a string foi dividida corretamente em 4 partes
        if len(data_parts) == 4:
            sensor_id = int(data_parts[0])  # ID do sensor
            sensor_type = int(data_parts[1])  # Tipo do sensor
            sensor_data = float(data_parts[2])  # Valor do sensor (convertido para float)
            description = data_parts[3]  # Descrição do sensor

            print(f"\033[91mATENÇÃO: SENSOR ID {sensor_id} do tipo {sensor_type} alarmado: "
                  f"Valor: {sensor_data:.2f}, Descrição: {description}\033[0m")

            # Verifica se passaram 15 minutos desde o último envio de notificação
            current_time = time.time()
            if current_time - last_notified_time > 900:  # 900 segundos = 15 minutos
                message = (f"ATENÇÃO: SENSOR ID {sensor_id} do tipo {sensor_type} alarmado: "
                           f"Valor: {sensor_data:.2f}, Descrição: {description}")
                notify_user(message)  # Envia o e-mail com a mensagem genérica
                last_notified_time = current_time  # Atualiza o tempo do último envio

        else:
            print(f"Erro: Dados mal formatados recebidos: {data_str}")
    except Exception as e:
        print(f"Erro ao processar dados: {data}, erro: {e}")


async def read_characteristic_continuously(client, char_uuid):
    """
    Lê a característica do dispositivo BLE continuamente.
    """
    while True:
        try:
            # Lê o valor da característica especificada (passada como argumento)
            data = await client.read_gatt_char(char_uuid)
            print(f"Dados recebidos da característica {char_uuid}: {data}")
            process_sensor_data(data)  # Processa os dados recebidos
            await asyncio.sleep(1)  # Aguarda 1 segundo antes de ler novamente
        except Exception as e:
            print(f"Erro ao ler característica {char_uuid}: {e}")
            await asyncio.sleep(1)  # Em caso de erro, tenta novamente após 1 segundo


async def main():
    """
    Estabelece conexão com o dispositivo Bluetooth, encontra uma característica e lê ela continuamente.
    """
    async with BleakClient(ESP32_ADDRESS) as client:
        print("Conexão estabelecida!")

        print("Procurando por características no dispositivo...")
        services = await client.get_services()
        characteristic_found = False
        char_uuid = None
        
        for service in services:
            for char in service.characteristics:
                # Agora não estamos mais verificando a propriedade 'notify'.
                # Tentamos simplesmente ler a primeira característica disponível.
                char_uuid = char.uuid
                characteristic_found = True
                print(f"Encontrada característica com UUID {char_uuid}. Iniciando leitura contínua.")
                break

            if characteristic_found:
                break

        if not characteristic_found:
            print("Nenhuma característica encontrada.")
            return  # Sai da função se nenhuma característica for encontrada

        # Inicia a leitura contínua da característica encontrada
        await read_characteristic_continuously(client, char_uuid)

# Executa a função principal
asyncio.run(main())
