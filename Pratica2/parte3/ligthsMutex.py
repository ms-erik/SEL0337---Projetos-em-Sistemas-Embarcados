import threading
import time
from gpiozero import LED
from time import sleep

# semaforo 1
red1 = LED(2)
yellow1 = LED(3)
green1 = LED(4)

#semaforo 2
red2 = LED(5)
yellow2 = LED(6)
green2 = LED(7)

mutex = threading.Lock()

def semaforo1():
    while True:
        mutex.acquire()
        green1.on()
        yellow1.off()
        red1.off()
        mutex.release()
        sleep(10)

        mutex.acquire()
        green1.off()
        yellow1.on()
        mutex.release()
        sleep(1)

        mutex.acquire()
        yellow1.off()
        red1.on()
        mutex.release()
        sleep(10)

def semaforo2():
    while True:
        mutex.acquire()
        green2.on()
        yellow2.off()
        red1.off()
        mutex.release()
        sleep(10)

        mutex.acquire()
        green2.off()
        yellow2.on()
        mutex.release()
        sleep(1)

        mutex.acquire()
        yellow2.off()
        red2.on()
        mutex.release()
        sleep(10)



if __name__ == '__main__':
    thread1 = threading.Thread(target=semaforo1)
    thread2 = threading.Thread(target=semaforo2)

    thread1.start()
    thread2.start()

    thread1.join()
    thread2.join()
    
