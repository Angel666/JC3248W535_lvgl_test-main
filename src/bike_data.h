#pragma once

#include <stdint.h>

struct BikeData
{
    // скорость

    float speed_kmh = 0.0f;
    float motor_rpm = 0.0f;

    // батарея

    float battery_voltage = 0.0f;
    float battery_current = 0.0f;
    float battery_power = 0.0f;
    int battery_percent = 0;
    float battery_energy_wh = 0.0f;
    float battery_energy_percent = 0.0f;

    // мотор

    float motor_current = 0.0f;

    // температуры

    float controller_temp = 0.0f;
    float motor_temp = 0.0f;

    // контроллер
    // Заполнение ШИМ (0...100%)
    float duty_cycle = 0.0f;

    // Загрузка CPU контроллера
    float cpu_load = 0.0f;

    // Угол энкодера
    float encoder_angle = 0.0f;

    // пользовательские

    int pas_level = 0;
    float pas_rpm = 0.0f;

    bool bluetooth_connected = false;

    // ошибки

    uint8_t fault_code = 0;

    // связь

    bool esc_online = false;
    uint32_t last_packet_ms = 0;

    // одометр

    uint32_t trip_distance_m = 0;
    uint32_t odometer_km = 0;

    bool moving = false;
    bool pedaling = false;
};

extern BikeData bikeData;