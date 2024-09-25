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
        val = 100 * (max(min_dist, min(max_dist, sensor.distance)) - min_dist)/ (max_dist - min_dist)
        print(f'\r {abs(100 - val)}%                                                                ',end='')
        pwm_led.ChangeDutyCycle(abs(100 - val))
except KeyboardInterrupt:
    print()
    GPIO.cleanup()
    exit(0)
