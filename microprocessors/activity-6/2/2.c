#include <delay.h>
#include <pic18fregs.h>

#pragma config XINST = OFF

#define BUTTON_PORT PORTBbits.RB0
#define BUTTON_TRIS TRISBbits.TRISB0
#define LED_LAT LATDbits.LATD0
#define LED_TRIS TRISDbits.TRISD0

void main(void) {
  ADCON1 |= 0XF; // Pinos digitais

  BUTTON_TRIS = 1; // BUTTON entrada
  LED_TRIS = 0;    // LED saida
  LED_LAT = 1;     // LED desligado

  while (1) {
    if (BUTTON_PORT == 0) {
      LED_LAT = ~LED_LAT;
      delay1mtcy(1); // 500ms @ 8MHz
    } else {
      LED_LAT = 0;
    }
  }
}
