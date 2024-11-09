#include "pins.h"
#include <delay.h>
#include <pic18fregs.h>
#ifndef HD44780_H
#define HD44780_H

#if !defined(LCD_RS) || !defined(LCD_RS_TRIS)
#error "LCD_RS pin isn't defined!"
#endif
#if !defined(LCD_EN) || !defined(LCD_EN_TRIS)
#error "LCD_RS pin isn't defined!"
#endif
#if !defined(LCD_D7) || !defined(LCD_D7_TRIS)
#error "LCD_D7 pin isn't defined!"
#endif
#if !defined(LCD_D6) || !defined(LCD_D6_TRIS)
#error "LCD_D6 pin isn't defined!"
#endif
#if !defined(LCD_D5) || !defined(LCD_D5_TRIS)
#error "LCD_D5 pin isn't defined!"
#endif
#if !defined(LCD_D4) || !defined(LCD_D4_TRIS)
#error "LCD_D4 pin isn't defined!"
#endif

void delay_for_busy_flag_8_bits(void);
void delay_for_busy_flag_4_bits(void);

void send_command_to_lcd_8_incomplete_bits(int rs, int command);

void send_command_to_lcd_4_bits(int rs, int command);

void config_lcd(void);

void print_char_lcd(int c);
void go_to_start_lcd(void);

void print_number_1unit(int num);
void print_number_2units(int num);
void print_number_4units(int num);

#endif
