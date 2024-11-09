#include "HD44780.h" // Chama código para controlar o LCD HD44780
#include <pic18fregs.h>
// Desabilita uso de instrucoes extendidas, não compativeis com PIC
#pragma config XINST = OFF

// Rotina de configuracao do ADC
void config_adc(void) {
  // Configura o ADC
  ADCON0bits.CHS = 0b0000;  // Canal 0
  ADCON2bits.ADFM = 1;      // Dado justificado para a direita
  ADCON2bits.ADCS = 0b111;  // Clock de conversao AD - do oscilador AD
  ADCON2bits.ACQT = 0b000;  // Tempo de aquisicao - 0 TAD
  ADCON1bits.PCFG = 0b1100; // Faz os pinos AN0, AN1 e AN2 serem analogicos, e
                            // os outros digitais
  ADCON1bits.VCFG1 = 1;     // Usa o pino em AN2 como referencia Vref- (1V)
  ADCON1bits.VCFG0 = 1;     // Usa o pino em AN1 como referencia Vref+ (1V)

  ADCON0bits.ADON = 1; // Liga o ADC
}
// Inicia conversao no ADC
void start_adc_conversion(void) { ADCON0bits.GO = 1; }
// Indica se uma conversao está sendo feita
int adc_is_busy(void) { return ADCON0bits.DONE; }
int read_upper_adc(void) { return ADRESH; } // Lê o valor superior do ADC
int read_lower_adc(void) { return ADRESL; } // Lê o valor inferior do ADC

void main(void) {
  config_lcd(); // Configura o LCD
  config_adc(); // Configura o ADC
  int adc_was_started = 0;
  while (1) {
    if (adc_is_busy() == 1) // Se o ADC estiver ocupado, esperar
      continue;
    // Se o ADC nao foi iniciado, comecar uma conversao
    if (adc_was_started == 0) {
      start_adc_conversion();
      adc_was_started = 1;
    } else { // Se uma conversao acabou, ler ela
      adc_was_started = 0;
      int val = ((read_upper_adc() << 8) | read_lower_adc());
      // As tensoes de referecia externa fazem com que o valor do ADC varie de 0
      // - 1000 com 1000 sendo 0C, e 0 100C, assim
      int temp = 1000 - val; // para fazer com que 0C = 0 e 100C = 1000
      if (temp < 0) // Caso o valor dê abaixo de zero, limitar ele em 0
        temp = 0;
      // Pela faixa de 100C - 0C ter 1000 níveis, dividir por 10 nos da as casas
      // das dezenas e unidades e módulo de 10 a primeira casa após a virgula
      print_number_2units(temp / 10);
      print_char_lcd('.');
      print_number_1unit(temp % 10);
      print_char_lcd(0b11011111); //°
      print_char_lcd('C');
      go_to_start_lcd(); // Retorna o cursor ao inicio para a próxima escrita
    }
  }
}
