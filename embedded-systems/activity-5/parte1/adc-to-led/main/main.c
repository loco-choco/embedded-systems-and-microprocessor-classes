#include "driver/dac_oneshot.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_check.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtc_wdt.h"
#include <stdio.h>
#include <stdlib.h>

#define ADC_POTENCIOMETRO_CANAL ADC_CHANNEL_9 // GPIO26, same as DAC channel 1
#define ADC_WIDTH ADC_WIDTH_BIT_12
#define MAX_VALUE 4096

SemaphoreHandle_t Sem_A;
int val_potenciometro;

static void adc_poten_input_task(void *args) {
  adc_oneshot_unit_handle_t handle = (adc_oneshot_unit_handle_t)args;
  while (1) {
    /* Read the DAC output voltage */
    int temp;
    ESP_ERROR_CHECK(adc_oneshot_read(handle, ADC_POTENCIOMETRO_CANAL, &temp));
    xSemaphoreTake(Sem_A, -1);
    val_potenciometro = temp;
    xSemaphoreGive(Sem_A);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

static void led_print_task(void *args) {
  int amount = 5;
  int led_pins[] = {19, 18, 5, 17, 16};

  for (int i = 0; i < amount; i++)
    gpio_set_direction(led_pins[i], GPIO_MODE_OUTPUT);
  while (1) {
    xSemaphoreTake(Sem_A, -1);
    int value = (val_potenciometro)*0.9;
    xSemaphoreGive(Sem_A);
    int i;
    for (i = 0; i < amount && (i < (value / (MAX_VALUE / (amount + 1)))); i++) {
      gpio_set_level(led_pins[i], 1);
    }
    while (i < amount) {
      gpio_set_level(led_pins[i], 0);
      i++;
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void app_main(void) {
  // Config do ADC para leitura do potenciometro
  Sem_A = xSemaphoreCreateMutex();

  adc_oneshot_unit_handle_t adc_poten_handle;
  adc_oneshot_unit_init_cfg_t adc_poten_config = {
      .unit_id = ADC_UNIT_2,
      .ulp_mode = ADC_ULP_MODE_DISABLE,
  };

  ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_poten_config, &adc_poten_handle));

  TaskHandle_t adc_poten_task_handle =
      xTaskCreatePinnedToCore(adc_poten_input_task, "potenciometro_input_task",
                              2048, adc_poten_handle, 5, NULL, 0);
  TaskHandle_t led_print_task_handle = xTaskCreatePinnedToCore(
      led_print_task, "led_print_task", 2048, NULL, 5, NULL, 0);
}
