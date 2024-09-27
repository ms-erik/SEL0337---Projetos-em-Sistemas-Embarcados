from gpiozero import LED
from time import sleep

red = LED(2)
yellow = LED(3)
green = LED(4)

green.on()
yellow.off()
red.off()

while True:
    sleep(10)
    green.off()
    yellow.on()
    sleep(1)
    yellow.off()
    red.on()
    sleep(10)
    yellow.on()
    sleep(1)
    green.on()
    yellow.off()
    red.off()
