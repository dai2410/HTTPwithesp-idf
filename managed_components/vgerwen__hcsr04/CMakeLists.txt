set(req esp_driver_gpio esp_timer freertos)

idf_component_register(
        SRCS "src/hcsr04_driver.c"
        INCLUDE_DIRS "include"
        PRIV_REQUIRES ${req})
