#include <delay.h>
#include <pic18fregs.h>

#pragma config XINST = OFF

#define LED_LAT LATDbits.LATD0
#define LED_TRIS TRISDbits.TRISD0

const int COUNT_TO = 0xFF - 5;

void timer_isr(void) __interrupt(1) __using(1) {
  if (PIR1bits.TMR1IF == 1) {
    LED_LAT = ~LED_LAT;
    TMR0L = COUNT_TO;
    PIR1bits.TMR1IF = 0;
  }
  return;
}

void main(void) {
  ADCON1 |= 0XF; // Pinos digitais

  LED_TRIS = 0; // LED saida
  LED_LAT = 0;  // LED desligado

  T1CON = 0b10000110;
  // Timer 1
  // 8 bits     - 1
  // outra fonte- 0
  // prescal 1:1- 0
  //              0
  // desativa osc-0
  // nao sincroni-1
  // clock extern-1
  // Desativado  -0
  TMR1L = COUNT_TO;
  T1CONbits.TMR1ON = 1;

  INTCONbits.GIEH = 1; // Interrupt de alta prioridade
  RCONbits.IPEN = 1;   // Habilita ter niveis de prioridade
  PIR1bits.TMR1IF = 0; // Zerar a flag caso ela nao esteja ja zerada
  PIE1bits.TMR1IE = 1; // Habilita interrupt do timer1
  TRISCbits.RC0 = 1;   // Pino C0 como entrada

  while (1) {
  }
}
