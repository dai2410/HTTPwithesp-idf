#include "web_server.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "led_control.h"
#include "dht_sensor.h"
#include "app_logic.h" 

static const char *TAG = "WEB_SERVER";
static httpd_handle_t server = NULL;

// Khai báo các biến con trỏ cho CẢ HAI tệp đã nhúng
extern const uint8_t _binary_index_html_start[] asm("_binary_index_html_start");
extern const uint8_t _binary_index_html_end[]   asm("_binary_index_html_end");


// Handler cho trang chính (/)
static esp_err_t index_get_handler(httpd_req_t *req) {
    const size_t html_size = (_binary_index_html_end - _binary_index_html_start);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char*)_binary_index_html_start, html_size);
    return ESP_OK;
}

// Handlers cho LED
static esp_err_t led_on_handler(httpd_req_t *req) {
    led_control_turn_on();
    httpd_resp_sendstr(req, "LED ON");
    ESP_LOGI(TAG, "LED ON");
    return ESP_OK;
}
static esp_err_t led_off_handler(httpd_req_t *req) {
    led_control_turn_off();
    httpd_resp_sendstr(req, "LED OFF");
    ESP_LOGI(TAG, "LED OFF");
    return ESP_OK;
}

// Handler cho API
static esp_err_t api_data_get_handler(httpd_req_t *req)
{
    char resp_str[150];
    float temp = get_temperature();
    float hum = get_humidity();
    int count = get_people_count();
    snprintf(resp_str, sizeof(resp_str), 
             "{\"temperature\": %.1f, \"humidity\": %.1f, \"count\": %d}", 
             temp, hum, count);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Hàm khởi tạo máy chủ web
void web_server_start(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server, &config) == ESP_OK) {
        
        // Đăng ký handler cho trang chính
        httpd_uri_t uri_index = { .uri = "/", .method = HTTP_GET, .handler = index_get_handler };
        httpd_register_uri_handler(server, &uri_index);

        // Đăng ký các handler còn lại
        httpd_uri_t uri_led_on = { .uri = "/led/on", .method = HTTP_GET, .handler = led_on_handler };
        httpd_register_uri_handler(server, &uri_led_on);
        httpd_uri_t uri_led_off = { .uri = "/led/off", .method = HTTP_GET, .handler = led_off_handler };
        httpd_register_uri_handler(server, &uri_led_off);
        httpd_uri_t uri_api_data = { .uri = "/api/data", .method = HTTP_GET, .handler = api_data_get_handler };
        httpd_register_uri_handler(server, &uri_api_data);
        
        ESP_LOGI(TAG, "Web server started");
    } else {
        ESP_LOGE(TAG, "Error starting web server!");
    }
}

// Hàm dừng máy chủ web
void web_server_stop(void) {
    if (server) {
        httpd_stop(server);
        server = NULL;
    }
}