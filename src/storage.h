#pragma once

#include <stdint.h>

void storage_init();

void storage_save_odometer(
    uint32_t km);

uint32_t storage_load_odometer();