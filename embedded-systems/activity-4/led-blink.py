from gpiozero import LED
from time import sleep

pinLED=LED(18)
# Faz o led no pino GPIO18 piscar com periodo de 2s
while True:
	pinLED.on()
	sleep(1)
	pinLED.off()
	sleep(1)
