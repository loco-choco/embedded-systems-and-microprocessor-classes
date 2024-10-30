#include <delay.h>
#include <pic18fregs.h>

#pragma config XINST = OFF

#define LED_LAT LATDbits.LATD0
#define LED_TRIS TRISDbits.TRISD0

void main(void) {
  // ADCON1 |= 0XF; // Pinos digitais

  LED_TRIS = 0; // LED saida
  LED_LAT = 0;  // LED desligado

  T2CON = 0b01111011;
  // Timer 2
  // Unimplemented   - 0
  // Postscaler 1:16 - 1
  //                   1
  //                   1
  //                   1
  // TMRON             0
  // Prescaler 1:16    1
  //                   1

  // Para ter a contage maxima, comecar a contar do 0
  const int TIMER_INIT = 0;
  TMR2 = TIMER_INIT;
  T2CONbits.TMR2ON = 1;
  while (1) {
    if (PIR1bits.TMR2IF == 1) {
      LED_LAT = ~LED_LAT;
      // Recarrega e reseta o timer
      TMR2 = TIMER_INIT;
      PIR1bits.TMR2IF = 0;
    }
  }
}
