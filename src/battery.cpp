#include "battery.h"
#include "bike_config.h"

float battery_voltage_to_percent(float voltage) {
    return VOLTAGE_TO_PERCENT(voltage);
}