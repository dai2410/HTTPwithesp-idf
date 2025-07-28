#include "dht_sensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "dht.h" // Thư viện DHT vừa thêm

static const char *TAG = "DHT_SENSOR";

// Chọn loại cảm biến và chân GPIO
#define SENSOR_TYPE DHT_TYPE_DHT11
#define DHT_GPIO    4

// Biến toàn cục (static) để lưu trữ giá trị, dùng volatile vì được truy cập từ nhiều task
static volatile float temperature = 0.0f;
static volatile float humidity = 0.0f;

// Hàm getter để các module khác truy cập an toàn
float get_temperature(void) {
    return temperature;
}

float get_humidity(void) {
    return humidity;
}

// Đây là hàm của Task (luồng mới)
static void dht_sensor_task(void *pvParameters)
{
    ESP_LOGI(TAG, "DHT Sensor Task Started");

    // Vòng lặp vô tận của task
    for (;;)
    {
        float temp_reading, hum_reading;
        // Đọc dữ liệu từ cảm biến
        if (dht_read_float_data(SENSOR_TYPE, DHT_GPIO, &hum_reading, &temp_reading) == ESP_OK)
        {
            // Cập nhật giá trị vào biến toàn cục
            humidity = hum_reading;
            temperature = temp_reading;
            ESP_LOGI(TAG, "Humidity: %.1f%%, Temp: %.1fC", humidity, temperature);
        }
        else
        {
            ESP_LOGE(TAG, "Could not read data from sensor");
        }

        // Tạm dừng task trong 5 giây trước khi đọc lại
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// Hàm khởi tạo
void dht_sensor_task_init(void)
{
    // Tạo task mới
    xTaskCreate(dht_sensor_task, "dht_sensor_task", 2048, NULL, 5, NULL);
}