//FT85BD TX  ---> GPIO16 (RX)
//FT85BD RX  ---> GPIO17 (TX)
//FT85BD GND ---> GND

#include <Arduino.h>
#include "bike_config.h"
#include "ft85bd_uart.h"
#include "bike_data.h"
#include "bike_math.h"
#include "battery.h"
#include "storage.h"

HardwareSerial FT85BD(1);

FT85BD_Data esc_data;



static uint32_t lastRequest = 0;

// ========== 1. CRC ФУНКЦИЯ ==========
static uint16_t crc16_ftesc(uint8_t *buf, uint16_t len) {
    uint16_t crc = 0xFFFF;
    while(len--) {
        crc ^= *buf++;
        for(uint8_t i = 0; i < 8; i++) {
            if(crc & 1) crc = (crc >> 1) ^ 0xA001;
            else crc >>= 1;
        }
    }
    return crc;
}

// ========== 2. ФУНКЦИИ ЧТЕНИЯ ДАННЫХ ==========
static int16_t read_i16(const uint8_t *data, int &index) {
    int16_t v = ((uint16_t)data[index] << 8) | data[index + 1];
    index += 2;
    return v;
}

static int32_t read_i32(const uint8_t *data, int &index) {
    int32_t v = ((uint32_t)data[index] << 24) |
                ((uint32_t)data[index+1] << 16) |
                ((uint32_t)data[index+2] << 8) |
                data[index+3];
    index += 4;
    return v;
}

static float read_f16(const uint8_t *data, float scale, int &index) {
    return (float)read_i16(data, index) / scale;
}

static float read_f32(const uint8_t *data, float scale, int &index) {
    return (float)read_i32(data, index) / scale;
}

// ========== 3. ПАРСЕРЫ ОТВЕТОВ ==========

// Парсер базового ответа (команда 0x00)
static void parse_realtime_packet(uint8_t *data) {
    int index = 1;
    esc_data.controller_id = data[index++];
    esc_data.fault = data[index++];
    esc_data.inpVoltage = read_f16(data, 100.0f, index);
    esc_data.inputCurrent = read_f32(data, 1000000.0f, index);
    esc_data.motorCurrent = read_f32(data, 1000000.0f, index);
    esc_data.rpm = read_f32(data, 1.0f, index);
    esc_data.dutyCycleNow = read_f16(data, 10000.0f, index);
    esc_data.temp_fet = read_f16(data, 100.0f, index);
    esc_data.temp_motor = read_f16(data, 100.0f, index);
    esc_data.cpu_load = read_f16(data, 10000.0f, index);
    esc_data.encoder_angle = read_f32(data, 1000000.0f, index);
    esc_data.pas_rpm = read_f32(data, 1.0f, index);
}

// Парсер расширенного ответа (команда 0x02 — UART/PAS режим)
static void parse_extended_packet(uint8_t *data) {
    int index = 1;
    esc_data.error_code = data[index++];
    esc_data.inpVoltage = read_f16(data, 100.0f, index);
    esc_data.inputCurrent = read_f32(data, 1000000.0f, index);
    esc_data.motorCurrent = read_f32(data, 1000000.0f, index);
    esc_data.rpm = read_f32(data, 1.0f, index);
    esc_data.dutyCycleNow = read_f16(data, 10000.0f, index);
    esc_data.temp_fet = read_f16(data, 100.0f, index);
    esc_data.temp_motor = read_f16(data, 100.0f, index);
    esc_data.cpu_load = read_f16(data, 10000.0f, index);
    esc_data.encoder_angle = read_f32(data, 1000000.0f, index);
    
    // Новые поля (из документации V1.6)
    esc_data.direction_switching = data[index++];
    esc_data.motor_direction = data[index++];
    esc_data.speed_limit_gear = data[index++];
    index += 3;  // Пропускаем D7 (horn), D8 (light), D9 (brake light)
    esc_data.cruise_enabled = data[index++];
    esc_data.cruise_active = data[index++];
    esc_data.multi_mode_enabled = data[index++];
    esc_data.multi_mode_type = data[index++];
}

// Парсер версии прошивки (команда 0x11)
static void parse_firmware_packet(uint8_t *data) {
    int index = 1;
    uint8_t major = data[index++];
    uint8_t minor = data[index++];
    snprintf(esc_data.firmware_version, sizeof(esc_data.firmware_version), "%d.%d", major, minor);
    Serial.printf("ESC Firmware: %s\n", esc_data.firmware_version);
}

