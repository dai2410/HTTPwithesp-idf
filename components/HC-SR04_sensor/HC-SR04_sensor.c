#include "HC-SR04_sensor.h"
#include "hcsr04_driver.h" // Include header của thư viện
#include "esp_log.h"

static const char* TAG = "HCSR04_WRAPPER";

// Hàm khởi tạo của component "vỏ bọc"
void ultrasonic_sensor_init(void) {
    // Chỉ cần gọi hàm khởi tạo của thư viện
    // Thư viện sẽ tự động cấu hình các chân GPIO đã định nghĩa trong menuconfig
    esp_err_t res = UltrasonicInit();
    if (res == ESP_OK) {
        ESP_LOGI(TAG, "HC-SR04 sensor initialized successfully.");
    } else {
        ESP_LOGE(TAG, "Failed to initialize HC-SR04 sensor.");
    }
}

// Hàm đo khoảng cách của component "vỏ bọc"
esp_err_t ultrasonic_sensor_measure(float *distance) {
    if (!distance) {
        return ESP_ERR_INVALID_ARG;
    }
    
    // Thư viện này trả về khoảng cách trực tiếp bằng cm (dưới dạng uint32_t)
    uint32_t distance_cm = 0;
    
    // Gọi hàm đo của thư viện
    // Tham số đầu tiên là khoảng cách tối đa (cm) mà chúng ta muốn đo
    esp_err_t result = UltrasonicMeasure(400, &distance_cm);

    if (result == ESP_OK) {
        // Gán kết quả vào con trỏ float
        *distance = (float)distance_cm;
    } else {
        // Nếu có lỗi, gán giá trị không hợp lệ
        *distance = -1.0f;
        // Ghi lại lỗi để gỡ lỗi nếu cần
        UltrasonicAssert(result);
    }
    
    return result;
}