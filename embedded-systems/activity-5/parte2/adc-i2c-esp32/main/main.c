#include "driver/i2c_slave.h"
#include "driver/i2c_types.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

QueueHandle_t i2c_transmit_warn;

// Prepare a callback function
static bool i2c_slave_request_cb(i2c_slave_dev_handle_t channel,
                                 const i2c_slave_rx_done_event_data_t *edata,
                                 void *arg) {
  // Ao receber evento de pedido, coloca na queue
  printf("Novo pedido de I2C! (ISF)\n");
  i2c_rw_t evt = I2C_MASTER_READ;
  BaseType_t xTaskWoken = 0;
  xQueueSendFromISR(i2c_transmit_warn, &evt, &xTaskWoken);
  return xTaskWoken;
}

adc_oneshot_unit_handle_t adc_handle;
// Waiting for request event and send data in a task
static void i2c_slave_task(void *arg) {
  i2c_rw_t evt;
  i2c_slave_dev_handle_t handle = *(i2c_slave_dev_handle_t *)arg;
  int adc_val;

  while (1) {
    // Ao receber pedido de enviar dado
    if (xQueueReceive(i2c_transmit_warn, &evt, 10) == pdTRUE) {
      // Faz leitura do ADC
      printf("Novo pedido de I2C! (Task)\n");

      ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL_9, &adc_val));
      // Envia para quem pediu
      unsigned char adc_val_in_bytes[2];
      adc_val_in_bytes[0] = adc_val % 256;
      adc_val_in_bytes[1] = (adc_val >> 8) % 256;
      printf("Dado do ADC enviado pelo I2C!\n");
      ESP_ERROR_CHECK(i2c_slave_transmit(handle, adc_val_in_bytes,
                                         sizeof(unsigned char) * 2, 1000));
      printf("Dado do ADC enviado pelo I2C!\n");
    }
  }
  vTaskDelete(NULL);
}

void app_main(void) {
  printf("Ligado!\n");

  // Criar a Queue para passagem de eventos
  printf("Criando Queue...\n");
  i2c_transmit_warn = xQueueCreate(16, sizeof(i2c_rw_t));

  printf("Config de I2C como ''Slave''...\n");
  // Cria e configura ports de I2C ""Slave""
  i2c_slave_config_t i2c_slv_config = {
      .addr_bit_len = I2C_ADDR_BIT_LEN_7, // 7-bit address
      .clk_source = I2C_CLK_SRC_DEFAULT,  // set the clock source
      .i2c_port = 0,                      // set I2C port number
      .send_buf_depth = 256,              // set tx buffer length
      .scl_io_num = 2,                    // SCL gpio number
      .sda_io_num = 1,                    // SDA gpio number
      .slave_addr = 0x58,                 // slave address
  };

  i2c_slave_dev_handle_t slave_handle;
  printf("Criando I2C...\n");
  ESP_ERROR_CHECK(i2c_new_slave_device(&i2c_slv_config, &slave_handle));

  // Adiciona evento para receber pedido de aquisicao de dados
  printf("Adicionando Callback de pedido de dado...\n");
  i2c_slave_event_callbacks_t cbs = {
      .on_recv_done = i2c_slave_request_cb,
  };
  ESP_ERROR_CHECK(i2c_slave_register_event_callbacks(slave_handle, &cbs,
                                                     i2c_transmit_warn));
  // Cria o ADC
  printf("Criando ADC...\n");
  adc_oneshot_unit_init_cfg_t adc_config = {
      .unit_id = ADC_UNIT_2,
      .ulp_mode = ADC_ULP_MODE_DISABLE,
  };

  ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_config, &adc_handle));

  // Cria task para adquirir o dado e enviar
  printf("Criando TASK para ADC devolver os dados...\n");
  xTaskCreate(i2c_slave_task, "i2c_slave_task", 1024 * 4, &slave_handle, 10,
              NULL);
}
