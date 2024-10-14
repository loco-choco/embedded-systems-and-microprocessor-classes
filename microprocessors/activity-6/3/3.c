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
  LED_LAT = 0;     // LED desligado
  int button_pressed = 0;
  while (1) {
    if (BUTTON_PORT == 0 && button_pressed == 0) {
      LED_LAT = !LED_LAT;
      button_pressed = 1;
      delay1ktcy(100); // 50ms @ 8MHz
    } else if (BUTTON_PORT == 1 && button_pressed == 1) {
      button_pressed = 0;
      delay1ktcy(100); // 50ms @ 8MHz
    }
  }
}
