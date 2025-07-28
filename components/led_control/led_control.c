#include "led_control.h"
#include "driver/gpio.h"

#define LED_GPIO 2

void led_control_init(void) {
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
}

void led_control_turn_on(void) {
    gpio_set_level(LED_GPIO, 1);
}

void led_control_turn_off(void) {
    gpio_set_level(LED_GPIO, 0);
}