#pragma once

void ui_set_speed(float speed);

void ui_set_voltage(float voltage);

void ui_set_current(float current);

void ui_set_power(float power);

void ui_set_pas(int pas);

void ui_set_temp(float temp);

void ui_set_motor_temp(float temp);

void ui_set_battery_percent(int percent);

void ui_set_error(int error);

void ui_set_bluetooth(bool connected);

void ui_set_time(const char* text);

void ui_set_status(bool online);

void ui_set_rpm(float rpm);

void ui_set_motor_temp(float temp);

void ui_set_fault(int fault);

void ui_set_trip(float km);

void ui_set_odo(uint32_t km);

void ui_set_moving(bool moving);

void ui_set_pedaling(bool pedaling);