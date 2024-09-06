from gpiozero import DistanceSensor, LED
from signal import pause
from time import sleep

sensor = DistanceSensor(23, 24, max_distance=1, threshold_distance=0.2)
led = LED(16)

sensor.when_in_range = led.on
sensor.when_out_of_range = led.off


try:
    while True:
        print('Distance to nearest object is', sensor.distance, 'm')
        sleep(1)
except KeyboardInterrupt:
    print("\n programa interrompido")
finally:
    led.off()

pause()
