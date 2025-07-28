#include "app_logic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Bao gồm các module chức năng mà nó cần
#include "buzzer_control.h"
#include "HC-SR04_sensor.h"

static const char *TAG = "APP_LOGIC";
#define DETECTION_THRESHOLD_CM 50

// Các biến trạng thái của ứng dụng được chuyển vào đây
static volatile int people_count = 0;
static volatile bool person_present = false;

// Hàm getter để các module khác gọi
int get_people_count(void) {
    return people_count;
}

// Luồng logic chính của ứng dụng (được chuyển từ main.c)
void app_logic_task(void *pvParameters) {
    ESP_LOGI(TAG, "Application Logic Task Started");
    while(1) {
        float distance;
        if (ultrasonic_sensor_measure(&distance) == ESP_OK) {
            // Logic đếm người
            if (distance < DETECTION_THRESHOLD_CM && !person_present) {
                person_present = true;
                people_count++;
                ESP_LOGI(TAG, "Person detected! Count: %d", people_count);
                buzzer_beep(200); // Kêu bíp 200ms
            } else if (distance >= DETECTION_THRESHOLD_CM && person_present) {
                person_present = false;
                ESP_LOGI(TAG, "Person left.");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

// Hàm khởi tạo của component này
void app_logic_init(void) {
    // Tạo luồng logic chính
    xTaskCreate(app_logic_task, "app_logic_task", 2048, NULL, 5, NULL);
}