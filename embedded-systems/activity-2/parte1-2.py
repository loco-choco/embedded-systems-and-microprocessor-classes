import RPi.GPIO as GPIO
import sys
from time import sleep

# Receber entrada do terminal
if len(sys.argv) < 2:
    print('Sem argumentos suficientes')
    exit(-1)
# Verificar se entrada eh correta
if not sys.argv[1].isdigit():
    print(f'Entrada {sys.argv[1]} nao eh um numero maior ou igual a 0')
    exit(-1)

countdown = int(sys.argv[-1])

# Configurar Pino do LED
GPIO.setmode(GPIO.BOARD)
pin = 16
GPIO.setup(pin, GPIO.OUT)

GPIO.output(pin, GPIO.LOW)
# Faz a contagem regressiva, parando e saindo caso receba um CTRL + C
try:
    while countdown > 0:
        print(f'\rLigando o LEd em {int(countdown/60)}:{countdown%60}             ', end='')
        countdown-=1
        sleep(1)
except KeyboardInterrupt:
    print()
    GPIO.cleanup()
    exit(0)
print('\nLed ligado!')
GPIO.output(pin, GPIO.HIGH)

# Esperar CTRL+C para sair
try:
    while True:
        pass
except KeyboardInterrupt:
    pass

GPIO.cleanup()
exit(0)
