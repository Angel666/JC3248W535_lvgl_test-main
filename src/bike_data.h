#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    // ===== ОСНОВНЫЕ ДАННЫЕ =====
    float battery_voltage;
    float battery_current;
    float motor_current;
    float battery_power;
    float battery_energy_wh;
    int battery_percent;
    float battery_energy_percent;
    
    float motor_rpm;
    float pas_rpm;
    
    float controller_temp;
    float motor_temp;
    float duty_cycle;
    float cpu_load;
    float encoder_angle;
    
    uint32_t trip_distance_m;
    uint32_t odometer_km;
    
    uint8_t fault_code;
    uint8_t pas_level;

    float speed_kmh;           // Скорость от ESC (из RPM)
    float speed_sensor_kmh;    // Скорость от датчика Холла в моторе
    float avg_speed;           // Средняя скорость за поездку
    float max_speed;           // Максимальная скорость за поездку

     // Внутренние счётчики (для расчёта)
    float speed_sum;           // Сумма скоростей для среднего
    uint32_t speed_samples;    // Количество измерений
    float max_speed_recorded;  // Внутренний максимум
    
    bool moving;
    bool pedaling;
    bool esc_online;
    bool braking;              // Статус торможения
    bool cruise_active;        // Статус круиза из ESC
    bool cruise_requested;     // Запрос на круиз (для кнопки)
    
    uint32_t last_packet_ms;
    
    // ===== PAS ПЕРЕДАЧИ (6 позиций: N + 1-5) =====
    uint8_t pas_gear;          // 0=N, 1-5 = передачи
    float pas_gear_currents[5]; // Токи для передач 1-5 (А) — задаются в коде
    
    // ===== СТАТУС BLE =====
    bool ble_connected;
    
    // ===== ДОПОЛНИТЕЛЬНЫЕ ДАННЫЕ =====
    char firmware_version[16];
    uint8_t controller_id;
    uint8_t error_code;
    
    // ===== СТАТУС SD-КАРТЫ =====
    bool sd_mounted;
    bool settings_loaded;
} BikeData;

extern BikeData bikeData;