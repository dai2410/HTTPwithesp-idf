idf_component_register(SRCS "web_server.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_http_server 
                             log 
                             led_control
                             dht_sensor
                             app_logic)

                             
# Nhúng file HTML
target_add_binary_data(
    __idf_web_server
    "web_content/index.html"
    TEXT
)


