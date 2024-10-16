from gpiozero import LED
from time import sleep

pinLED=LED(15)

while True:
	pinLED.on()
	sleep(1)
	pinLED.off()
	sleep(1)
