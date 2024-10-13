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