// ========== 4. ОБНОВЛЕНИЕ ДАННЫХ ==========
static uint32_t lastDistanceUpdate = 0;
static uint32_t lastEnergyUpdate = 0;
static uint32_t lastSavedOdoKm = 0;

static void update_bike_data() {
    bikeData.battery_voltage = esc_data.inpVoltage;
    bikeData.battery_current = esc_data.inputCurrent;
    bikeData.motor_current = esc_data.motorCurrent;
    bikeData.battery_power = esc_data.inpVoltage * esc_data.inputCurrent;

    // Подсчёт потреблённой энергии (Wh)
    uint32_t nowEnergy = millis();
    if(lastEnergyUpdate != 0) {
        float dtHours = (nowEnergy - lastEnergyUpdate) / 3600000.0f;
        bikeData.battery_energy_wh += bikeData.battery_power * dtHours;
    }
    lastEnergyUpdate = nowEnergy;

    bikeData.battery_percent = battery_voltage_to_percent(esc_data.inpVoltage);
    bikeData.battery_energy_percent = 100.0f - (bikeData.battery_energy_wh / BATTERY_CAPACITY_WH * 100.0f);
    if(bikeData.battery_energy_percent < 0) bikeData.battery_energy_percent = 0;

    bikeData.motor_rpm = esc_data.rpm;
    bikeData.pas_rpm = esc_data.pas_rpm;
    bikeData.moving = bikeData.speed_kmh > 1.0f;
    bikeData.pedaling = bikeData.pas_rpm > 5.0f;
    bikeData.speed_kmh = rpm_to_kmh(esc_data.rpm);

    // Расчёт пройденного расстояния
    uint32_t now = millis();
    if(lastDistanceUpdate != 0) {
        float dt = (now - lastDistanceUpdate) / 1000.0f;
        float meters = bikeData.speed_kmh * 1000.0f / 3600.0f * dt;
        bikeData.trip_distance_m += (uint32_t)meters;
        bikeData.odometer_km = storage_load_odometer() + bikeData.trip_distance_m / 1000;
        if(bikeData.odometer_km >= lastSavedOdoKm + 1) {
            storage_save_odometer(bikeData.odometer_km);
            lastSavedOdoKm = bikeData.odometer_km;
        }
    }
    lastDistanceUpdate = now;

    bikeData.controller_temp = esc_data.temp_fet;
    bikeData.motor_temp = esc_data.temp_motor;
    bikeData.duty_cycle = esc_data.dutyCycleNow;
    bikeData.cpu_load = esc_data.cpu_load;
    bikeData.encoder_angle = esc_data.encoder_angle;
    bikeData.fault_code = esc_data.fault;
    bikeData.last_packet_ms = millis();
    bikeData.esc_online = true;

    // Новые поля
    bikeData.cruise_active = esc_data.cruise_active;
    bikeData.error_code = esc_data.error_code;
    bikeData.controller_id = esc_data.controller_id;
    snprintf(bikeData.firmware_version, sizeof(bikeData.firmware_version), "%s", esc_data.firmware_version);
}

// ========== 5. ФУНКЦИЯ ОТПРАВКИ ПАКЕТА ==========
void ft85bd_send_packet(uint8_t *payload, uint8_t payload_len) {
    uint8_t packet[256];
    int idx = 0;
    packet[idx++] = 0xAA;
    packet[idx++] = payload_len;
    memcpy(&packet[idx], payload, payload_len);
    idx += payload_len;
    uint16_t crc = crc16_ftesc(payload, payload_len);
    packet[idx++] = (crc >> 8) & 0xFF;
    packet[idx++] = crc & 0xFF;
    packet[idx++] = 0xDD;
    FT85BD.write(packet, idx);
}

// ========== 6. ОСНОВНЫЕ ФУНКЦИИ ==========
void ft85bd_init() {
    FT85BD.begin(FT85BD_BAUDRATE, SERIAL_8N1, FT85BD_RX_PIN, FT85BD_TX_PIN);
}

void ft85bd_request_data() {
    uint8_t payload[1] = {0x00};
    ft85bd_send_packet(payload, 1);
}

void ft85bd_request_extended_data() {
    uint8_t payload[1] = {0x02};
    ft85bd_send_packet(payload, 1);
}

void ft85bd_request_firmware() {
    uint8_t payload[1] = {0x11};
    ft85bd_send_packet(payload, 1);
}

