#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

// Bao gồm các module đã tạo
#include "wifi_manager.h"
#include "web_server.h"
#include "Led_control.h"
#include "dht_sensor.h"
#include "buzzer_control.h"
#include "HC-SR04_sensor.h"
#include "app_logic.h"

void app_main(void) {
    // Khởi tạo bộ nhớ flash NVS (cần cho WiFi)
    ESP_ERROR_CHECK(nvs_flash_init());

    // Khởi tạo GPIO cho LED
    led_control_init();
    
    // khởi tạo DHT11
    dht_sensor_task_init();

    buzzer_init();
    ultrasonic_sensor_init();

    app_logic_init();

    // Khởi tạo và kết nối WiFi
    wifi_manager_init_sta();


    // Khởi động máy chủ web
    web_server_start();


}