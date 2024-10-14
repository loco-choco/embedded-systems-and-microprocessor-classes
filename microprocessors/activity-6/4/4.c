#include <delay.h>
#include <pic18fregs.h>

#pragma config XINST = OFF

#define BUTTON_PORT PORTBbits.RB0
#define BUTTON_TRIS TRISBbits.TRISB0
#define SEG7_LAT LATD
#define SEG7_TRIS TRISD

int seg(int number) {
  switch (number) {
  case 0:
    return 0b00111111;
  case 1:
    return 0b00000110;
  case 2:
    return 0b01011011;
  case 3:
    return 0b01001111;
  case 4:
    return 0b01100110;
  case 5:
    return 0b01101101;
  case 6:
    return 0b01111101;
  case 7:
    return 0b00000111;
  case 8:
    return 0b01111111;
  case 9:
    return 0b01101111;
  default:
    return 0;
  }
}

int main(void) {
  ADCON1 |= 0XF; // Pinos digitais

  BUTTON_TRIS = 1; // BUTTON entrada
  SEG7_TRIS = 0;   // SEG7 saida
  SEG7_LAT = 0;
  int button_pressed = 0;
  int number = 0;
  while (1) {
    if (BUTTON_PORT == 0 && button_pressed == 0) {
      button_pressed = 1;
      SEG7_LAT = seg(number);
      number = (number + 1) % 10;
      delay1ktcy(100); // 50ms @ 8MHz
    } else if (BUTTON_PORT == 1 && button_pressed == 1) {
      button_pressed = 0;
      delay1ktcy(100); // 50ms @ 8MHz
    }
  }
}
