/* Using LVGL with Arduino requires some extra steps:
   Be sure to read the docs here: https://docs.lvgl.io/master/details/integration/framework/arduino.html
   To use the built-in examples and demos of LVGL uncomment the includes below respectively.
   You also need to copy 'lvgl/examples' to 'lvgl/src/examples'. Similarly for the demos 'lvgl/demos' to 'lvgl/src/demos'.
*/

#include <lvgl.h>
#include <examples/lv_examples.h>
#include <ui/ui.h>
#include <demos/lv_demos.h>

#include "bike_data.h"
#include "dashboard.h"
#include "ft85bd_uart.h"
#include "storage.h"
#include "ui_adapter.h"
#include "ble_bridge.h"
#include "buttons.h"
#include "sd_config.h"
#include "speed_sensor.h"

#include <Arduino.h>
#include "pincfg.h"
#include "dispcfg.h"
#include "AXS15231B_touch.h"
#include <Arduino_GFX_Library.h>

// ============================================================
//  ДИСПЛЕЙ И СЕНСОР
// ============================================================

Arduino_DataBus *bus = new Arduino_ESP32QSPI(TFT_CS, TFT_SCK, TFT_SDA0, TFT_SDA1, TFT_SDA2, TFT_SDA3);
Arduino_GFX *g = new Arduino_AXS15231B(bus, GFX_NOT_DEFINED, 0, false, TFT_res_W, TFT_res_H);
Arduino_Canvas *gfx = new Arduino_Canvas(TFT_res_W, TFT_res_H, g, 0, 0, TFT_rot);
AXS15231B_Touch touch(Touch_SCL, Touch_SDA, Touch_INT, Touch_ADDR, TFT_rot);

// ============================================================
//  LVGL НАСТРОЙКИ
// ============================================================

#if LV_USE_LOG != 0
// Log to serial console
void my_print(lv_log_level_t level, const char *buf) {
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

// Callback so LVGL know the elapsed time
uint32_t millis_cb(void) {
    return millis();
}

// LVGL calls it when a rendered image needs to copied to the display
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);

    // Call it to tell LVGL everything is ready
    lv_disp_flush_ready(disp);
}

