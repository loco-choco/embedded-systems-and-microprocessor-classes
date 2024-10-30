#include <delay.h>
#include <pic18fregs.h>

#pragma config XINST = OFF

#define LED_LAT LATDbits.LATD0
#define LED_TRIS TRISDbits.TRISD0

void main(void) {
  // ADCON1 |= 0XF; // Pinos digitais

  LED_TRIS = 0; // LED saida
  LED_LAT = 0;  // LED desligado

  T3CON = 0b10110000;
  // Timer 3
  // 16 bits    - 1
  // T3CCP2     - 0
  // 1:8        - 1
  //              1
  // T3CPP1     - 0
  // Clock Extrn- 0
  // Clock Intr - 0
  // Habilitar  - 0

  // Para ter a contage maxima, comecar a contar do 0
  const int TIMER_LOWER = 0;
  const int TIMER_HIGHER = 0;
  TMR3L = TIMER_LOWER;
  TMR3H = TIMER_HIGHER;
  T3CONbits.TMR3ON = 1;
  while (1) {
    if (PIR2bits.TMR3IF == 1) {
      LED_LAT = ~LED_LAT;
      // Recarrega e reseta o timer
      TMR3L = TIMER_LOWER;
      TMR3H = TIMER_HIGHER;
      PIR2bits.TMR3IF = 0;
    }
  }
}
