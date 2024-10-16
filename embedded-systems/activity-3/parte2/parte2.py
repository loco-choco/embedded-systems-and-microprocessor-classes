from RPI.GPIO import LED
from mfrc522 import SimpleMFRC522

GPIO.setwarnings(False)

GPIO.setup(led_pwm_pin, GPIO.OUT)

# Leds de status
led_vermelho = LED(4)
led_amarelo = LED(5)
led_verde = LED(6)

pinLED.on()
pinLED.off()


leitor = SimpleMFRC522()

print("Digite 'l' para ler a tag e comparar com a sua chave")
print("Digite 'e' para escrever a sua chave na tag")
print("Digite outra coisa para sair")
flag = True
while flag:
    comando = input(">>> ")
    if command == "e": 
        chave = input("chave: ")
        print("Aproxime a tag do leitor para gravar.")
        led_amarelo.on()
        leitor.write(chave) 
        led_amarelo.off()
        led_verde.on()
        print("Concluído!")
        sleep(1)
        led_verde.off()
    elif command == "l": 
        chave = input("chave: ")
        print("Aproxime a tag do leitor para ler.")
        chave = input("chave esperada: ")
        led_amarelo.on()
        (tag, chave_lida) = leitor.read() 
        chave_lida = chave_lida.strip() # remover whitespaces que aparecem depois
        led_amarelo.off()
        if chave_lida != chave:
            print(f'Chave errada (lido: {chave_lida})')
            led_vermelho.on()
        else:
            print('Chave correta!')
            led_verde.on()
        sleep(1)
        led_verde.off()
        led_vermelho.off()

GPIO.cleanup()
