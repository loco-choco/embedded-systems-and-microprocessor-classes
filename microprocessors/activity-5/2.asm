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
