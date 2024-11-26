import asyncio
from bleak import BleakClient

# Endereço do dispositivo ESP32 (adicione manualmente ou procure dinamicamente)
ESP32_ADDRESS = "e0:5a:1b:5f:d7:16"  # Substitua pelo endereço MAC do ESP32

# Callback para processar notificações
def notification_handler(sender, data):
    print(f"Notificação de {sender}: {data}")

async def main():
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

asyncio.run(main())
