#include <Arduino.h>
#include <Preferences.h>

#include "storage.h"

static Preferences prefs;

void storage_init()
{
    prefs.begin(
        "ft85bd",
        false
    );
}

void storage_save_odometer(
    uint32_t km)
{
    prefs.putULong(
        "odo",
        km
    );
}

uint32_t storage_load_odometer()
{
    return prefs.getULong(
        "odo",
        0
    );
}