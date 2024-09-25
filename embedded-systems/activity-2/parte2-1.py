import RPi.GPIO as GPIO
import RPi.GPIO as PWMLED
import sys
from time import sleep

GPIO.setmode(GPIO.BOARD)
# Configurar Pino do LED
led_pwm_pin = 32
GPIO.setup(led_pwm_pin, GPIO.OUT)
pwm_led = GPIO.PWM(led_pwm_pin, 100)
print('Ligando Led com PWM')

val = 25
pwm_led.start(val)
try:
    while True:
        #val = (val + 25/1000) % 100
        pwm_led.ChangeDutyCycle(val)
        #sleep(0.5/1000)
except KeyboardInterrupt:
    print()
    GPIO.cleanup()
    exit(0)