void ft85bd_enable_auto_data(bool enable, uint16_t frequency_hz) {
    uint8_t payload[4];
    payload[0] = 0x1A;
    payload[1] = enable ? 1 : 0;
    payload[2] = (frequency_hz >> 8) & 0xFF;
    payload[3] = frequency_hz & 0xFF;
    ft85bd_send_packet(payload, 4);
}

// ===== НОВАЯ ФУНКЦИЯ: PAS_GEAR_CONTROL_AND_OBTAIN_DATA_ONCE (02H) =====
void ft85bd_set_pas_gear(uint8_t gear) {
    // gear: 0=N, 1=1st, 2=2nd, 3=3rd, 4=4th, 5=5th
    if(gear > 5) gear = 5;
    
    uint8_t payload[15] = {0};  // 15 байт данных (DLEN = 0x0F)
    payload[0] = 0x02;          // CMD
    payload[1] = gear;          // D0: передача PAS (0x00..0x05)
    // D1-D14 зарезервированы, заполнены нулями
    
    ft85bd_send_packet(payload, 15);
}

void reset_trip_stats() {
    bikeData.trip_distance_m = 0;
    bikeData.avg_speed = 0;
    bikeData.max_speed = 0;
    bikeData.speed_sum = 0;
    bikeData.speed_samples = 0;
    bikeData.max_speed_recorded = 0;
}

// ===== УДАЛЕНА СТАРАЯ ФУНКЦИЯ: ft85bd_set_current_gear_and_obtain =====

void ft85bd_update() {

    bikeData.speed_kmh = rpm_to_kmh(esc_data.rpm);

    // ===== РАСЧЁТ СРЕДНЕЙ И МАКСИМАЛЬНОЙ СКОРОСТИ =====
    static float speed_sum = 0;
    static uint32_t speed_samples = 0;
    static float max_speed_recorded = 0;

    // Максимальная скорость
    if(bikeData.speed_kmh > max_speed_recorded) {
        max_speed_recorded = bikeData.speed_kmh;
    }
    bikeData.max_speed = max_speed_recorded;

    // Средняя скорость (скользящее среднее за последние 100 измерений)
    speed_sum += bikeData.speed_kmh;
    speed_samples++;
    if(speed_samples > 100) {
        speed_sum = bikeData.speed_kmh * 100;
        speed_samples = 100;
    }
    bikeData.avg_speed = speed_sum / speed_samples;

    // Сброс максимума при остановке (если нужно)
    // if(bikeData.speed_kmh < 0.5f && !bikeData.moving) {
    //     max_speed_recorded = 0;
    // }

    if(millis() - bikeData.last_packet_ms > 1000) {
        bikeData.esc_online = false;
    }
    
    if(millis() - lastRequest > 100) {
        lastRequest = millis();
        ft85bd_request_data();
    }
    
    static uint8_t rxbuf[256];
    static int pos = 0;
    
    while(FT85BD.available()) {
        uint8_t b = FT85BD.read();
        if(pos < sizeof(rxbuf)) {
            rxbuf[pos++] = b;
        } else {
            pos = 0;
        }
        
        if(b == 0xDD) {
            if(pos >= 4) {
                uint16_t data_len = 0;
                uint8_t *data = nullptr;
                bool is_extended = false;
                
                if(rxbuf[0] == 0xAA) {
                    data_len = rxbuf[1];
                    data = &rxbuf[2];
                    is_extended = false;
                } else if(rxbuf[0] == 0xBB) {
                    data_len = ((uint16_t)rxbuf[1] << 8) | (uint16_t)rxbuf[2];
                    data = &rxbuf[3];
                    is_extended = true;
                } else {
                    pos = 0;
                    continue;
                }
                
                int header_len = is_extended ? 3 : 2;
                if(pos < (data_len + header_len + 3)) {
                    continue;
                }
                
                uint16_t data_crc16_calc = crc16_ftesc(data, data_len);
                uint16_t data_crc16_rece = ((uint16_t)data[data_len] << 8) | (uint16_t)data[data_len + 1];
                if(data_crc16_calc != data_crc16_rece) {
                    pos = 0;
                    continue;
                }
                
                if(data[data_len + 2] != 0xDD) {
                    pos = 0;
                    continue;
                }
                
                uint8_t uart_cmd = data[0];
                switch(uart_cmd) {
                    case 0x00: parse_realtime_packet(data); break;
                    case 0x02: parse_extended_packet(data); break;
                    case 0x11: parse_firmware_packet(data); break;
                    default: break;
                }
                update_bike_data();
            }
            pos = 0;
        }
    }
}