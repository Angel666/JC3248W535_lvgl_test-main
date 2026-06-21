#pragma once

#include <Arduino.h>

struct FT85BD_Data {
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
    
    // Новые поля из протокола V1.6
    uint8_t error_code;
    uint8_t direction_switching;
    uint8_t motor_direction;
    uint8_t speed_limit_gear;
    uint8_t cruise_enabled;
    uint8_t cruise_active;
    uint8_t multi_mode_enabled;
    uint8_t multi_mode_type;
    char firmware_version[16];
};

extern FT85BD_Data esc_data;

// ===== ОСНОВНЫЕ ФУНКЦИИ =====
void ft85bd_init();
void ft85bd_update();
void ft85bd_request_data();

// ===== ФУНКЦИИ ОТПРАВКИ =====
void ft85bd_send_packet(uint8_t *payload, uint8_t payload_len);
void ft85bd_request_extended_data();
void ft85bd_request_firmware();
void ft85bd_enable_auto_data(bool enable, uint16_t frequency_hz);

// ===== PAS-РЕЖИМ (НОВЫЙ) =====
void ft85bd_set_pas_gear(uint8_t gear);  // gear: 0=N, 1-5
