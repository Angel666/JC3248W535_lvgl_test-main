// speed_sensor.cpp
#include "bike_config.h"
#include "speed_sensor.h"
#include <Arduino.h>
#include <driver/pcnt.h>

#define PCNT_UNIT           PCNT_UNIT_0
#define PCNT_FILTER_VALUE   100
#define MEASUREMENT_INTERVAL_MS 100  // ← ОПРЕДЕЛЕНО КАК МАКРОС

static volatile int32_t pulse_count = 0;
static volatile bool overflow_occurred = false;
static float current_speed_kmh = 0;

static void IRAM_ATTR pcnt_overflow_handler(void *arg) {
    overflow_occurred = true;
}

void speed_sensor_init() {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << SPEED_SENSOR_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    
    pcnt_config_t pcnt_config = {
        .pulse_gpio_num = SPEED_SENSOR_PIN,
        .ctrl_gpio_num = PCNT_PIN_NOT_USED,
        .lctrl_mode = PCNT_MODE_KEEP,
        .hctrl_mode = PCNT_MODE_KEEP,
        .pos_mode = PCNT_COUNT_INC,
        .neg_mode = PCNT_COUNT_DIS,
        .counter_h_lim = 3000,
        .counter_l_lim = 0,
        .unit = PCNT_UNIT,
        .channel = PCNT_CHANNEL_0
    };
    pcnt_unit_config(&pcnt_config);
    
    pcnt_set_filter_value(PCNT_UNIT, PCNT_FILTER_VALUE);
    pcnt_filter_enable(PCNT_UNIT);
    
    pcnt_isr_service_install(0);
    pcnt_isr_handler_add(PCNT_UNIT, pcnt_overflow_handler, NULL);
    pcnt_intr_enable(PCNT_UNIT);
    
    pcnt_counter_pause(PCNT_UNIT);
    pcnt_counter_clear(PCNT_UNIT);
    pcnt_counter_resume(PCNT_UNIT);
    
    Serial.println("Speed sensor initialized");
}

void speed_sensor_update() {
    static uint32_t last_time = 0;
    uint32_t now = millis();
    
    if(now - last_time < MEASUREMENT_INTERVAL_MS) {  // ← Используем макрос
        return;
    }
    last_time = now;
    
    int16_t count = 0;
    pcnt_counter_pause(PCNT_UNIT);
    pcnt_get_counter_value(PCNT_UNIT, &count);
    pcnt_counter_clear(PCNT_UNIT);
    pcnt_counter_resume(PCNT_UNIT);
    
    if(overflow_occurred) {
        overflow_occurred = false;
        return;
    }
    
    // Используем константу напрямую, а не создаём переменную
    float revolutions = (float)count / MAGNETS_COUNT;
    float rps = revolutions / (MEASUREMENT_INTERVAL_MS / 1000.0f);  // ← Прямой расчёт
    float speed_mps = rps * WHEEL_CIRCUMFERENCE_M;
    current_speed_kmh = speed_mps * 3.6f;
    
    if(current_speed_kmh > 120.0f) {
        current_speed_kmh = 0;
    }
}

float speed_sensor_get_kmh() {
    return current_speed_kmh;
}

void speed_sensor_reset() {
    pcnt_counter_pause(PCNT_UNIT);
    pcnt_counter_clear(PCNT_UNIT);
    pcnt_counter_resume(PCNT_UNIT);
    current_speed_kmh = 0;
}