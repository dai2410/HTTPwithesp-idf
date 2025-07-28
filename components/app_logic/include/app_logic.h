#ifndef APP_LOGIC_H
#define APP_LOGIC_H

// Hàm để khởi tạo và bắt đầu luồng logic chính
void app_logic_init(void);

// Hàm để các module khác (web_server) có thể lấy số người đếm
int get_people_count(void);

#endif // APP_LOGIC_H