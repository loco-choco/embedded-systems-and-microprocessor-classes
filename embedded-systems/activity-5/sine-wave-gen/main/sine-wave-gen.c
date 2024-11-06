#include "driver/dac_oneshot.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_check.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtc_wdt.h"
#include <stdio.h>

#define ADC_POTENCIOMETRO_CANAL ADC_CHANNEL_9 // GPIO26, same as DAC channel 1
#define ADC_WIDTH ADC_WIDTH_BIT_12
#define LED_PIN 2

int val_potenciometro = 0;
static void adc_poten_input_task(void *args) {
  int val_potenciometro = 0;
  adc_oneshot_unit_handle_t handle = (adc_oneshot_unit_handle_t)args;
  while (1) {
    /* Read the DAC output voltage */
    ESP_ERROR_CHECK(
        adc_oneshot_read(handle, ADC_POTENCIOMETRO_CANAL, &val_potenciometro));
    printf("Potenciometro %4d\n", val_potenciometro);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

static void dac_output_task(void *args) {
  dac_oneshot_handle_t handle = (dac_oneshot_handle_t)args;
  uint32_t val = 0;
  while (1) {
    /* Set the voltage every 100 ms */
    ESP_ERROR_CHECK(dac_oneshot_output_voltage(handle, val));
    val += 10;
    val %= 250;
    vTaskDelay(pdMS_TO_TICKS((val_potenciometro * 200 / 4096)));
  }
}

void app_main(void) {
  //  rtc_wdt_disable();
  // Config do DAC para geracao de ondas
  dac_oneshot_handle_t chan0_handle;
  dac_oneshot_config_t chan0_cfg = {
      .chan_id = DAC_CHAN_0, // GPIO25 na ESP32
  };
  ESP_ERROR_CHECK(dac_oneshot_new_channel(&chan0_cfg, &chan0_handle));

  xTaskCreatePinnedToCore(dac_output_task, "dac_chan0_output_task", 2048,
                          chan0_handle, 5, NULL, 0);
  //----------------------------------
  // Config do ADC para leitura do potenciometro
  adc_oneshot_unit_handle_t adc_poten_handle;
  adc_oneshot_unit_init_cfg_t adc_poten_config = {
      .unit_id = ADC_UNIT_2,
      .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_poten_config, &adc_poten_handle));

  TaskHandle_t adc_poten_task_handle =
      xTaskCreatePinnedToCore(adc_poten_input_task, "potenciometro_input_task",
                              2048, adc_poten_handle, 5, NULL, 1);
  //----------------------------------
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
