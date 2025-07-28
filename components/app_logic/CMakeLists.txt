idf_component_register(SRCS "app_logic.c"
                    INCLUDE_DIRS "include"
                    REQUIRES freertos 
                             log
                             buzzer_control
                             HC-SR04_sensor)