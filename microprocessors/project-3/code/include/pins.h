// Definicão dos pinos que serão usados nesse projeto
// com nomes mais legiveis
#ifndef PINS_H
#define PINS_H
// LCD PINS
#define LCD_RS LATEbits.LATE2
#define LCD_RS_TRIS TRISEbits.TRISE2
#define LCD_EN LATEbits.LATE1
#define LCD_EN_TRIS TRISEbits.TRISE1
#define LCD_D7 LATDbits.LATD7
#define LCD_D7_TRIS TRISDbits.TRISD7
#define LCD_D6 LATDbits.LATD6
#define LCD_D6_TRIS TRISDbits.TRISD6
#define LCD_D5 LATDbits.LATD5
#define LCD_D5_TRIS TRISDbits.TRISD5
#define LCD_D4 LATDbits.LATD4
#define LCD_D4_TRIS TRISDbits.TRISD4
#endif
