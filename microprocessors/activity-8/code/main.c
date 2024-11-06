#include <adc.h>
#include <pic18fregs.h>

#pragma config XINST = OFF

void config_lcd() {}
void print_string_lcd(char *str) {}

void main(void) {
  adc_open(ADC_CHN_0, ADC_FOSC_2 | ADC_ACQT_0 | ADC_CAL | ADC_TRIGGER,
           ADC_CFG_0A,
           ADC_FRM_LJUST | ADC_INT_OFF | ADC_VCFG_VDD_VSS | ADC_NVCFG_VSS);
  while (1) {
    if (adc_busy() == 0) {
      int val = adc_read();
      adc_conv();
    }
  }
}
