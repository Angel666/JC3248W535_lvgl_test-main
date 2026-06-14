#pragma once

#include <Arduino.h>

struct FT85BD_Data
{
    uint8_t controller_id;
    uint8_t fault;

    float inpVoltage;
    float inputCurrent;
    float motorCurrent;

    float rpm;
    float dutyCycleNow;

    float temp_fet;
    float temp_motor;

    float cpu_load;
    float encoder_angle;

    float pas_rpm;

      // ===== НОВЫЕ ПОЛЯ =====
    uint8_t error_code;             // Детальный код ошибки
    uint8_t direction_switching;    // Разрешена смена направления
    uint8_t motor_direction;        // 0=вперед, 1=назад
    uint8_t speed_limit_gear;       // 0=нет,1=низ,2=сред,3=выс,4=реверс
    uint8_t cruise_enabled;         // Круиз разрешен
    uint8_t cruise_active;          // Круиз активен
    uint8_t multi_mode_enabled;     // Мульти-режим разрешен
    uint8_t multi_mode_type;        // Тип мульти-режима
    
    char firmware_version[16];      // Версия прошивки ESC
};

extern FT85BD_Data esc_data;

void ft85bd_init();
void ft85bd_update();
void ft85bd_request_data();