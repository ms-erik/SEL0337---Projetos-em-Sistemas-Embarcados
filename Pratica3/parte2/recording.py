import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522


from mfrc522 import SimpleMFRC522
from time import sleep
import RPi.GPIO as GPIO

GPIO.setwarnings(False)
leitor = SimpleMFRC522()
texto = "eba6996" 
print("Aproxime a tag do leitor para gravar.")
leitor.write(texto)
print("Concluído!")


GPIO.setwarnings(False)
leitor = SimpleMFRC522()
print("Aproxime a tag do leitor para leitura.")
while True: 
    id,texto = leitor.read()
    print("ID: {}\nTexto: {}".format(id, texto))
    sleep(3) 
