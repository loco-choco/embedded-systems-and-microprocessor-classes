#include "driver/ledc.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include <stdio.h>

// https://github.com/espressif/esp-idf/blob/v5.3.2/examples/peripherals/ledc/ledc_basic/main/ledc_basic_example_main.c
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY (5000) // Hz

#define R_CHANNEL LEDC_CHANNEL_0
#define LEDR_OUTPUT_IO (5)
#define G_CHANNEL LEDC_CHANNEL_1
#define LEDG_OUTPUT_IO (6)
#define B_CHANNEL LEDC_CHANNEL_2
#define LEDB_OUTPUT_IO (7)

void app_main(void) {
  uart_set_baudrate(UART_NUM_0, 115200); // Baudrate da UART
  // Configura o TIMER do PWM
  ledc_timer_config_t ledc_timer = {.speed_mode = LEDC_MODE,
                                    .duty_resolution = LEDC_DUTY_RES,
                                    .timer_num = LEDC_TIMER,
                                    .freq_hz = LEDC_FREQUENCY,
                                    .clk_cfg = LEDC_AUTO_CLK};
  ledc_timer_config(&ledc_timer);
  // Configuracao BASE de cada pino
  ledc_channel_config_t ledc_channel = {.speed_mode = LEDC_MODE,
                                        .timer_sel = LEDC_TIMER,
                                        .intr_type = LEDC_INTR_DISABLE,
                                        .duty = 0,
                                        .hpoint = 0};
  // R
  ledc_channel.gpio_num = LEDR_OUTPUT_IO;
  ledc_channel.channel = R_CHANNEL;
  ledc_channel_config(&ledc_channel);
  // G
  ledc_channel.gpio_num = LEDG_OUTPUT_IO;
  ledc_channel.channel = G_CHANNEL;
  ledc_channel_config(&ledc_channel);
  // B
  ledc_channel.gpio_num = LEDB_OUTPUT_IO;
  ledc_channel.channel = B_CHANNEL;
  ledc_channel_config(&ledc_channel);

  // Loop do Programa
  int incremento = 5;
  int r, g, b;
  r = 0;
  g = 0;
  b = 0;
  while (1) {
    // R
    ledc_set_duty(LEDC_MODE, R_CHANNEL, r);
    ledc_update_duty(LEDC_MODE, R_CHANNEL); // Atualiza o valor do PWM
    // G
    ledc_set_duty(LEDC_MODE, G_CHANNEL, g);
    ledc_update_duty(LEDC_MODE, G_CHANNEL); // Atualiza o valor do PWM
    // B
    ledc_set_duty(LEDC_MODE, B_CHANNEL, b);
    ledc_update_duty(LEDC_MODE, B_CHANNEL); // Atualiza o valor do PWM
    printf("Inc: %d, (R=%d, G=%d, B=%d)\n", incremento, r, g, b);
    r = (r + 2 * incremento) % 255;
    g = (g + incremento) % 255;
    b = (b + 3 * incremento) % 255;
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
