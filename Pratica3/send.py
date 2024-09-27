import smbus import SMBus
import time

bus = SMBus(1)
address = 0x08


#def send_data(value):
#    bus.write_byte(address, value)

while True:
    ledstate = input("Digite 0 para desligar o LED, 1 para ligar:")
    if ledstate == "1":
        bus.write_byte(address, 0x1)
    elif ledstate == "0":
        bus.write_byte(addr,0x0)
    else:
        print("Comando inválido, saindo.")
        break
    time.sleep(1)
