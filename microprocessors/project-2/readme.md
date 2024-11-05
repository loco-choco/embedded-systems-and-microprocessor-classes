# Projeto 2 - 

**Ivan Roberto Wagner Pancheniak Filho - 12624224**

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

## Diagrama das conexoes com o processador

## Video Explicativo do Funcionamento do Programa


