idf_component_register(SRCS "wifi_manager.c"
                    INCLUDE_DIRS "include"
                     REQUIRES esp_wifi 
                             esp_event 
                             log 
                             nvs_flash 
                             esp_netif)