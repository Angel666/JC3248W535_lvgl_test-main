#pragma once

#include "bike_config.h"

// Преобразование RPM в км/ч
static inline float rpm_to_kmh(float rpm) {
    // Длина окружности в метрах * RPM * 60 / 1000
    return rpm * WHEEL_CIRCUMFERENCE_M * 60.0f / 1000.0f;
}

// Преобразование км/ч в RPM
static inline float kmh_to_rpm(float kmh) {
    return kmh * 1000.0f / (WHEEL_CIRCUMFERENCE_M * 60.0f);
}

// Расчёт пройденного расстояния за время
static inline float distance_meters(float speed_kmh, float time_seconds) {
    return speed_kmh * 1000.0f / 3600.0f * time_seconds;
}