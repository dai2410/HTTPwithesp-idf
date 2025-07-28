#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

// Hàm khởi tạo và bắt đầu task đọc cảm biến
void dht_sensor_task_init(void);

// Hàm để các module khác có thể lấy giá trị nhiệt độ
float get_temperature(void);

// Hàm để các module khác có thể lấy giá trị độ ẩm
float get_humidity(void);

#endif // DHT_SENSOR_H