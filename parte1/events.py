import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False) # desabilita avisos

def button(pin)
    if GPIO.input(pin):
        GPIO.output(18, False)
        print (“released”)
    else:
# SE o botão foi pressionado:
        GPIO.output(18, True)
        print(“pressed”)


GPIO.setup(4, GPIO.IN, GPIO.PUD_UP)
GPIO.setep(18, GPIO.OUT)

# GPI.PUD_UP = “pull-up” / “GPI.PUD_DOWM para “pull-down”

GPIO.add_event_detect(4, GPIO.BOTH, callback =button, bouncetime=200)
try:
    while True:
        pass
except KeyboardInterrupt:
    GPIO.cleanup()
