import smbus
import time

bus = smbus.SMBus(1)
address = 0x08

def read_potentiometer():
    data = bus.read_i2c_block_data(address, 0, 2) # Lê dois bytes
    value = data[0] * 256 + data[1] # Reconstroi o valor de 10 bits
    return value

while True:
    input("Pressione Enter para ler o valor do potenciômetro: ")
    potentiometer_value = read_potentiometer()
    print("Valor do potenciômetro:", potentiometer_value)
