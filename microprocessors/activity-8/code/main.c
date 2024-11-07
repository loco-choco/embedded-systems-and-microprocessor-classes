#include <delay.h>
#include <pic18fregs.h>
#pragma config XINST = OFF

//#define LED_LAT LATDbits.LATD0
//#define LED_TRIS TRISDbits.TRISD0

#define LCD_RS LATEbits.LATE2
#define LCD_RS_TRIS TRISEbits.TRISE2
#define LCD_EN LATEbits.LATE1
#define LCD_EN_TRIS TRISEbits.TRISE1
#define LCD_D7 LATDbits.LATD7
#define LCD_D7_TRIS TRISDbits.TRISD7
#define LCD_D7_IN PORTDbits.RD7
#define LCD_D6 LATDbits.LATD6
#define LCD_D6_TRIS TRISDbits.TRISD6
#define LCD_D6_IN PORTDbits.RD6
#define LCD_D5 LATDbits.LATD5
#define LCD_D5_TRIS TRISDbits.TRISD5
#define LCD_D5_IN PORTDbits.RD5
#define LCD_D4 LATDbits.LATD4
#define LCD_D4_TRIS TRISDbits.TRISD4
#define LCD_D4_IN PORTDbits.RD4

int check_for_busy_flag_4_bits(void) {
  int busy;
  LCD_D4_TRIS = 1;
  LCD_D5_TRIS = 1;
  LCD_D6_TRIS = 1;
  LCD_D7_TRIS = 1;

  LCD_EN = 1;
  delay1ktcy(1);    // Delay para chegar bits do LCD
  busy = LCD_D7_IN; // Busy flag
  // outros dados, mas nao uteis
  // LCD_D6_IN
  // LCD_D5_IN
  // LCD_D4_IN
  LCD_EN = 0;
  delay1ktcy(1);
  return busy;
}

int check_for_busy_flag_8_bits(void) {
  int busy;
  LCD_D4_TRIS = 1;
  LCD_D5_TRIS = 1;
  LCD_D6_TRIS = 1;
  LCD_D7_TRIS = 1;

  LCD_EN = 1;
  delay1ktcy(1);    // Delay para chegar bits do LCD
  busy = LCD_D7_IN; // Busy flag
  // outros dados, mas nao uteis
  // LCD_D6_IN
  // LCD_D5_IN
  // LCD_D4_IN
  LCD_EN = 0;
  delay1ktcy(1);
  LCD_EN = 1;
  delay1ktcy(1); // Delay para chegar bits do LCD
  // LCD_D7_IN; // Busy flag
  // outros dados, mas nao uteis
  // LCD_D6_IN
  // LCD_D5_IN
  // LCD_D4_IN
  LCD_EN = 0;
  delay1ktcy(1);
  return busy;
}

void send_command_to_lcd_8bits(int rs, int command) {
  // Mudar para modo saida
  LCD_D4_TRIS = 0;
  LCD_D5_TRIS = 0;
  LCD_D6_TRIS = 0;
  LCD_D7_TRIS = 0;

  LCD_EN = 1;
  LCD_RS = rs;
  LCD_D7 = (command & (1 << 7)) >> 7;
  LCD_D6 = (command & (1 << 6)) >> 6;
  LCD_D5 = (command & (1 << 5)) >> 5;
  LCD_D4 = (command & (1 << 4)) >> 4;
  delay1ktcy(1); // Delay para chegar no LCD
  LCD_EN = 0;
  delay1ktcy(1);
  LCD_EN = 1;
  LCD_RS = rs;
  LCD_D7 = (command & (1 << 3)) >> 3;
  LCD_D6 = (command & (1 << 2)) >> 2;
  LCD_D5 = (command & (1 << 1)) >> 1;
  LCD_D4 = command & 1;
  delay1ktcy(1); // Delay para chegar no LCD
  LCD_EN = 0;
  delay1ktcy(1);
}

void send_command_to_lcd_4bits(int rs, int command) {
  // Mudar para modo saida
  LCD_D4_TRIS = 0;
  LCD_D5_TRIS = 0;
  LCD_D6_TRIS = 0;
  LCD_D7_TRIS = 0;

  LCD_EN = 1;
  LCD_RS = rs;
  LCD_D7 = (command & (1 << 3)) >> 3;
  LCD_D6 = (command & (1 << 2)) >> 2;
  LCD_D5 = (command & (1 << 1)) >> 1;
  LCD_D4 = command & 1;
  delay1ktcy(1); // Delay para chegar no LCD
  LCD_EN = 0;
  delay1ktcy(1);
}

void config_lcd(void) {
  int busy = 0;
  // Seta ports de controle como saida
  LCD_RS_TRIS = 0;
  LCD_EN_TRIS = 0;
  TRISDbits.TRISD3 = 0;
  TRISDbits.TRISD2 = 0;
  TRISDbits.TRISD1 = 0;
  TRISDbits.TRISD0 = 0;
  LATDbits.LATD3 = 0;
  LATDbits.LATD2 = 0;
  LATDbits.LATD1 = 0;
  LATDbits.LATD0 = 0;
  // Zerar todos os pinos
  // Tabela 12 de https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
  // explica como fazer operacao de uma linha em 4 bits
  // Setando modo 4 bits
  send_command_to_lcd_4bits(0, 0b00000010);
  // busy = check_for_busy_flag_4_bits(); -- o correto seria verificar a flag de
  // ocupado, mas o LCD está num perpetuo modo de WRITE

  send_command_to_lcd_8bits(0, 0b00100000); // Seleciona linha 1 e fonte 5x8
  send_command_to_lcd_8bits(0, 0b00000110); // Liga o display e o cursor
}

void print_char_lcd(int c) { send_command_to_lcd_8bits(1, c); }
void go_to_start_lcd(void) {}

void config_adc(void) {
  // Configura o ADC
  ADCON0bits.CHS = 0b0000; // Canal 0
  ADCON2bits.ADFM = 1;     // Dado justificado para a direita
  ADCON2bits.ADCS = 0b111; // Clock de conversao AD - do oscilador AD
  ADCON2bits.ACQT = 0b000; // Tempo de aquisicao - 0 TAD

  ADCON0bits.ADON = 1; // Liga o ADC
}

void start_adc_conversion(void) { ADCON0bits.GO = 1; }

int adc_is_busy(void) { return ADCON0bits.DONE; }

int read_upper_adc(void) { return ADRESL; }

int read_lower_adc(void) { return ADRESH; }

void main(void) {
  config_lcd();
  config_adc();

  // print_char_lcd(0b11111111);

  int adc_was_started = 0;
  while (1) {
    if (adc_is_busy() == 1)
      continue;
    if (adc_was_started == 0) {
      start_adc_conversion();
      adc_was_started = 1;
    } else {
      adc_was_started = 0;
      int adc_upper = read_upper_adc();
      int adc_lower = read_lower_adc();
      if (adc_upper > 256 / 2) {
        LATDbits.LATD3 = 1;

      } else {
        LATDbits.LATD3 = 0;
      }
    }
  }
}
