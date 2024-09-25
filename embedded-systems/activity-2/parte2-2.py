import RPi.GPIO as GPIO
from gpiozero import DistanceSensor
import sys
from time import sleep

GPIO.setmode(GPIO.BOARD)
# Configurar Pino do LED
led_pwm_pin = 32
GPIO.setup(led_pwm_pin, GPIO.OUT)
pwm_led = GPIO.PWM(led_pwm_pin, 100)
# Configurar Pinos do Sensor de Distancia
min_dist = 0.1
max_dist = 0.3
sensor_echo_pin = 23
sensor_trigger_pin = 24
sensor = DistanceSensor(echo=sensor_echo_pin, trigger=sensor_trigger_pin)

print('Lendo proximidade para acender o LED')
val = 50
pwm_led.start(val)
try:
    while True:
        # A partir do valor de distancia, gerar uma porcentagem entre max_dist e min_dist
        val = 100 * (max(min_dist, min(max_dist, sensor.distance)) - min_dist)/ (max_dist - min_dist)
        print(f'\r {abs(100 - val)}%                                                                ',end='')
        # A partir dessa porcentagem, ter o LED acender com intencidade de forma que, quanto mais perto (100%), menos intenso
        pwm_led.ChangeDutyCycle(abs(100 - val))
except KeyboardInterrupt:
    print()
    GPIO.cleanup()
    exit(0)
