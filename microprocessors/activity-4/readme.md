# Atividade Semanal 4

## Parte 1
Codigo em Assembly da [primeira parte](1.asm).
```asm
  org 0000h
main:
  ; --- Transferencia de Dados --- Ciclos de Maquinas Total: 16 ciclos
  mov A, #12h       ; 1 us - 1 ciclo
  mov A, #00h       ; 1 us - 1 ciclo
  mov R0, #34h      ; 1 us - 1 ciclo
  mov B, #56        ; 2 us - 2 ciclos
  mov 40h, P1       ; 2 us - 2 ciclos
  mov R4, 40h       ; 2 us - 2 ciclos
  mov 50h, R4       ; 2 us - 2 ciclos
  mov R1, 50h       ; 2 us - 2 ciclos
  mov A, @R1        ; 1 us - 1 ciclo
  mov DPTR, #9A5Bh  ; 2 us - 2 ciclos
  ; --- Instrucoes Aritmeticas ---
  mov ACC, #2
  mov B, #3
  mov R4, #7
  add A, R4
  dec ACC ; dec de ACC 3 vezes
  dec ACC
  dec ACC
  inc B
  subb A, B
  mul AB
  inc B ; inc de B 2 vezes
  inc B
  div AB
  mov 70h, A
  mov 71h, B
  ; --- Instrucoes Logicas e Booleanas ---
  mov A, #11001100b
  mov B, #10101010b
  anl A, B
  rr A ; rotacionar A para direita 2 vezes
  rr A
  cpl A
  rl A ; rotacionar A para esquerda 2 vezes
  rl A
  orl A, B
  xrl A, B
  swap A
  mov psw, A
  ; --- Instrucoes de Desvio Condicional e Incodicional ---
inicio:
  clr A
bloco1:
  mov R0, #10h
  jz bloco2 ; pula para bloco2 se A==0
  jnz bloco3 ; pula para bloco3 se A!=0
  nop ; consumir 1us sem fazer nada
bloco2:
  mov A, R0
  jmp bloco1
bloco3:
  djnz R0, bloco3
  jmp inicio
  ; --- Encerramento do Programa ---
  jmp $ ; Segura o programa nessa linha
end
```
### Questões

#### Sobre Transferência de Dados

##### Qual foi o tempo gasto em cada linha de instrução e quantos ciclos de máquina esse programa contém?
**Justifique sua resposta.**

Simulando no EdSim51, o tempo gasto em cada linha de instrução foi os na tabela a seguir:
|-------------------|------|
| mov A, #12h       | 1 us |
| mov A, #00h       | 1 us |
| mov R0, #34h      | 1 us |
| mov B, #56        | 2 us |
| mov 40h, P1       | 2 us |
| mov R4, 40h       | 2 us |
| mov 50h, R4       | 2 us |
| mov R1, 50h       | 2 us |
| mov A, @R1        | 1 us |
| mov DPTR, #9A5Bh  | 2 us |
Disso, determinamos que, pelo clock do processador ser de 1 us, 
a quantidade de ciclos de maquina gasta foi de 16us.

##### O que aconteceu ao mover uma porta inteira de 8 registradores (ex.: MOV A, P1) para um destino e por que seu valor é FF?

Ao mover uma porta inteira de 8 registradores para um destino, essa porta acaba sendo lida.
E por ela nao ter nenhum valor atualmente, retorna-se seu valor inicial de FFh (todos os bits em 1).

##### Qual valor apareceu no acumulador após ter movido R1 de forma indireta para ele?

Apareceu o valor no endereco do valor contido em R1, ou seja, por ele ter 50h,
o acumulador ficou com o valor no endereco 50h de 40h.

##### Por que foi possível mover um valor de 4 dígitos para DPTR?
**Em quais registradores especiais do simulador foi possível verificar mudanças quando
essa operação foi realizada? Qual o maior valor que pode ser movido para DPTR em hexadecimal?**

Eh possivel mover um valor de 4 digitos para DPTR por ele ser um registrador composto por 2 registradores de 8 bits,
DPH e DPL. Ao ser executado o MOV para DPTR, os registrados DPH e DPL tem seus valores atualizados, com DPL recebendo a 
parte menos significativa do valor, e DPH a mais significativa. Com essa capacidade expandida de 16 bits, o DPTR pode 
ter como maior valor movido para ele FFFFh.

#### Sobre Instruções Aritméticas

##### Por que ao mover o valor 4 para ACC, o bit menos significativo de PSW resulta em 1?
**E ao mover o valor 3, esse bit resulta em 0?**
Isso ocorre pelo bit menos significativo de PSW indicar se o bit menos significativo do ACC esta ativado,
ou seja, se o ACC eh um numero par (bit de PSW em 1) ou impar (bit de PSW em 0).

##### Tente decrementar 1 unidade de algum registrador ou endereço de memória cujo valor é igual a zero
**(ex.: DEC A, DEC Rn, ou DEC 60h, sendo A, Rn, ou 60h iguais a zero). Por que a operação resulta em FF?**

Essas operacoes resultam em FF por essa subtracao causar o underflow do valor binario, fazendo com que 00h
va para o maior valor representavel, que eh FFh.

## Parte 2
Codigo em Assembly da [segunda parte](2.asm).
```asm
org 0000h
  jmp main
  ; Descobre a quantidade de enderecos entre 20h-23h 
  ; com valores menores que 45h
  org 0033h
main:
  ; current_addr (R0) = 20h
  mov R0, #20h
  ; amount_of_addrs (R1) = 0
  mov R1, #0
  ; do {
loop:
    ; if mem[current_addr] - 45h < 0
    mov A, @R0
    subb A,#45h
    jnc if_end 
  if_less_then_45: ; {
      ; amount_of_addrs++
      inc R1
  if_end: ; }
    ; current_addr++
    inc R0
loop_condition: ; }
  ; while current_addr - 24h < 0
  mov A, R0
  subb A,#24h
  jc loop
program_end:
  ; para segurar o programa no fim,
  ; e nao deixar ele executar instrucoes invalidas
  ; while (1){}
  nop
  jmp program_end
end
```

Ele faz a funcao do seguinte pseudo codigo:
```C
current_addr = 20h
amount_of_addrs = 0
while(current_addr < 24h){
  if(mem[current_addr] < 45h){
    amount_of_addrs++
  }
  current_addr++
}
```
