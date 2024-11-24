idf_component_register(SRCS "adc-i2c-esp32.c"
                    PRIV_REQUIRES esp_driver_i2c esp_adc
                    INCLUDE_DIRS ".")
