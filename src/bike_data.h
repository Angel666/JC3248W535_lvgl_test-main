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

    // ===== НОВЫЕ ПОЛЯ ДЛЯ ОТОБРАЖЕНИЯ =====
    char firmware_version[16];      // Версия прошивки ESC
    uint8_t speed_limit_gear;       // 0=нет,1=низ,2=сред,3=выс,4=реверс
    uint8_t motor_direction;        // 0=вперед, 1=назад
    bool cruise_enabled;            // Круиз разрешен
    bool cruise_active;             // Круиз активен
    uint8_t multi_mode_type;        // Тип мульти-режима
    float cpu_load;                 // Загрузка CPU (%)
    float encoder_angle;            // Угол энкодера (градусы)
    float pas_rpm_display;          // PAS RPM для отображения
    uint8_t error_code;             // Детальный код ошибки
    uint8_t controller_id;          // ID контроллера
};

extern BikeData bikeData;