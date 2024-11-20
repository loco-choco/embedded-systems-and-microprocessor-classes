idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                  PRIV_REQUIRES esp_driver_gpio esp_driver_gptimer esp_adc driver nvs_flash spi_flash)
