#include <stdio.h>

#include <lvgl.h>

#include "ui/ui.h"
#include "ui_adapter.h"

void ui_set_speed(float speed)
{
    char buf[16];

    snprintf(buf, sizeof(buf), "%.0f", speed);

    lv_label_set_text(ui_lblSpeed, buf);
}

void ui_set_voltage(float voltage)
{
    char buf[16];

    snprintf(buf, sizeof(buf), "%.1f V", voltage);

    lv_label_set_text(ui_lblVolt, buf);
}

void ui_set_current(float current)
{
    char buf[16];

    snprintf(buf, sizeof(buf), "%.1f A", current);

    lv_label_set_text(ui_lblCurrent, buf);
}

void ui_set_power(float power)
{
    char buf[16];

    snprintf(buf, sizeof(buf), "%.0f W", power);

    lv_label_set_text(ui_lblWatt, buf);
}

void ui_set_pas(int pas)
{
    char buf[16];

    snprintf(buf, sizeof(buf), "PAS %d", pas);

    lv_label_set_text(ui_lblPas, buf);
}

void ui_set_temp(float temp)
{
    char buf[16];

    snprintf(buf, sizeof(buf), "ESC %.0fC", temp);

    lv_label_set_text(ui_lblTemp, buf);
}

void ui_set_motor_temp(float temp)
{
    char buf[16];

    snprintf(buf,
             sizeof(buf),
             "MTR %.0fC",
             temp);

    lv_label_set_text(ui_lblMotorTmp, buf);
}

void ui_set_battery_percent(int percent)
{
    char buf[16];

    snprintf(buf,
             sizeof(buf),
             "%d%%",
             percent);

    lv_label_set_text(ui_lblBat, buf);
}

void ui_set_bluetooth(bool connected)
{
    if(connected)
        lv_label_set_text(ui_lblBT, "BT ON");
    else
        lv_label_set_text(ui_lblBT, "BT OFF");
}

void ui_set_time(const char* text)
{
    lv_label_set_text(ui_lblTime, text);
}

void ui_set_status(bool online)
{
    if(online)
        lv_label_set_text(ui_lblBT, "ESC OK");
    else
        lv_label_set_text(ui_lblBT, "ESC OFF");
}

void ui_set_rpm(float rpm)
{
    char buf[20];

    snprintf(buf,
             sizeof(buf),
             "%.0f RPM",
             rpm);

    lv_label_set_text(ui_lblRpm, buf);
}

void ui_set_fault(int fault)
{
    char buf[20];

    snprintf(buf,
             sizeof(buf),
             "ERR %d",
             fault);

    lv_label_set_text(ui_lblFault, buf);
}

void ui_set_trip(float km)
{
    char buf[20];

    snprintf(buf,
             sizeof(buf),
             "TRIP %.1f",
             km);

    lv_label_set_text(ui_lblTrip, buf);
}

void ui_set_odo(uint32_t km)
{
    char buf[20];

    snprintf(buf,
             sizeof(buf),
             "ODO %lu",
             (unsigned long)km);

    lv_label_set_text(ui_lblOdo, buf);
}

void ui_set_moving(bool moving)
{
    lv_label_set_text(ui_lblBT,
        moving ? "MOVING" : "STOP");
}

void ui_set_pedaling(bool pedaling)
{
    lv_label_set_text(ui_lblTime,
        pedaling ? "PEDAL" : "IDLE");
}

// ===== НОВЫЕ ФУНКЦИИ ДЛЯ РАСШИРЕННЫХ ДАННЫХ =====

// Версия прошивки
void ui_set_firmware_version(const char* version)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "FW: %s", version);
    
    // Обновляем на infoScreen (если виджет существует)
    if(ui_lblFirmware != NULL) {
        lv_label_set_text(ui_lblFirmware, buf);
    }
}

// Передача / ограничение скорости
void ui_set_speed_gear(uint8_t gear)
{
    const char* gear_text = "";
    switch(gear) {
        case 0: gear_text = "No limit"; break;
        case 1: gear_text = "Low"; break;
        case 2: gear_text = "Medium"; break;
        case 3: gear_text = "High"; break;
        case 4: gear_text = "Reverse"; break;
        default: gear_text = "Unknown"; break;
    }
    
    char buf[32];
    snprintf(buf, sizeof(buf), "Speed: %s", gear_text);
    
    if(ui_lblSpeedGear != NULL) {
        lv_label_set_text(ui_lblSpeedGear, buf);
    }
}

