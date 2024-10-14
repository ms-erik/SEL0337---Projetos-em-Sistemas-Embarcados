from gpiozero import LED  
from time import sleep 

green = LED(15)
green.close()

red = LED(18)

red.on()
sleep(15)
red.off()
