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
