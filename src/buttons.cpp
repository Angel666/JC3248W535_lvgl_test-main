#include <Arduino.h>
#include <driver/gpio.h>
#include "ft85bd_uart.h"
#include "bike_data.h"
#include "ui_adapter.h"
#include "bike_config.h"



static uint32_t last_press_up = 0;
static uint32_t last_press_down = 0;
static uint32_t press_start_down = 0;
static bool long_press_triggered = false;

void buttons_init(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUTTON_GEAR_UP_PIN) |
                        (1ULL << BUTTON_GEAR_DOWN_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    Serial.println("Buttons initialized");
}

// ===== ОТПРАВКА КОМАНДЫ PAS =====
static void send_pas_gear(void) {
    ft85bd_set_pas_gear(bikeData.pas_gear);
    ui_set_pas_gear(bikeData.pas_gear);  // Обновляем UI
}

// ===== ОТПРАВКА КОМАНДЫ КРУИЗА =====
static void send_cruise_command(bool enable) {
    uint8_t payload[15] = {0};
    payload[0] = 0x02;
    payload[3] = 0x02;   // throttle = 512 (нейтраль)
    payload[4] = 0x00;
    payload[11] = 0x01;  // Включить круиз
    payload[12] = enable ? 0x01 : 0x00;
    ft85bd_send_packet(payload, 15);
}

void buttons_update(void) {
    uint32_t now = millis();
    
    // ===== КНОПКА "+" (передача вверх) =====
    if(gpio_get_level((gpio_num_t)BUTTON_GEAR_UP_PIN) == 0 && (now - last_press_up > DEBOUNCE_MS)) {
        last_press_up = now;
        
        // Циклическое переключение: N→1→2→3→4→5→N
        if(bikeData.pas_gear < 5) {
            bikeData.pas_gear++;
        } else {
            bikeData.pas_gear = 0;
        }
        
        send_pas_gear();
        Serial.printf("PAS Gear: %d\n", bikeData.pas_gear);
    }
    
    // ===== КНОПКА "-" (передача вниз / круиз при удержании) =====
    bool down_pressed = (gpio_get_level((gpio_num_t)BUTTON_GEAR_DOWN_PIN) == 0);
    
    if(down_pressed && press_start_down == 0) {
        press_start_down = now;
        long_press_triggered = false;
    }
    else if(down_pressed && !long_press_triggered && (now - press_start_down > LONG_PRESS_MS)) {
        // Долгое нажатие (>500ms) — переключение круиза
        bikeData.cruise_requested = !bikeData.cruise_requested;
        send_cruise_command(bikeData.cruise_requested);
        Serial.printf("Cruise: %s\n", bikeData.cruise_requested ? "ON" : "OFF");
        long_press_triggered = true;
        last_press_down = now;
    }
    else if(!down_pressed && press_start_down != 0) {
        if(!long_press_triggered && (now - press_start_down > DEBOUNCE_MS)) {
            // Короткое нажатие — передача вниз
            if(bikeData.pas_gear > 0) {
                bikeData.pas_gear--;
            } else {
                bikeData.pas_gear = 5;  // N → 5
            }
            
            send_pas_gear();
            Serial.printf("PAS Gear: %d\n", bikeData.pas_gear);
            last_press_down = now;
        }
        press_start_down = 0;
    }
}