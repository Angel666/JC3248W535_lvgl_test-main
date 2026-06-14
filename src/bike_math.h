#pragma once

#include "bike_config.h"

inline float rpm_to_kmh(float rpm)
{
    return rpm * WHEEL_CIRCUMFERENCE * 60.0f / 1000.0f;
}