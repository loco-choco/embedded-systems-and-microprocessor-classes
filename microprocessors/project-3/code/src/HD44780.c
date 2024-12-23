#include "HD44780.h"    // Protótipo das funcoes implementadas por esse código
#include <delay.h>      // Rotinas de delay para a pic
#include <pic18fregs.h> // Pinos da pic

// Faz o delay necessário após uma escrita de comandos de 8bits
void delay_for_busy_flag_8_bits(void) {
  // Temos que permitir com que o LCD escreva nos pinos
  // pois ele coloca informacao de seu estado interno
  // o qual estamos ignorando
  LCD_D4_TRIS = 1;
  LCD_D5_TRIS = 1;
  LCD_D6_TRIS = 1;
  LCD_D7_TRIS = 1;
  // Emula o delay e o ciclo de clock de receber no modo 8 bits
  LCD_EN = 1;
  delay1ktcy(2);
  LCD_EN = 0;
  delay1ktcy(2);
  LCD_D4_TRIS = 0;
  LCD_D5_TRIS = 0;
  LCD_D6_TRIS = 0;
  LCD_D7_TRIS = 0;
}

// Faz o delay necessário após uma escrita de comandos de 4bits
void delay_for_busy_flag_4_bits(void) {
  // Emula o delay e o ciclo de clock de receber no modo 4 bits
  delay_for_busy_flag_8_bits();
  delay_for_busy_flag_8_bits();
}

// Envia um comando de 8bits mas apenas com 4 ports (apenas os 4 primeiros bits
// de command são enviados pelos ports de dados mais significativos)
void send_command_to_lcd_8_incomplete_bits(int rs, int command) {
  LCD_EN = 1;
  LCD_RS = rs;
  LCD_D7 = (command & (1 << 3)) >> 3;
  LCD_D6 = (command & (1 << 2)) >> 2;
  LCD_D5 = (command & (1 << 1)) >> 1;
  LCD_D4 = command & 1;
  delay1ktcy(8); // Delay para chegar no LCD
  LCD_EN = 0;
  delay1ktcy(2);
}

// Envia um comando no formato de 4bits
void send_command_to_lcd_4_bits(int rs, int command) {
  LCD_EN = 1;
  LCD_RS = rs;
  LCD_D7 = (command & (1 << 7)) >> 7;
  LCD_D6 = (command & (1 << 6)) >> 6;
  LCD_D5 = (command & (1 << 5)) >> 5;
  LCD_D4 = (command & (1 << 4)) >> 4;
  delay1ktcy(8); // Delay para chegar no LCD
  LCD_EN = 0;
  delay1ktcy(2);
  LCD_EN = 1;
  LCD_RS = rs;
  LCD_D7 = (command & (1 << 3)) >> 3;
  LCD_D6 = (command & (1 << 2)) >> 2;
  LCD_D5 = (command & (1 << 1)) >> 1;
  LCD_D4 = command & 1;
  delay1ktcy(8); // Delay para chegar no LCD
  LCD_EN = 0;
  delay1ktcy(2);
}
// Configura as portas do LCD e envia os primeiros comandos para ligar e
// habilitar a escrita desejada no LCD
void config_lcd(void) {
  int busy = 0;
  // Seta ports de controle e dados como saida
  LCD_RS_TRIS = 0;
  LCD_EN_TRIS = 0;
  LCD_D4_TRIS = 0;
  LCD_D5_TRIS = 0;
  LCD_D6_TRIS = 0;
  LCD_D7_TRIS = 0;

  // Tabela 12 de https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
  // explica como fazer operacao de uma linha em 4 bits
  // Setando modo 4 bits
  send_command_to_lcd_8_incomplete_bits(0, 0b0010);
  delay_for_busy_flag_4_bits();

  send_command_to_lcd_4_bits(0, 0b00100011); // Seleciona linha 1 e fonte 5x8
  delay_for_busy_flag_4_bits();
  // Liga o display mas com cursor desligado
  send_command_to_lcd_4_bits(0, 0b00001100);
  delay_for_busy_flag_4_bits();
  //  Seta o modo para incrementar e
  //  shiftar para o lado a cada escrita
  send_command_to_lcd_4_bits(0, 0b00000110);
  delay_for_busy_flag_4_bits();
}
// Envia um comando de escrever um caracter no display
void print_char_lcd(int c) { send_command_to_lcd_4_bits(1, c); }
// Move o cursor do LCD para o inicio
void go_to_start_lcd(void) {
  send_command_to_lcd_4_bits(0, 0b00000010); // Move o cursor para o inicio
  delay_for_busy_flag_4_bits();
}
// Escreve a primeira casa de um numero
void print_number_1unit(int num) { print_char_lcd('0' + num % 10); }
// Escreve as 2 primeiras casas de um numero
void print_number_2units(int num) {
  print_char_lcd('0' + (num / 10) % 10);
  print_char_lcd('0' + num % 10);
}
// Escreve as 4 primeiras casas de um numero
void print_number_4units(int num) {
  print_char_lcd('0' + (num / 1000) % 10);
  print_char_lcd('0' + (num / 100) % 10);
  print_char_lcd('0' + (num / 10) % 10);
  print_char_lcd('0' + num % 10);
}
