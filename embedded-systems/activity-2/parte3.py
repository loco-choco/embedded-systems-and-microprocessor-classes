import RPi.GPIO as GPIO
from gpiozero import DistanceSensor
import sys
import threading
from time import sleep

# Indica para pausar a tarefa
parar = False
# Indica para encerrar a tarefa
terminar = False
def tarefa(pwm_led, terminar_mutex, parar_mutex):
    global parar
    global terminar
    pausar = False
    val = 0
    print('Iniciando Tarefa')
    pwm_led.start(val)
    # A tarefa de 'tarefa' eh continuamente mudar a intensidade do led
    # mas se ele tem que parar, ele deve desligar o LED, e nao avancar na intensidade que estava antes
    while True:
        with terminar_mutex:
            if terminar:
                break
        with parar_mutex:
            pausar = parar
        if not pausar:
            val = (val + 1) % 100
            pwm_led.ChangeDutyCycle(val)
        else:
            pwm_led.ChangeDutyCycle(0)
        sleep(0.01)
    print('Parando Tarefa')

def verificar_proximidade(sensor, terminar_mutex, parar_mutex):
    global parar
    global terminar
    min_dist = 0.15
    ja_parou = False
    print('Iniciando Verificacao')
    # Verifica se o sensor de proximidade gerou uma distancia abaixa de min_dist, e se sim
    # indica para tarefa pausar
    while True:  
        with terminar_mutex:
            if terminar:
                break
        if sensor.distance < min_dist and not ja_parou:
            ja_parou = True
            print('Muito Perto!')
            with parar_mutex:
                parar = True
        elif sensor.distance >= min_dist and ja_parou:
            ja_parou = False
            print('Seguro Continuar')
            with parar_mutex:
                parar = False
        sleep(0.05)
    print('Parando Verificacao')

if __name__ == '__main__':
    GPIO.setmode(GPIO.BOARD)

    # Configurar Pino do LED
    led_pwm_pin = 32
    GPIO.setup(led_pwm_pin, GPIO.OUT)
    pwm_led = GPIO.PWM(led_pwm_pin, 100)
    # Configurar Pinos do Sensor de Distancia
    sensor_echo_pin = 23
    sensor_trigger_pin = 24
    sensor = DistanceSensor(echo=sensor_echo_pin, trigger=sensor_trigger_pin)
    
    # Configurar Threads e sinal de pare compartilhado
    terminar_mutex = threading.Lock()
    parar_mutex = threading.Lock()

    thread_tarefa = threading.Thread(target=tarefa, args=(pwm_led, terminar_mutex, parar_mutex))
    thread_verificacao = threading.Thread(target=verificar_proximidade, args=(sensor, terminar_mutex, parar_mutex))
    
    thread_tarefa.start() 
    thread_verificacao.start()
    
    try:
        while True:
            pass
    except KeyboardInterrupt:
        print('Desligando Aplicacao')

    # Indica para as threads pararem sua execucao
    with terminar_mutex:
        terminar = True

    print('Esperando o resto terminar')

    thread_tarefa.join()
    thread_verificacao.join()
    
    GPIO.cleanup()
    exit(0)
