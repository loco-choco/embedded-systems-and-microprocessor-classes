  org 0000h
main:
  ; --- Transferencia de Dados ---
  mov A, #12h
  mov A, #00h
  mov R0, #34h
  mov B, #56
  mov 40h, P1
  mov R4, 40h
  mov 50h, R4
  mov R1, 50h
  mov A, @R1
  mov DPTR, #9A5Bh
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
