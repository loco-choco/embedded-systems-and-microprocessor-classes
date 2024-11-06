idf_component_register(SRCS "sine-wave-gen.c"
                    INCLUDE_DIRS "."
                  PRIV_REQUIRES esp_driver_dac esp_driver_gpio esp_driver_gptimer esp_adc)
