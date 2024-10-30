#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define LED_PIN 2

void app_main(void) {
  // esp_rom_gpio_pad_select_gpio(LED_PIN);
  gpio_reset_pin(LED_PIN);
  gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
  uint8_t ON = 0;
  while (1) {
    ON = !ON;
    gpio_set_level(LED_PIN, ON);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
