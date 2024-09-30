from mfrc522 import SimpleMFRC522
from time import sleep
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(21, GPIO.OUT)
GPIO.output(21, False)

leitor = SimpleMFRC522()
print("Aproxime a tag do leitor para leitura.")
try:
    while True: 
        id,texto = leitor.read()
        if:
            print("Acesso permitido")
            GPIO.output(21, True)
        }
        else:
            print("Acesso negado");
            GPIO.output(21, False)
        sleep(3) 

except KeyboardInterrupt:
    GPIO.cleanup()