// Read the touchpad
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
    uint16_t x, y;
    if (touch.touched()) {
        // Read touched point from touch module
        touch.readData(&x, &y);

        // Set the coordinates
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// ============================================================
//  SETUP
// ============================================================

void setup() {
    #ifdef ARDUINO_USB_CDC_ON_BOOT
    delay(2000);
    #endif

    Serial.begin(115200);
    Serial.println("\n\n=== E-Bike Display Starting ===");
    
    // ===== 1. ЗАГРУЗКА ОДОМЕТРА =====
    storage_init();
    bikeData.odometer_km = storage_load_odometer();
    bikeData.trip_distance_m = 0;
    Serial.printf("Odometer loaded: %.1f km\n", (float)bikeData.odometer_km);

    // ===== 2. ИНИЦИАЛИЗАЦИЯ ДИСПЛЕЯ =====
    Serial.println("Arduino_GFX LVGL");
    String LVGL_Arduino = String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch() + " example";
    Serial.println(LVGL_Arduino);

    // Display setup
    if(!gfx->begin(40000000UL)) {
        Serial.println("Failed to initialize display!");
        return;
    }
    gfx->fillScreen(BLACK);

    // Switch backlight on
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    // Touch setup
    if(!touch.begin()) {
        Serial.println("Failed to initialize touch module!");
        return;
    }
    touch.enOffsetCorrection(true);
    touch.setOffsets(Touch_X_min, Touch_X_max, TFT_res_W-1, Touch_Y_min, Touch_Y_max, TFT_res_H-1);

    // ===== 3. ИНИЦИАЛИЗАЦИЯ LVGL =====
    lv_init();

    // Set a tick source so that LVGL will know how much time elapsed
    lv_tick_set_cb(millis_cb);

    // Register print function for debugging
    #if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print);
    #endif

    // Initialize the display buffer
    uint32_t screenWidth = gfx->width();
    uint32_t screenHeight = gfx->height();
    uint32_t bufSize = screenWidth * screenHeight / 10;
    lv_color_t *disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (!disp_draw_buf) {
        Serial.println("LVGL failed to allocate display buffer!");
        return;
    }

    // Initialize the display driver
    lv_display_t *disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Initialize the input device driver
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);

    // ===== 4. ЗАГРУЗКА UI (из SquareLine Studio) =====
    ui_init();
    Serial.println("UI loaded");

    // ===== 5. ИНИЦИАЛИЗАЦИЯ FT85BD UART =====
    ft85bd_init();
    Serial.println("FT85BD UART initialized");

    // ===== 6. ИНИЦИАЛИЗАЦИЯ ДАТЧИКА СКОРОСТИ (Холл) =====
    speed_sensor_init();
    Serial.println("Speed sensor initialized");

    // ===== 7. ИНИЦИАЛИЗАЦИЯ SD-КАРТЫ =====
    bool sd_ok = sd_init();

    // ===== 8. ЗАГРУЗКА НАСТРОЕК ПЕРЕДАЧ (PAS) =====
    bool loaded = false;

    if(sd_ok) {
        loaded = sd_load_gear_settings(bikeData.pas_gear_currents);
        if(loaded) {
            Serial.println("Gear settings loaded from SD card");
        } else {
            Serial.println("Using default gear settings");
        }
    } else {
        loaded = load_from_preferences(bikeData.pas_gear_currents);
        if(loaded) {
            Serial.println("Gear settings loaded from Preferences");
        }
    }

    // Дефолтные значения, если ничего не загрузилось
    if(!loaded) {
        bikeData.pas_gear_currents[0] = 5.0f;   // 1-я передача
        bikeData.pas_gear_currents[1] = 10.0f;  // 2-я передача
        bikeData.pas_gear_currents[2] = 15.0f;  // 3-я передача
        bikeData.pas_gear_currents[3] = 20.0f;  // 4-я передача
        bikeData.pas_gear_currents[4] = 25.0f;  // 5-я передача

        // Сохраняем дефолтные значения
        if(sd_ok) {
            sd_save_gear_settings(bikeData.pas_gear_currents);
        } else {
            save_to_preferences(bikeData.pas_gear_currents);
        }
        Serial.println("Default gear settings saved");
    }

    // ===== 9. ИНИЦИАЛИЗАЦИЯ КНОПОК =====
    buttons_init();
    Serial.println("Buttons initialized");

    // ===== 10. ИНИЦИАЛИЗАЦИЯ BLE =====
    ble_bridge_init();
    Serial.println("BLE bridge initialized");

    // ===== 11. НАЧАЛЬНЫЕ ЗНАЧЕНИЯ =====
    bikeData.pas_gear = 0;          // N (нейтраль)
    bikeData.cruise_requested = false;
    bikeData.ble_connected = false;
    bikeData.braking = false;
    bikeData.esc_online = false;

    // ===== 12. ЗАПРОС ВЕРСИИ ПРОШИВКИ =====
    ft85bd_request_firmware();

    Serial.println("=== Setup Complete ===");
}

// ============================================================
//  LOOP
// ============================================================

void loop() {
    // ===== 1. ОБНОВЛЕНИЕ UART (FT85BD) =====
    ft85bd_update();

    // ===== 2. ОБНОВЛЕНИЕ ДАННЫХ ДАШБОРДА =====
    dashboard_update();

    // ===== 3. ОБНОВЛЕНИЕ КНОПОК =====
    buttons_update();

    // ===== 4. ОБНОВЛЕНИЕ ДАТЧИКА СКОРОСТИ =====
    speed_sensor_update();

    // ===== 5. ОБНОВЛЕНИЕ BLE =====
    ble_bridge_update();

    // ===== 6. ОБНОВЛЕНИЕ LVGL =====
    lv_task_handler();

    // ===== 7. ОБНОВЛЕНИЕ ДИСПЛЕЯ (GFX) =====
    gfx->flush();
}