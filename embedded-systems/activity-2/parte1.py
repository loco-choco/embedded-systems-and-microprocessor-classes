from RPI.GPIO import LED
from time import sleep

pinLED=LED(4)

while True:
	pinLED.on()
	sleep(1)
	pinLED.off()
