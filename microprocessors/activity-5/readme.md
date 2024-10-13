# Atividade Semanal 5

**Ivan Roberto Wagner Pancheniak Filho - 12624224**

### Questões

#### Responder com verdadeiro (V) ou falso (F) às seguintes afirmações sobre a Pilha (Stack).

(F) A pilha é uma memória RAM sequencial do tipo FIFO.

(V) A pilha geralmente é utilizada para armazenar endereço de retorno de sub-rotinas e
também de interrupções.

(F) O ponteiro de pilha (Stack Pointer) é um registrador que aponta para um endereço da
memória ROM, que é o endereço de retorno do programa após o atendimento a uma
interrupção ou sub-rotina.

(V) As instruções PUSH e POP são exclusivas para operações com pilha.

(V) A instrução CALL <endereço> deve ser usada para indicar qual endereço o programa
deve desviar no caso de um atendimento à interrupção ou chamada de sub-rotina.

(V) A instrução RET, colocada no final de uma sub-rotina, faz com que o último endereço
armazenado na pilha seja carregado no registrador PC (program counter).

(V) A área da RAM interna dedicada à pilha é determinada pelo ponteiro SP, um dos SFRs,
que possui tamanho 8 bits, mesmo tamanho do barramento de endereço da CPU.

(V) Geralmente são baseadas em flip-flops tipo D

#### Refletir se existe diferença entre o endereço armazenado em um espaço de pilha e o endereço armazenado no Stack Pointer (SP)?

Existe uma diferença entre essas duas coisas. O Stack Pointer apenas indica o topo da pilha,
enquanto que o endereço armazenado no espaço da pilha pode ser o endereço de retorno de uma sub-rotina.

#### Supondo que um banco de 8 LEDs foi conectado à Porta P1 e um banco de 8 Switches conectado à P2 (EdSim51).

**Escrever um programa em Assembly para acender o LED 0 (pode ser qualquer outro) ao acionar o Switch 7 (pode ser qualquer outro). Apagar o LED ao desligar o Switch.**

O programa é o presente no arquivo [1.asm](1.asm).
```asm
org 0000h
jmp main
org 0033h
main:
	jb p2.0, ascender_led
	setb p1.7
	jmp main
ascender_led:
	clr p1.7
	jmp main
```

#### Supondo que foram conectados um banco de 8 switches na Porta P2 (EdSim51).

**Escrever um programa em Assembly para monitorar, quando uma das chaves for pressionada, e
indicar o número da chave pressionada em R0 (Se SW3 foi pressionada, então R0 = 3).**

O programa é o presente no arquivo [2.asm](2.asm).
```asm
org 0000h
jmp main
org 0033h
main:
	mov r0, #0
checar_chaves:
	mov r0, #0
	mov A, p2
checar_chaves_loop:
	clr c
	rrc A
	jnc chave_ligada
	inc r0
	jmp checar_chaves_loop
chave_ligada:
	jmp $
```

#### Criar uma subrotina de delay de 50 milissegundos a partir da contagem de ciclos de instruções e intervalo de tempo.

**Essa estrutura poderá servir para piscar um LED a cada 50 ms (ver exemplo na aula correspondente).**

O programa é o presente no arquivo [3.asm](3.asm).
```asm
org 0000h
jmp main
org 0033h
main:
delay_50ms:
	mov A, #1Ah
	mov r0, #42h
delay_50ms_loop:
	subb A, #1
	jnc delay_50ms_loop
	djnz r0, delay_50ms_loop
led:
	jb p1.0, led_ligar
led_desligar:
	setb p1.0
	jmp main
led_ligar:
	clr p1.0
	jmp main
```

#### Colocou-se 3 LEDs nos endereços P1.0, P1.1 e P1.2 no microcontrolador e 3 chaves nos endereços P2.0, P2.1 e P2.2.

**Considerando que os LEDs acendem quando é colocado nível baixo na saída e as chaves, quando pressionadas,
colocam nível baixo na porta, explique o funcionamento do programa a seguir quando cada chave é pressionada.**

```asm
ORG 0000H
Leitura:
JNB P2.0, PX
JNB P2.1, PY
JNB P2.2, PZ
LCALL Leitura
PX:
MOV P1, #0
RET
PY:
MOV P1, #00000101b
RET
PZ:
MOV A, P1
CPL A
MOV P1, A
RET
FIM:
SJMP FIM
```

De acordo com o programa dado, quando a chave SW0 (P2.0) é ativada (valor 0) todos os 3 LEDs em P1 ativam (000b).
Caso a chave SW1 seja ativada (P2.1), apenas o LED em P1.1 é ativado (101b). E por fim, caso a chave SW2 seja ativada
(P2.2), os estados dos LEDs inverte, portanto, os que estavam ativados (0), desativam (1), e os desativados (1), ativam (0).
