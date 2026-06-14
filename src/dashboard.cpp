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
}