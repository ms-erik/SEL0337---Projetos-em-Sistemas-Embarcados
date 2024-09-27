import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(18, GPIO.OUT)


def counter(seconds):
    while(seconds):
        min, sec = divmod(seconds, 60)
        time = '{:02d}:{02d:}'.format(min, sec)
        print(time, end='\r')
        seconds-=1
        sleep(1)
    print("Time's over, LED's ON!\n")
    GPIO.output(18, True)


def main():
    while True:
        try:
            c = int(input('Type a value in secons\n'))
            if (c <=0):
                print("Invalid number(negative)\n")
            else:
                counter(c)
                break;

        except ValueError:
            print("Invalid input!\n")


try:
    main()
except:
    GPIO.cleanup()
