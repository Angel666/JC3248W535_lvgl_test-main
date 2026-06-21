#pragma once

#include <driver/pcnt.h>

// Объявления функций (ОБЯЗАТЕЛЬНО!)
void speed_sensor_init();
void speed_sensor_update();
float speed_sensor_get_kmh();
void speed_sensor_reset();