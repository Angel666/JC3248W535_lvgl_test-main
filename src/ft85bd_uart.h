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
};

extern FT85BD_Data esc_data;

void ft85bd_init();
void ft85bd_update();
void ft85bd_request_data();