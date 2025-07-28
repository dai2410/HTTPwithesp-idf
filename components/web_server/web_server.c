#include "web_server.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "led_control.h"
#include "dht_sensor.h"
#include "app_logic.h"


static const char *TAG = "WEB_SERVER";
static httpd_handle_t server = NULL;

// SỬA ĐỔI CHÍNH: Thêm các dòng console.log() vào JavaScript
const char* html_index = 
"<!DOCTYPE html><html><head><title>ESP32 Control</title>"
"<meta charset=\"UTF-8\">"
"</head><body>"
"<h2>LED ESP32</h2>"
"<button onclick=\"fetch('/led/on')\">LED ON</button> "
"<button onclick=\"fetch('/led/off')\">LED OFF</button>"
"<h2>Sensor Data</h2>"
"<p>Temperature: <span id=\"temp\">--</span> °C</p>"
"<p>Humidity: <span id=\"hum\">--</span> %</p>"
"<p>People Count: <span id=\"count\">0</span></p>"
"<script>"
"console.log('Script loaded.');" // Ghi log khi script được tải
"function getSensorData() {"
"  console.log('Fetching sensor data...');" // Ghi log trước khi fetch
"  fetch('/api/data')"
"    .then(response => {"
"      console.log('Received response from server.');" // Ghi log khi nhận được phản hồi
"      if (!response.ok) {"
"        throw new Error('Network response was not ok ' + response.statusText);"
"      }"
"      return response.json();"
"    })"
"    .then(data => {"
"      console.log('Data parsed as JSON:', data);" // Ghi log dữ liệu JSON
"      var tempElement = document.getElementById('temp');"
"      var humElement = document.getElementById('hum');"
"      var countpeople = document.getElementById('count');"
"      if (tempElement && humElement && countpeople) {"
"        console.log('Updating DOM elements.');" // Ghi log trước khi cập nhật
"        tempElement.innerText = data.temperature.toFixed(1);"
"        humElement.innerText = data.humidity.toFixed(1);"
"        countpeople.innerText = data.count;"
"      } else {"
"        console.error('Error: Could not find temp or hum elements.');"
"      }"
"    })"
"    .catch(error => console.error('Error in fetch operation:', error));"
"}"
"document.addEventListener('DOMContentLoaded', function() {"
"  console.log('DOM fully loaded and parsed.');" // Ghi log khi DOM sẵn sàng
"  getSensorData();"
"  setInterval(getSensorData, 2000);"
"});"
"</script>"
"</body></html>";


// Xử lý HTTP GET: Trang chính
static esp_err_t index_get_handler(httpd_req_t *req) {
    httpd_resp_send(req, html_index, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Bật LED
static esp_err_t led_on_handler(httpd_req_t *req) {
    led_control_turn_on();
    ESP_LOGI(TAG, "LED turned ON");
    httpd_resp_sendstr(req, "LED ON");
    return ESP_OK;
}

// Tắt LED
static esp_err_t led_off_handler(httpd_req_t *req) {
    led_control_turn_off();
    ESP_LOGI(TAG, "LED turned OFF");
    httpd_resp_sendstr(req, "LED OFF");
    return ESP_OK;
}

//Lây dữ liệu  định dạng json rồi gửi lên webser
static esp_err_t dht_data_get_handler(httpd_req_t *req)
{
    char resp_str[150];
    float temp = get_temperature();
    float hum = get_humidity();
    int count = get_people_count();
    snprintf(resp_str, sizeof(resp_str), "{\"temperature\": %.1f, \"humidity\": %.1f, \"count\":%d}", temp, hum, count);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void web_server_start(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t uri_index = { .uri = "/", .method = HTTP_GET, .handler = index_get_handler };
        httpd_register_uri_handler(server, &uri_index);
        httpd_uri_t uri_led_on = { .uri = "/led/on", .method = HTTP_GET, .handler = led_on_handler };
        httpd_register_uri_handler(server, &uri_led_on);
        httpd_uri_t uri_led_off = { .uri = "/led/off", .method = HTTP_GET, .handler = led_off_handler };
        httpd_register_uri_handler(server, &uri_led_off);
        httpd_uri_t uri_dht_data = { .uri = "/api/data", .method = HTTP_GET, .handler = dht_data_get_handler };
        httpd_register_uri_handler(server, &uri_dht_data);
        ESP_LOGI(TAG, "Web server started");
    } else {
        ESP_LOGE(TAG, "Error starting web server!");
    }
}

void web_server_stop(void) {
    if (server) {
        httpd_stop(server);
        server = NULL;
    }
}