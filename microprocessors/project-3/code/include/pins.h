// Definicão dos pinos que serão usados nesse projeto
// com nomes mais legiveis
#ifndef PINS_H
#define PINS_H
// LCD PINS
#define LCD_RS LATBbits.LATB4
#define LCD_RS_TRIS TRISBbits.TRISB4
#define LCD_EN LATBbits.LATB5
#define LCD_EN_TRIS TRISBbits.TRISB5
#define LCD_D7 LATBbits.LATB3
#define LCD_D7_TRIS TRISBbits.TRISB3
#define LCD_D6 LATBbits.LATB2
#define LCD_D6_TRIS TRISBbits.TRISB2
#define LCD_D5 LATBbits.LATB1
#define LCD_D5_TRIS TRISBbits.TRISB1
#define LCD_D4 LATBbits.LATB0
#define LCD_D4_TRIS TRISBbits.TRISB0
#endif
