#include <cstdio>
#include <lvgl.h>

#include "dashboard.h"
#include "ui_adapter.h"

#include "bike_data.h"
#include "ui/ui.h"

void dashboard_update()
{
    // скорость

    ui_set_speed(
        bikeData.speed_kmh);

    // батарея

    ui_set_voltage(
        bikeData.battery_voltage);

    ui_set_current(
        bikeData.battery_current);

    ui_set_power(
        bikeData.battery_power);

    ui_set_battery_percent(
        bikeData.battery_percent);

    // PAS

    ui_set_pas(
        bikeData.pas_level);

    // температуры

    ui_set_temp(
        bikeData.controller_temp);

    ui_set_motor_temp(
        bikeData.motor_temp);

    // мотор

    ui_set_rpm(
        bikeData.motor_rpm);

    // ошибки

    ui_set_fault(
        bikeData.fault_code);

    // пробег

    ui_set_trip(
        bikeData.trip_distance_m / 1000.0f);

    ui_set_odo(
        bikeData.odometer_km);

    // статус контроллера

    ui_set_status(
        bikeData.esc_online);

    ui_set_moving(bikeData.moving);

    ui_set_pedaling(bikeData.pedaling);

     // ===== НОВЫЕ ВЫЗОВЫ ДЛЯ ВТОРОГО ЭКРАНА =====
    ui_set_firmware_version(bikeData.firmware_version);

    ui_set_speed_gear(bikeData.speed_limit_gear);

    ui_set_motor_direction(bikeData.motor_direction);

    ui_set_cruise_status(bikeData.cruise_enabled, bikeData.cruise_active);

    ui_set_multi_mode(bikeData.multi_mode_type);

    ui_set_cpu_load(bikeData.cpu_load);

    ui_set_encoder_angle(bikeData.encoder_angle);

    ui_set_pas_rpm(bikeData.pas_rpm);

    ui_set_error_code(bikeData.error_code);

    ui_set_controller_id(bikeData.controller_id);

    //BT status
    ui_set_ble_status(bikeData.ble_connected);

    // ===== ОТОБРАЖЕНИЕ СТАТУСА УПРАВЛЕНИЯ =====
    const char* gear_names[] = {"OFF", "LOW", "MED", "HIGH"};
    if(bikeData.current_gear <= 3) {
        lv_label_set_text(ui_lblGearDisplay, gear_names[bikeData.current_gear]);
    }

    // Статус круиза
    if(bikeData.cruise_active || bikeData.cruise_requested) {
        lv_label_set_text(ui_lblCruiseStatus, "CRUISE ON");
        lv_obj_set_style_text_color(ui_lblCruiseStatus, lv_color_hex(0x00FF00), 0);
    } else {
        lv_label_set_text(ui_lblCruiseStatus, "CRUISE OFF");
        lv_obj_set_style_text_color(ui_lblCruiseStatus, lv_color_hex(0x888888), 0);
    }

}