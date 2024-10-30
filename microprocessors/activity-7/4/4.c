#include <delay.h>
#include <pic18fregs.h>

#pragma config XINST = OFF

#define LED_LAT LATDbits.LATD1
#define LED_TRIS TRISDbits.TRISD1

void timer_isr(void) __interrupt(1) __using(1) {
  if (INTCON3bits.INT2IF == 1) {
    LED_LAT = ~LED_LAT;
    INTCON3bits.INT2IF = 0;
  }
  return;
}

void main(void) {
  ADCON1 |= 0XF; // Pinos digitais

  LED_TRIS = 0; // LED saida
  LED_LAT = 0;  // LED desligado
  INTCON2bits.RBPU = 0;
  // Porta B liga em 0, tem que ligar os resistores de pullup na porta
  INTCONbits.PEIE = 1;     // Habilita Interrupts dos perifericos
  INTCONbits.GIEH = 1;     // Interrupt de alta prioridade
  RCONbits.IPEN = 1;       // Habilita ter niveis de prioridade
  INTCON3bits.INT2IF = 0;  // Zerar a flag caso ela nao esteja ja zerada
  INTCON3bits.INT2IE = 1;  // Habilita a interrupcao 2
  INTCON2bits.INTEDG2 = 0; // Habilita ao presionar a tecla
  TRISBbits.RB2 = 1;       // Pino B como entrada

  while (1) {
  }
}
