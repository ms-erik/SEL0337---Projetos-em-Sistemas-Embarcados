from gpiozero import LED  
from time import sleep 

green = LED(15)
green.on()

try:
    while True:
        sleep(4)
        green.off()
        sleep(1)
        green.on()

except KeyboardInterrupt:
    green.off()
    green.close()

finally:
    green.off()
    green.close()