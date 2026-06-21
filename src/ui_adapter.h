#pragma once

// ============================================================
//  ЭКРАН 1: scrMain
// ============================================================
void ui_set_speed(float speed);
void ui_set_speed_arc(float speed, float max_speed);
void ui_set_battery_bar(int percent);
void ui_set_battery_percent(int percent);
void ui_set_voltage(float voltage);
void ui_set_power(float power);
void ui_set_pas_gear(uint8_t gear);    // N, 1, 2, 3, 4, 5
void ui_set_pas(int pas);
void ui_set_odo(uint32_t km);
void ui_set_trip(float km);
void ui_set_avg_speed(float speed);
void ui_set_max_speed(float speed);

void ui_set_cruise_visible(bool visible);
void ui_set_brake_visible(bool visible);
void ui_set_bt_status(bool connected);

// ============================================================
//  ЭКРАН 2: scrSettings
// ============================================================
void ui_set_esc_status(bool online);
void ui_set_cpu_load(float load);
void ui_set_mosfet_temp(float temp);
void ui_set_error_code(uint8_t code);
void ui_set_firmware_version(const char* version);

// ============================================================
//  ОБРАБОТЧИКИ (для кнопок)
// ============================================================
void ui_on_pas_gear_up(void);
void ui_on_pas_gear_down(void);
void ui_on_cruise_toggle(void);