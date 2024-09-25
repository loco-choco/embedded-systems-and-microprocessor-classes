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
  
  
  