import asyncio
from bleak import BleakClient
from notify import notify_user
import time

# Endereço MAC do dispositivo ESP32
ESP32_ADDRESS = "24:6f:28:16:6e:0a"  # Substitua pelo endereço correto do seu ESP32

# Variável de controle para evitar notificações frequentes
last_notified_time = 0

def process_sensor_data(data):
    """
    Processa os dados recebidos via Bluetooth e notifica o usuário caso necessário.
    """
    global last_notified_time
    try:
        # Converte os dados recebidos de bytes para string (UTF-8)
        data_str = data.decode('utf-8')

        # Divide a string em partes: "1,0,23.5,Sensor de Temperatura"
        data_parts = data_str.split(',')

        if len(data_parts) == 4:  # Verifica se a string contém os dados esperados
            sensor_id = int(data_parts[0])  # ID do sensor
            sensor_type = int(data_parts[1])  # Tipo do sensor
            sensor_data = float(data_parts[2])  # Valor do sensor
            description = data_parts[3]  # Descrição do sensor

            print(f"\033[91mATENÇÃO: SENSOR ID {sensor_id} do tipo {sensor_type} alarmado: "
                  f"Valor: {sensor_data:.2f}, Descrição: {description}\033[0m")

            # Notifica o usuário se passaram 15 minutos desde a última notificação
            current_time = time.time()
            if current_time - last_notified_time > 900:  # 15 minutos
                message = (f"ATENÇÃO: SENSOR ID {sensor_id} do tipo {sensor_type} alarmado: "
                           f"Valor: {sensor_data:.2f}, Descrição: {description}")
                notify_user(message)
                last_notified_time = current_time
        else:
            print(f"Erro: Dados mal formatados recebidos: {data_str}")
    except Exception as e:
        print(f"Erro ao processar dados: {data}, erro: {e}")

async def notification_handler(sender, data):
    """
    Callback para tratar notificações recebidas do dispositivo Bluetooth.
    """
    print(f"Notificação recebida da característica {sender}: {data}")
    process_sensor_data(data)

async def main():
    """
    Conecta ao dispositivo Bluetooth e inscreve-se para receber notificações das características.
    """
    async with BleakClient(ESP32_ADDRESS) as client:
        print("Conexão estabelecida!")
        print("Aguardando dados do sensor...")

        # Inscreve-se para notificações de todas as características disponíveis no dispositivo
        for service in client.services:
            for char in service.characteristics:
                await client.start_notify(char.uuid, notification_handler)
                print(f"Inscrito para notificações da característica {char.uuid}")

        print("Aguardando notificações (Ctrl+C para sair)...")
        try:
            while True:
                await asyncio.sleep(1)  # Mantém a conexão ativa
        except KeyboardInterrupt:
            print("Encerrando...")
            # Cancela a inscrição de notificações antes de desconectar
            for service in client.services:
                for char in service.characteristics:
                    await client.stop_notify(char.uuid)

# Executa a função principal
asyncio.run(main())
