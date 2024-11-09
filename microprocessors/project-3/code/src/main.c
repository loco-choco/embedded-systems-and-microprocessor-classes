#include "HD44780.h"
#include <delay.h>
#include <pic18fregs.h>
#pragma config XINST = OFF

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

int read_upper_adc(void) { return ADRESH; }

int read_lower_adc(void) { return ADRESL; }

void main(void) {
  // No modo 4 bits, esses bits estao no GND no LCD
  TRISDbits.TRISD3 = 0;
  TRISDbits.TRISD2 = 0;
  TRISDbits.TRISD1 = 0;
  TRISDbits.TRISD0 = 0;
  LATDbits.LATD3 = 0;
  LATDbits.LATD2 = 0;
  LATDbits.LATD1 = 0;
  LATDbits.LATD0 = 0;

  config_lcd();
  config_adc();
  int adc_was_started = 0;
  while (1) {
    if (adc_is_busy() == 1)
      continue;
    if (adc_was_started == 0) {
      start_adc_conversion();
      adc_was_started = 1;
    } else {
      adc_was_started = 0;
      int val = ((read_upper_adc() << 8) | read_lower_adc());
      print_char_lcd('A');
      print_char_lcd('D');
      print_char_lcd('C');
      print_char_lcd(':');
      print_number_2units(val / 100);
      print_number_2units(val % 100);
      go_to_start_lcd();
    }
  }
}
