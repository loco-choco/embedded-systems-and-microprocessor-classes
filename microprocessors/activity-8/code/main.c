#include <pic18fregs.h>

#pragma config XINST = OFF

#define LED_LAT LATDbits.LATD0
#define LED_TRIS TRISDbits.TRISD0

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
  LED_TRIS = 0;
  LED_LAT = 0;
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
      int adc_upper = read_upper_adc();
      int adc_lower = read_lower_adc();
      if (adc_upper > 256 / 2) {
        LED_LAT = 1;

      } else {
        LED_LAT = 0;
      }
    }
  }
}
