#include "battery.h"
#include "bike_config.h"

int battery_voltage_to_percent(float voltage)
{
    if(voltage >= BATTERY_FULL_VOLTAGE)
        return 100;

    if(voltage <= BATTERY_EMPTY_VOLTAGE)
        return 0;

    return (int)(
        (voltage - BATTERY_EMPTY_VOLTAGE)
        * 100.0f
        / (BATTERY_FULL_VOLTAGE - BATTERY_EMPTY_VOLTAGE)
    );
}