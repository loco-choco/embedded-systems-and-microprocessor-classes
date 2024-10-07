  org 0000h
  jmp main
  org 000Bh ; TIMER 0 Interrupt
timer0_int:
  clr tr0 ; para timer 0
  clr tf0 ; limpa a flag do timer 0
  jmp timer0_int_body
numeros: ; 0 ate 9
  db 11000000b, 11111001b, 10100100b, 10110000b, 10011001b
  db 10010010b, 10000010b, 11111000b, 10000000b, 10010000b
  org 0033h
main:
  ; Variaveis
  ; r0 - numero
  ; r1 - contador
  ; r2 - ligado
  ; r3 - modo
  ; r4 - status anterior de sw0 (p2.0) - 0, desabilitado. 1, habilitado.
  ; r5 - status anterior de sw1 (p2.1) - 0, desabilitado. 1, habilitado.
  ; Resetar todas as variaveis
  mov r0, #0
  mov r1, #0
  mov r2, #0
  mov r3, #0
  mov r4, #0
  mov r5, #0
  ; Pegar o estado atual das chaves sw0 e sw1
  jb p2.0, verifica_sw1_inicio
  mov r4, #01h
verifica_sw1_inicio:
  jb p2.1, habilitar_interrupt_timer_0
  mov r5, #01h
  ; Habilitar interrupts do timer 0
  ; e setar ele com um timer de 8600h (1/32 s)
habilitar_interrupt_timer_0:
  mov tmod, #01h
  mov tl0, #00h
  mov th0, #86h
  setb tr0 ; habilita timer 0
  setb ea ; habilita interrupts
  setb et0 ; habilita interrupt por timer 0
  ; Loop para ligar e alterar modo do timer
  ; dependendo das chaves SW0 (p2.0) e SW1 (p2.1)
input_loop:
  ; se sw0 mudar de status, habilitar modo rapido
  ; se sw1 mudar de status, habilitar modo lento
check_sw0:
  mov a, r4 ; armazenar valor antigo do status de sw0 para referencia
  jb p2.0, sw0_disabled
sw0_enabled:
  mov r4, #1 ; atualiza que sw0 esta ativado
  cjne a, #0, check_sw1
  jmp enable_slow_mode
sw0_disabled:
  mov r4, #0 ; atualiza que sw0 esta desativado
  cjne a, #1, check_sw1
enable_slow_mode:
  mov r2, #1 ; se mudou, ativar o clock
  mov r3, #0 ; colocar em modo lentro
check_sw1:
  mov a, r5 ; armazenar valor antigo do status de sw1 para referencia
  jb p2.1, sw1_disabled
sw1_enabled:
  mov r5, #1 ; atualiza que sw1 esta ativado
  cjne a, #0, loop_end
  jmp enable_fast_mode
sw1_disabled:
  mov r5, #0 ; atualiza que sw1 esta desativado
  cjne a, #1, loop_end
enable_fast_mode:
  mov r2, #1 ; se mudou, ativar o clock
  mov r3, #1 ; colocar em modo rapido
loop_end:
  jmp input_loop
timer0_int_body:
  push ACC
  cjne r2, #1, resetar_timer ; se nao estiver ligado, voltar a contagem do timer
contagem_ligada:
  inc r1
  mov A, r1
  cjne r3, #0, modo_rapido ; Se for 0, modo lento, se for 1, modo rapido
modo_devagar:
  anl A, #00011111b
  cjne A, #0, atualiza_display ; Se o resultando desse AND for 0, entao o valor de contador eh divisivel por 32
  jmp incrementa_numero
modo_rapido:
  anl A, #0000111b
  cjne A, #0, atualiza_display ; Se o resultando desse AND for 0, entao o valor eh divisivel por 8
incrementa_numero:
  inc r0
  mov A, r0
  subb A, #10
  jc atualiza_display ; Se maior que 9, zerar r0
acima_de_9:
  mov r0, #0
atualiza_display:
  mov dptr, #numeros
  mov a, r0
  movc a, @a+dptr
  mov p1, a
resetar_timer:
  mov tl0, #00h
  mov th0, #86h
  setb tr0 ; habilita timer 0
  pop ACC
  reti
  
