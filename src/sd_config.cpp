#include "sd_config.h"
#include "bike_data.h"
#include <Arduino.h>

// ===== ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ =====
static bool sd_initialized = false;
SdFat SD;

// ===== ИНИЦИАЛИЗАЦИЯ SD-КАРТЫ =====
bool sd_init()
{
    SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
    
    if(!SD.begin(SD_CS_PIN, SD_SCK_MHZ(4))) {
        Serial.println("SD Card Mount Failed");
        sd_initialized = false;
        return false;
    }
    
    sd_initialized = true;
    Serial.println("SD Card initialized successfully");
    return true;
}

// ===== ЗАГРУЗКА НАСТРОЕК =====
bool sd_load_gear_settings(float gear_currents[5])
{
    if(!sd_initialized) {
        Serial.println("SD Card not initialized, trying Preferences...");
        return load_from_preferences(gear_currents);
    }
    
    SdFile file;
    if(!file.open("/config.ini", O_READ)) {
        Serial.println("Failed to open config.ini for reading");
        return false;
    }
    
    String line;
    int loaded_count = 0;
    char buf[64];
    
    while(file.fgets(buf, sizeof(buf)) && loaded_count < 5) {
        line = String(buf);
        line.trim();
        
        if(line.startsWith("gear") && line.indexOf('=') > 0) {
            int pos = line.indexOf('=');
            if(pos > 0) {
                String key = line.substring(0, pos);
                String value = line.substring(pos + 1);
                int num = key.substring(4).toInt();
                if(num >= 1 && num <= 5) {
                    gear_currents[num - 1] = value.toFloat();
                    Serial.printf("Loaded gear%d_current = %.1fA\n", num, gear_currents[num - 1]);
                    loaded_count++;
                }
            }
        }
    }
    
    file.close();
    
    if(loaded_count == 5) {
        Serial.println("All gear settings loaded from SD card");
        return true;
    } else {
        Serial.println("Not all gear settings found on SD card");
        return false;
    }
}

// ===== СОХРАНЕНИЕ НАСТРОЕК =====
bool sd_save_gear_settings(float gear_currents[5])
{
    if(!sd_initialized) {
        Serial.println("SD Card not initialized, saving to Preferences...");
        return save_to_preferences(gear_currents);
    }
    
    if(SD.exists("/config.ini")) {
        SD.remove("/config.ini");
    }
    
    SdFile file;
    if(!file.open("/config.ini", O_CREAT | O_WRITE)) {
        Serial.println("Failed to open config.ini for writing");
        return false;
    }
    
    file.println("# ESC GEAR SETTINGS (PAS MODE)");
    file.println("# PAS Gear: N, 1, 2, 3, 4, 5");
    file.println("# Format: gearN=CURRENT (Amps)");
    file.println();
    file.printf("gear1_current=%.1f\n", gear_currents[0]);
    file.printf("gear2_current=%.1f\n", gear_currents[1]);
    file.printf("gear3_current=%.1f\n", gear_currents[2]);
    file.printf("gear4_current=%.1f\n", gear_currents[3]);
    file.printf("gear5_current=%.1f\n", gear_currents[4]);
    file.println();
    file.println("# Gear names:");
    file.println("# 1 = 1st gear");
    file.println("# 2 = 2nd gear");
    file.println("# 3 = 3rd gear");
    file.println("# 4 = 4th gear");
    file.println("# 5 = 5th gear");
    
    file.close();
    Serial.println("Gear settings saved to SD card");
    return true;
}

// ===== ЗАГРУЗКА ИЗ PREFERENCES =====
bool load_from_preferences(float gear_currents[5])
{
    Preferences preferences;
    preferences.begin("esc_settings", true);
    
    bool loaded = false;
    for(int i = 0; i < 5; i++) {
        String key = "gear" + String(i + 1) + "_current";
        if(preferences.isKey(key.c_str())) {
            gear_currents[i] = preferences.getFloat(key.c_str(), 0.0f);
            loaded = true;
        }
    }
    
    preferences.end();
    
    if(loaded) {
        Serial.println("Gear settings loaded from Preferences");
    } else {
        Serial.println("No gear settings found in Preferences");
    }
    return loaded;
}

// ===== СОХРАНЕНИЕ В PREFERENCES =====
bool save_to_preferences(float gear_currents[5])
{
    Preferences preferences;
    preferences.begin("esc_settings", false);
    
    for(int i = 0; i < 5; i++) {
        String key = "gear" + String(i + 1) + "_current";
        preferences.putFloat(key.c_str(), gear_currents[i]);
        Serial.printf("Saved gear%d_current = %.1fA to Preferences\n", i + 1, gear_currents[i]);
    }
    
    preferences.end();
    Serial.println("Gear settings saved to Preferences");
    return true;
}

// ===== ОТЛАДОЧНАЯ ФУНКЦИЯ (ИСПРАВЛЕННАЯ) =====
void sd_print_error()
{
    if(!sd_initialized) {
        Serial.println("SD Card not initialized");
        return;
    }
    
    SdFile root;
    if(!root.open("/", O_READ)) {
        Serial.println("Failed to open root directory");
        return;
    }
    
    Serial.println("Files on SD card:");
    SdFile file;
    char filename[64];
    while(file.openNext(&root, O_READ)) {
        // getName() с буфером
        file.getName(filename, sizeof(filename));
        Serial.printf("  %s, size: %llu bytes\n", filename, (unsigned long long)file.fileSize());
        file.close();
    }
    root.close();
}