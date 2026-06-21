#include <Arduino.h>
#include <stdio.h>
#include <lvgl.h>
#include "ui/ui.h"
#include "ui_adapter.h"
#include "bike_data.h"
#include "ft85bd_uart.h"

// ============================================================
//  ЭКРАН 1: scrMain
// ============================================================

void ui_set_speed(float speed) {
    if(ui_lblSpeed) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%.0f", speed);
        lv_label_set_text(ui_lblSpeed, buf);
    }
}

void ui_set_speed_arc(float speed, float max_speed) {
    if(ui_arcSpeed) {
        int value = (int)((speed / max_speed) * 100);
        if(value > 100) value = 100;
        lv_arc_set_value(ui_arcSpeed, value);
    }
}

void ui_set_battery_bar(int percent) {
    if(ui_sldBatt) {
        lv_slider_set_value(ui_sldBatt, percent, LV_ANIM_OFF);
        lv_color_t color;
        if(percent < 20) color = lv_color_hex(0xFF0000);
        else if(percent < 50) color = lv_color_hex(0xFFA500);
        else color = lv_color_hex(0x00CC00);
        lv_obj_set_style_bg_color(ui_sldBatt, color, LV_PART_INDICATOR);
    }
}

void ui_set_battery_percent(int percent) {
    if(ui_lblBatt) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d%%", percent);
        lv_label_set_text(ui_lblBatt, buf);
    }
}

void ui_set_voltage(float voltage) {
    if(ui_lblVoltage) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%.1fV", voltage);
        lv_label_set_text(ui_lblVoltage, buf);
    }
}

void ui_set_power(float power) {
    if(ui_lblPower) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%.0fW", power);
        lv_label_set_text(ui_lblPower, buf);
    }
}

// ===== PAS ПЕРЕДАЧА (6 позиций: N, 1, 2, 3, 4, 5) =====
void ui_set_pas_gear(uint8_t gear) {
    if(ui_lblGear) {
        const char* names[] = {"N", "1", "2", "3", "4", "5"};
        if(gear <= 5) {
            lv_label_set_text(ui_lblGear, names[gear]);
        }
    }
}

void ui_set_pas(int pas) {
    if(ui_lblPAS) {
        char buf[8];
        snprintf(buf, sizeof(buf), "PAS %d", pas);
        lv_label_set_text(ui_lblPAS, buf);
    }
}

void ui_set_odo(uint32_t km) {
    if(ui_lblODO) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%.1f km", (float)km);
        lv_label_set_text(ui_lblODO, buf);
    }
}

void ui_set_trip(float km) {
    if(ui_lblTrip) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%.1f km", km);
        lv_label_set_text(ui_lblTrip, buf);
    }
}

void ui_set_avg_speed(float speed) {
    if(ui_lblAVG) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%.1f", speed);
        lv_label_set_text(ui_lblAVG, buf);
    }
}

void ui_set_max_speed(float speed) {
    if(ui_lblMAXspd) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%.1f", speed);
        lv_label_set_text(ui_lblMAXspd, buf);
    }
}

// ============================================================
//  КОНТЕЙНЕРЫ
// ============================================================

void ui_set_cruise_visible(bool visible) {
    if(ui_contCruise) {
        if(visible) {
            lv_obj_clear_flag(ui_contCruise, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(ui_contCruise, lv_color_hex(0x00AA00), 0);
        } else {
            lv_obj_add_flag(ui_contCruise, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void ui_set_brake_visible(bool visible) {
    if(ui_contBrake) {
        if(visible) {
            lv_obj_clear_flag(ui_contBrake, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(ui_contBrake, lv_color_hex(0xFF0000), 0);
        } else {
            lv_obj_add_flag(ui_contBrake, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void ui_set_bt_status(bool connected) {
    if(ui_contBT) {
        if(connected) {
            lv_obj_set_style_bg_color(ui_contBT, lv_color_hex(0x0088FF), 0);
        } else {
            lv_obj_set_style_bg_color(ui_contBT, lv_color_hex(0x444444), 0);
        }
    }
}

// ============================================================
//  ЭКРАН 2: scrSettings
// ============================================================

void ui_set_esc_status(bool online) {
    if(ui_lblESCstatus) {
        if(online) {
            lv_label_set_text(ui_lblESCstatus, "ESC: ONLINE");
            lv_obj_set_style_text_color(ui_lblESCstatus, lv_color_hex(0x00CC00), 0);
        } else {
            lv_label_set_text(ui_lblESCstatus, "ESC: OFFLINE");
            lv_obj_set_style_text_color(ui_lblESCstatus, lv_color_hex(0xFF0000), 0);
        }
    }
}

void ui_set_cpu_load(float load) {
    if(ui_lblCPU) {
        char buf[16];
        snprintf(buf, sizeof(buf), "CPU: %.1f%%", load);
        lv_label_set_text(ui_lblCPU, buf);
    }
}

void ui_set_mosfet_temp(float temp) {
    if(ui_lblMosfetTmp) {
        char buf[16];
        snprintf(buf, sizeof(buf), "FET: %.0f°C", temp);
        lv_label_set_text(ui_lblMosfetTmp, buf);
    }
}

void ui_set_error_code(uint8_t code) {
    if(ui_lblERR) {
        if(code == 0) {
            lv_label_set_text(ui_lblERR, "Err: 0 OK");
            lv_obj_set_style_text_color(ui_lblERR, lv_color_hex(0x00CC00), 0);
        } else {
            char buf[16];
            snprintf(buf, sizeof(buf), "Err: %d", code);
            lv_label_set_text(ui_lblERR, buf);
            lv_obj_set_style_text_color(ui_lblERR, lv_color_hex(0xFF0000), 0);
        }
    }
}

void ui_set_firmware_version(const char* version) {
    // Если добавите lblFW на экран настроек
    // if(ui_lblFW) lv_label_set_text(ui_lblFW, version);
}

// ============================================================
//  ОБРАБОТЧИКИ (вызываются из buttons.cpp)
// ============================================================

void ui_on_pas_gear_up(void) {
    if(bikeData.pas_gear < 5) {
        bikeData.pas_gear++;
    } else {
        bikeData.pas_gear = 0;
    }
    ui_set_pas_gear(bikeData.pas_gear);
    ft85bd_set_pas_gear(bikeData.pas_gear);
}

void ui_on_pas_gear_down(void) {
    if(bikeData.pas_gear > 0) {
        bikeData.pas_gear--;
    } else {
        bikeData.pas_gear = 5;
    }
    ui_set_pas_gear(bikeData.pas_gear);
    ft85bd_set_pas_gear(bikeData.pas_gear);
}

void ui_on_cruise_toggle(void) {
    bikeData.cruise_requested = !bikeData.cruise_requested;
    uint8_t payload[15] = {0};
    payload[0] = 0x02;
    payload[3] = 0x02;
    payload[4] = 0x00;
    payload[11] = 0x01;
    payload[12] = bikeData.cruise_requested ? 0x01 : 0x00;
    ft85bd_send_packet(payload, 15);
}