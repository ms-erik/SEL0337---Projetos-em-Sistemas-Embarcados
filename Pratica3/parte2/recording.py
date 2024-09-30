import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522



GPIO.setwarnings(False)
leitor = SimpleMFRC522()
texto = "eba6996" 
print("Aproxime a tag do leitor para gravar.")
leitor.write(texto)
print("Concluído!")

