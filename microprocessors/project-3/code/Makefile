CFLAGS= -I include/ -mpic16 -p18F4550 --use-non-free 
all: HD44780
	sdcc $(CFLAGS) src/main.c out/HD44780.o -o out/
HD44780: src/HD44780.c
	sdcc $(CFLAGS) -c src/HD44780.c -o out/HD44780.o
clean:
	rm out/*
