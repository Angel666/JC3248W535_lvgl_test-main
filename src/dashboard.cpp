#include <cstdio>
#include <lvgl.h>

#include "dashboard.h"
#include "ui_adapter.h"
#include "bike_data.h"
#include "ui/ui.h"
#include "speed_sensor.h"

void dashboard_update() {
    // ===== ОБНОВЛЕНИЕ СКОРОСТИ ОТ ДАТЧИКА ХОЛЛА =====
    speed_sensor_update();                     // Обновляем показания датчика
    bikeData.speed_sensor_kmh = speed_sensor_get_kmh();  // Сохраняем в bikeData
    
    // ===== ЭКРАН 1 =====
    // Используем скорость от датчика Холла (более точная)
    ui_set_speed(bikeData.speed_sensor_kmh);
    ui_set_speed_arc(bikeData.speed_sensor_kmh, 50.0f);  // max 50 km/h
    
    ui_set_battery_bar(bikeData.battery_percent);
    ui_set_battery_percent(bikeData.battery_percent);
    ui_set_voltage(bikeData.battery_voltage);
    ui_set_power(bikeData.battery_power);
    ui_set_pas_gear(bikeData.pas_gear);
    ui_set_pas(bikeData.pas_level);
    ui_set_odo(bikeData.odometer_km);
    ui_set_trip(bikeData.trip_distance_m / 1000.0f);
    ui_set_avg_speed(bikeData.avg_speed);
    ui_set_max_speed(bikeData.max_speed);
    
    ui_set_cruise_visible(bikeData.cruise_active);
    ui_set_brake_visible(bikeData.braking);
    ui_set_bt_status(bikeData.ble_connected);
    
    // ===== ЭКРАН 2 =====
    ui_set_esc_status(bikeData.esc_online);
    ui_set_cpu_load(bikeData.cpu_load);
    ui_set_mosfet_temp(bikeData.controller_temp);
    ui_set_error_code(bikeData.fault_code);
    ui_set_firmware_version(bikeData.firmware_version);
}