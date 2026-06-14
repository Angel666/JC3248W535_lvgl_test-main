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

void ui_set_fault(int fault);

void ui_set_trip(float km);

void ui_set_odo(uint32_t km);

void ui_set_moving(bool moving);

void ui_set_pedaling(bool pedaling);

// ===== НОВЫЕ ФУНКЦИИ ДЛЯ РАСШИРЕННЫХ ДАННЫХ =====
void ui_set_firmware_version(const char* version);
void ui_set_speed_gear(uint8_t gear);
void ui_set_motor_direction(uint8_t direction);
void ui_set_cruise_status(bool enabled, bool active);
void ui_set_multi_mode(uint8_t mode);
void ui_set_cpu_load(float load);
void ui_set_encoder_angle(float angle);
void ui_set_pas_rpm(float rpm);
void ui_set_error_code(uint8_t error_code);
void ui_set_controller_id(uint8_t id);

// Функции для переключения экранов
void ui_switch_to_main_screen(void);
void ui_switch_to_info_screen(void);