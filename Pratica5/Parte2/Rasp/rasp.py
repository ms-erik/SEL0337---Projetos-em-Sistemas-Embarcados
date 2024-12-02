import asyncio
from bleak import BleakClient
from notify import notify_user
import time
import uuid  # Importando o módulo uuid para conversão de UUID

# Endereço do dispositivo ESP32
ESP32_ADDRESS = "24:6f:28:16:6e:0a"  # Substitua pelo endereço MAC do ESP32

# UUID da característica (em formato de string)
CHARACTERISTIC_UUID_STR = "87654321-4321-4321-4321-210987654321"  # Substitua pelo UUID correto

# Converte o UUID para o formato necessário
CHARACTERISTIC_UUID = uuid.UUID(CHARACTERISTIC_UUID_STR)

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

# Função de callback para processar notificações
def notification_callback(sender: int, data: bytearray):
    """
    Função chamada quando uma notificação é recebida.
    """
    print(f"Notificação recebida da característica {sender}: {data}")
    process_sensor_data(data)  # Processa os dados da notificação

async def read_characteristic_continuously(client, char_uuid):
    """
    Lê a característica do dispositivo BLE continuamente, esperando notificações.
    """
    try:
        # Inscreve-se para receber notificações da característica
        await client.start_notify(char_uuid, notification_callback)
        print(f"Inscrito para notificações na característica {char_uuid}")

        # A partir daqui, as notificações serão tratadas pela função de callback.
        # Manter o cliente BLE ativo e esperando notificações
        while True:
            await asyncio.sleep(1)  # Mantenha o loop ativo para receber notificações

    except Exception as e:
        print(f"Erro ao inscrever ou receber notificações: {e}")

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
                # Tentamos simplesmente ler a primeira característica disponível que pode ser notificada.
                char_uuid = char.uuid
                characteristic_found = True
                print(f"Encontrada característica com UUID {char_uuid}. Iniciando inscrição para notificações.")
                break

            if characteristic_found:
                break

        if not characteristic_found:
            print("Nenhuma característica encontrada.")
            return  # Sai da função se nenhuma característica for encontrada

        # Inicia a inscrição para notificações da característica encontrada
        await read_characteristic_continuously(client, char_uuid)

# Executa a função principal
asyncio.run(main())
