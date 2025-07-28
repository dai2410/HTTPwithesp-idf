#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include "esp_err.h"

void ultrasonic_sensor_init(void);
esp_err_t ultrasonic_sensor_measure(float *distance);

#endif // ULTRASONIC_SENSOR_H