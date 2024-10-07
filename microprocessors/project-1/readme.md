# Projeto 1 - Cronômetro Digital usando Assembly e 8051

**Ivan Roberto Wagner Pancheniak Filho - 12624224**

Nesse projeto de Assembly de 8051 foi desenvolvido um contador com velocidade variável,
expondo o número atual em um _display_ de 7 segmentos, e usando interrupções do temporizador
interno para manter a contagem em um tempo constante.

O programa está no arquivo [timer.asm](timer.asm), e ele tem o seguinte fluxo de código:

``` mermaid
flowchart TD
    A(Reset) --> B("contador = 0
    ligado = 0
    modo = 0
    numero = 0")
    B --> C(Habilita interupcao do timer0 e seta timer de 8600h - 1/32s)
    C -->SW0{Ve valor de SW0}
    SW0 -->|Igual a SW0_anterior|SW1
    SW0 -->|Diferente que SW0_anterior|ToggleLento("ligado = 1
    modo = 0
    SW0_anterior = SW0")
    ToggleLento --> SW1{Ve valor de SW1}
    SW1 -->|Igual a SW1_anterior|SW0
    SW1 -->|Diferente que SW1_anterior|ToggleRapido("ligado = 1
    modo = 1
    SW1_anterior = SW1")
    ToggleRapido --> SW0
    Interrupt("Timer0
    Interrupt") --> Ligado("Ve valor de ligado")
    Ligado --> |1|Contador(contador++)
    Ligado --> |0|ResetTimer
    Contador --> Modo(Ve valor de modo)
    Modo --> |0|Lento(Contador divisivel por 32)
    Modo --> |1|Rapido(Contador divisivel por 8)
    Lento --> |Nao|ResetTimer
    Rapido --> |Nao|ResetTimer 
    Lento --> |Sim|Numero(numero = numero + 1 % 10)
    Rapido --> |Sim|Numero
    Numero --> Atualiza(Atualiza display com numero)
    Atualiza --> ResetTimer(Reseta Timer0 com 8600h)
    ResetTimer --> RETI(RETI)
```

Sendo que cada registrador tem as seguintes funções:

|    |              |
|----|--------------|
| r0 | numero       |
| r1 | contador     |
| r2 | ligado       |
| r3 | modo         |
| r4 | SW0_anterior |
| r5 | SW1_anterior |

## Diagrama das conexoes com o processador

![Diagrama](diagram.png)

## Video Explicativo do Funcionamento do Programa


[![Video](https://img.youtube.com/vi/CPFiauVPb5c/0.jpg)](https://www.youtube.com/watch?v=CPFiauVPb5c)