// Направление движения
void ui_set_motor_direction(uint8_t direction)
{
    const char* dir_text = (direction == 0) ? "→ FWD" : "← REV";
    char buf[16];
    snprintf(buf, sizeof(buf), "Dir: %s", dir_text);
    
    if(ui_lblDirection != NULL) {
        lv_label_set_text(ui_lblDirection, buf);
        
        // Меняем цвет для наглядности
        if(direction == 0) {
            lv_obj_set_style_text_color(ui_lblDirection, lv_color_hex(0x00AAFF), 0);
        } else {
            lv_obj_set_style_text_color(ui_lblDirection, lv_color_hex(0xFF5500), 0);
        }
    }
}

// Статус круиз-контроля
void ui_set_cruise_status(bool enabled, bool active)
{
    char buf[32];
    
    if(active) {
        snprintf(buf, sizeof(buf), "CRUISE ACTIVE");
        if(ui_lblCruise != NULL) {
            lv_label_set_text(ui_lblCruise, buf);
            lv_obj_set_style_text_color(ui_lblCruise, lv_color_hex(0x00FF00), 0);
        }
    } else if(enabled) {
        snprintf(buf, sizeof(buf), "Cruise: Ready");
        if(ui_lblCruise != NULL) {
            lv_label_set_text(ui_lblCruise, buf);
            lv_obj_set_style_text_color(ui_lblCruise, lv_color_hex(0x888888), 0);
        }
    } else {
        snprintf(buf, sizeof(buf), "Cruise: OFF");
        if(ui_lblCruise != NULL) {
            lv_label_set_text(ui_lblCruise, buf);
            lv_obj_set_style_text_color(ui_lblCruise, lv_color_hex(0x444444), 0);
        }
    }
}

// Мульти-режим
void ui_set_multi_mode(uint8_t mode)
{
    const char* mode_text = "Normal";
    switch(mode) {
        case 1: mode_text = "Tank U-turn"; break;
        case 2: mode_text = "Off-road"; break;
        default: mode_text = "Normal"; break;
    }
    
    char buf[32];
    snprintf(buf, sizeof(buf), "Mode: %s", mode_text);
    
    if(ui_lblMultiMode != NULL) {
        lv_label_set_text(ui_lblMultiMode, buf);
    }
}

// Загрузка CPU
void ui_set_cpu_load(float load)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "CPU: %.1f%%", load);
    
    if(ui_lblCpu != NULL) {
        lv_label_set_text(ui_lblCpu, buf);
    }
}

// Угол энкодера
void ui_set_encoder_angle(float angle)
{
    char buf[24];
    snprintf(buf, sizeof(buf), "Encoder: %.1f°", angle);
    
    if(ui_lblEncoder != NULL) {
        lv_label_set_text(ui_lblEncoder, buf);
    }
}

// PAS RPM
void ui_set_pas_rpm(float rpm)
{
    char buf[24];
    snprintf(buf, sizeof(buf), "PAS: %.0f RPM", rpm);
    
    if(ui_lblPasRpm != NULL) {
        lv_label_set_text(ui_lblPasRpm, buf);
    }
}

// Детальный код ошибки
void ui_set_error_code(uint8_t error_code)
{
    char buf[24];
    
    if(error_code > 0) {
        snprintf(buf, sizeof(buf), "Err code: %d", error_code);
        if(ui_lblErrorCode != NULL) {
            lv_label_set_text(ui_lblErrorCode, buf);
            lv_obj_set_style_text_color(ui_lblErrorCode, lv_color_hex(0xFF0000), 0);
        }
    } else {
        snprintf(buf, sizeof(buf), "Err code: 0");
        if(ui_lblErrorCode != NULL) {
            lv_label_set_text(ui_lblErrorCode, buf);
            lv_obj_set_style_text_color(ui_lblErrorCode, lv_color_hex(0x00AA00), 0);
        }
    }
}

// ID контроллера
void ui_set_controller_id(uint8_t id)
{
    char buf[24];
    snprintf(buf, sizeof(buf), "ESC ID: %d", id);
    
    if(ui_lblControllerId != NULL) {
        lv_label_set_text(ui_lblControllerId, buf);
    }
}

void ui_set_ble_status(bool connected)
{
    if (ui_lblBT == NULL) return;

    // Меняем цвет BT в зависимости от статуса
    lv_color_t color;
    if (connected) {
        color = lv_color_hex(0x00FF00); // Зеленый - подключен
    } else {
        color = lv_color_hex(0x888888); // Серый - нет подключения
    }
    lv_obj_set_style_bg_color(ui_lblBT, color, LV_PART_MAIN);
}

// ===== ФУНКЦИИ ПЕРЕКЛЮЧЕНИЯ ЭКРАНОВ =====

void ui_switch_to_main_screen(void)
{
    lv_scr_load_anim(ui_mainScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
}

void ui_switch_to_info_screen(void)
{
    lv_scr_load_anim(ui_infoScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
}