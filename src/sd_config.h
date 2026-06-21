#pragma once

#include <FS.h>          
#include <SdFat.h>       
#include <Preferences.h>
#include "bike_config.h"



// ===== ФУНКЦИИ =====
bool sd_init();
bool sd_load_gear_settings(float gear_currents[5]);
bool sd_save_gear_settings(float gear_currents[5]);
bool load_from_preferences(float gear_currents[5]);
bool save_to_preferences(float gear_currents[5]);
void sd_print_error();