idf_component_register(SRCS "dht_sensor.c"
                    INCLUDE_DIRS "include"
                    REQUIRES freertos dht) # Yêu cầu freertos và component dht