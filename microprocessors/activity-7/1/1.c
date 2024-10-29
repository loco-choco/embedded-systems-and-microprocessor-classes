#include <delay.h>
#include <pic18fregs.h>

#pragma config XINST = OFF

#define LED_LAT LATCbits.LATC0
#define LED_TRIS TRISCbits.TRISC0

void main(void) {
  // ADCON1 |= 0XF; // Pinos digitais

  LED_TRIS = 0; // LED saida
  LED_LAT = 0;  // LED desligado

  T0CON = 0b00000111;
  // Timer 0
  // Desativado - 0
  // 16 bits    - 0
  // Cic. Instr - 0
  // Subida     - 0
  // PSA Habili - 0
  // 1:256      - 1
  //              1
  //              1

  // Com CLOCK de 8MHz -> 0.5us de Cic. Instr,  Prescaler de 1:256, temos que
  // cada bit vale 128us. Para conseguir 1s -> 7812*128us = 0.99s Portanto, TMR0
  // = (65535 - 7812) = 61629 = E17B
  const int TIMER_LOWER = 0x7B;
  const int TIMER_HIGHER = 0xE1;
  TMR0L = TIMER_LOWER;
  TMR0H = TIMER_HIGHER;
  T0CONbits.TMR0ON = 1;
  while (1) {
    if (INTCONbits.TMR0IF == 1) {
      LED_LAT = ~LED_LAT;
      // Recarrega e reseta o timer
      TMR0L = TIMER_LOWER;
      TMR0H = TIMER_HIGHER;
      INTCONbits.TMR0IF = 0;
    }
  }
}
