#include "ui.h"
#include "ui_adapter.h"

// ============================================================
//  ПЕРЕКЛЮЧЕНИЕ ЭКРАНОВ
// ============================================================

void ui_btnSettings_event(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_scrSettings, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
    }
}

void ui_btnBack_event(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_scrMain, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
    }
}