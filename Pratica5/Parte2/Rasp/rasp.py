import asyncio
import struct
from bleak import BleakClient
from notify import notify_user  # Certifique-se de que a função notify_user está implementada no arquivo notify.py

# Endereço do dispositivo ESP32 (adicione manualmente ou procure dinamicamente)
ESP32_ADDRESS = "e0:5a:1b:5f:d7:16"  # Substitua pelo endereço MAC do ESP32

# Callback para processar notificações
def notification_handler(sender, data):
    flag=0
    """
    Processa os dados recebidos via Bluetooth e notifica o usuário se necessário.
    """
    try:
        # Decodifica os dados como ponto flutuante (IEEE 754, 4 bytes)
        if len(data) == 4:  # Certifica-se de que tem o tamanho correto
            distancia = struct.unpack('<f', data)[0]  # '<f' indica float em ordem little-endian
            print(f"\033[91mATENÇÃO: SENSOR DE DISTÂNCIA IDENTIFICOU AUSÊNCIA DO OBJETO MONITORADO.\n"
                    f"Distância capturada: {distancia:.2f} cm\033[0m")
            if (flag == 0):
                         notify_user()
                         flag+=1
           # print(f"Distância capturada: {distancia:.2f} cm")
        else:
            print(f"Dado recebido inválido (tamanho {len(data)} bytes): {data}")
    except Exception as e:
        print(f"Erro ao processar dados de {sender}: {data}, erro: {e}")

async def main():
    """
    Estabelece conexão com o dispositivo Bluetooth e monitora notificações.
    """
    async with BleakClient(ESP32_ADDRESS) as client:
        print("Conexão estabelecida!")

        print("Serviços disponíveis:")
        # Itera por todos os serviços e características
        for service in client.services:
            print(f"Serviço: {service.uuid}")
            for char in service.characteristics:
                print(f"  Característica: {char.uuid}")
                # Inscreve-se para notificações de todas as características
                await client.start_notify(char.uuid, notification_handler)
                print(f"Inscrito para notificações da característica {char.uuid}")

        print("Aguardando notificações (Ctrl+C para sair)...")
        try:
            while True:
                await asyncio.sleep(1)  # Mantém a conexão aberta
        except KeyboardInterrupt:
            print("Encerrando...")
            # Interrompe a notificação de todas as características
            for service in client.services:
                for char in service.characteristics:
                    await client.stop_notify(char.uuid)

# Executa a função principal
asyncio.run(main())
