idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "." 
                    REQUIRES nvs_flash 
                             freertos
                             wifi_manager
                             web_server
                             led_control
                             dht_sensor
                             buzzer_control
                             HC-SR04_sensor
                             app_logic)