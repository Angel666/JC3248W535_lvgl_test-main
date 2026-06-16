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

#define FT85BD_RX_PIN 16
#define FT85BD_TX_PIN 17

static uint32_t lastRequest = 0;

// ========== 1. CRC ФУНКЦИЯ ==========
static uint16_t crc16_ftesc(uint8_t *buf, uint16_t len)
{
    uint16_t crc = 0xFFFF;

    while(len--)
    {
        crc ^= *buf++;

        for(uint8_t i = 0; i < 8; i++)
        {
            if(crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }

    return crc;
}

// ========== 2. ФУНКЦИИ ЧТЕНИЯ ДАННЫХ ==========
static int16_t read_i16(
    const uint8_t *data,
    int &index)
{
    int16_t v =
        ((uint16_t)data[index] << 8) |
        data[index + 1];

    index += 2;

    return v;
}

static int32_t read_i32(
    const uint8_t *data,
    int &index)
{
    int32_t v =
        ((uint32_t)data[index] << 24) |
        ((uint32_t)data[index+1] << 16) |
        ((uint32_t)data[index+2] << 8) |
        data[index+3];

    index += 4;

    return v;
}

static float read_f16(
    const uint8_t *data,
    float scale,
    int &index)
{
    return (float)read_i16(data,index) / scale;
}

static float read_f32(
    const uint8_t *data,
    float scale,
    int &index)
{
    return (float)read_i32(data,index) / scale;
}

// ========== 3. ПАРСЕРЫ ОТВЕТОВ ==========

// Парсер базового ответа (команда 0x00)
static void parse_realtime_packet(
    uint8_t *data)
{
    int index = 1;

    esc_data.controller_id = data[index++];
    esc_data.fault = data[index++];

    esc_data.inpVoltage =
        read_f16(data,100.0f,index);

    esc_data.inputCurrent =
        read_f32(data,1000000.0f,index);

    esc_data.motorCurrent =
        read_f32(data,1000000.0f,index);

    esc_data.rpm =
        read_f32(data,1.0f,index);

    esc_data.dutyCycleNow =
        read_f16(data,10000.0f,index);

    esc_data.temp_fet =
        read_f16(data,100.0f,index);

    esc_data.temp_motor =
        read_f16(data,100.0f,index);

    esc_data.cpu_load =
        read_f16(data,10000.0f,index);

    esc_data.encoder_angle =
        read_f32(data,1000000.0f,index);

    esc_data.pas_rpm =
        read_f32(data,1.0f,index);
}

// Парсер расширенного ответа (команда 0x02)
static void parse_extended_packet(uint8_t *data)
{
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
static void parse_firmware_packet(uint8_t *data)
{
    int index = 1;
    uint8_t major = data[index++];
    uint8_t minor = data[index++];
    
    snprintf(esc_data.firmware_version, 
             sizeof(esc_data.firmware_version),
             "%d.%d", major, minor);
    
    Serial.printf("ESC Firmware: %s\n", esc_data.firmware_version);
}

// ========== 4. ОБНОВЛЕНИЕ ДАННЫХ ==========
static uint32_t lastDistanceUpdate = 0;

// Для расчёта энергии
static uint32_t lastEnergyUpdate = 0;

// Последний сохранённый пробег
static uint32_t lastSavedOdoKm = 0;

static void update_bike_data()
{
    bikeData.battery_voltage =
        esc_data.inpVoltage;

    bikeData.battery_current =
        esc_data.inputCurrent;

    bikeData.motor_current =
        esc_data.motorCurrent;

    bikeData.battery_power =
        esc_data.inpVoltage *
        esc_data.inputCurrent;

    // Подсчёт потреблённой энергии (Wh)
    uint32_t nowEnergy = millis();

    if(lastEnergyUpdate != 0)
    {
        float dtHours =
            (nowEnergy - lastEnergyUpdate)
            / 3600000.0f;

        bikeData.battery_energy_wh +=
            bikeData.battery_power *
            dtHours;
    }

    lastEnergyUpdate = nowEnergy;    

    bikeData.battery_percent =
    battery_voltage_to_percent(
        esc_data.inpVoltage
    );
    
    bikeData.battery_energy_percent =
    100.0f -
    (bikeData.battery_energy_wh /
     BATTERY_CAPACITY_WH * 100.0f);

    if(bikeData.battery_energy_percent < 0)
    {
        bikeData.battery_energy_percent = 0;
    }

    bikeData.motor_rpm =
        esc_data.rpm;

    bikeData.pas_rpm =
        esc_data.pas_rpm;

    // движение
    bikeData.moving =
        bikeData.speed_kmh > 1.0f;

    // педалирование (простая логика)
    bikeData.pedaling =
        bikeData.pas_rpm > 5.0f;

    bikeData.speed_kmh =
        rpm_to_kmh(esc_data.rpm);

    // Расчёт пройденного расстояния
    uint32_t now = millis();

    if(lastDistanceUpdate != 0)
    {
        float dt =
            (now - lastDistanceUpdate) / 1000.0f;

        float meters =
            bikeData.speed_kmh *
            1000.0f / 3600.0f *
            dt;

        bikeData.trip_distance_m +=
            (uint32_t)meters;

        bikeData.odometer_km =
            storage_load_odometer() +
            bikeData.trip_distance_m / 1000;

        // Сохраняем одометр не чаще чем раз в 1 км
        if(bikeData.odometer_km >= lastSavedOdoKm + 1)
        {
            storage_save_odometer(
                bikeData.odometer_km
            );

            lastSavedOdoKm =
                bikeData.odometer_km;
        }        
    }

    lastDistanceUpdate = now;   

    bikeData.controller_temp =
        esc_data.temp_fet;

    bikeData.motor_temp =
        esc_data.temp_motor;

    bikeData.duty_cycle =
        esc_data.dutyCycleNow;

    // ===== СУЩЕСТВУЮЩИЕ ПОЛЯ (НЕ МЕНЯЕМ) =====
    bikeData.cpu_load =
        esc_data.cpu_load;

    bikeData.encoder_angle =
        esc_data.encoder_angle;

    bikeData.fault_code =
        esc_data.fault;

    bikeData.last_packet_ms =
        millis();

    bikeData.esc_online =
        true;

    // ===== НОВЫЕ ПОЛЯ (для второго экрана) =====
    snprintf(bikeData.firmware_version, 
             sizeof(bikeData.firmware_version),
             "%s", esc_data.firmware_version);
    
    bikeData.speed_limit_gear = esc_data.speed_limit_gear;
    bikeData.motor_direction = esc_data.motor_direction;
    bikeData.cruise_enabled = esc_data.cruise_enabled;
    bikeData.multi_mode_type = esc_data.multi_mode_type;
    bikeData.error_code = esc_data.error_code;
    bikeData.controller_id = esc_data.controller_id;
    bikeData.current_gear = esc_data.speed_limit_gear;  // Передача из ESC
    bikeData.cruise_active = esc_data.cruise_active;    // Статус круиза из ESC
    
}

// ========== 5. ФУНКЦИЯ ОТПРАВКИ ПАКЕТА ==========
void ft85bd_send_packet(uint8_t *payload, uint8_t payload_len){
    uint8_t packet[256];
    int idx = 0;
    
    packet[idx++] = 0xAA;                    // STX
    packet[idx++] = payload_len;             // DLEN
    memcpy(&packet[idx], payload, payload_len);
    idx += payload_len;
    
    // Вычисляем CRC для CMD+DATA
    uint16_t crc = crc16_ftesc(payload, payload_len);
    packet[idx++] = (crc >> 8) & 0xFF;       // CRC high
    packet[idx++] = crc & 0xFF;              // CRC low
    packet[idx++] = 0xDD;                    // ETX
    
    FT85BD.write(packet, idx);
}

// ========== 6. ОСНОВНЫЕ ФУНКЦИИ ==========
void ft85bd_init()
{
    FT85BD.begin(
        FT85BD_BAUDRATE,
        SERIAL_8N1,
        FT85BD_RX_PIN,
        FT85BD_TX_PIN
    );
}

void ft85bd_request_data()
{
    uint8_t payload[1] = {0x00};  // CMD = 0x00
    ft85bd_send_packet(payload, 1);
}

// Новая функция для запроса расширенных данных (команда 0x02)
void ft85bd_request_extended_data()
{
    uint8_t payload[1] = {0x02};
    ft85bd_send_packet(payload, 1);
}

// Новая функция для запроса версии прошивки (команда 0x11)
void ft85bd_request_firmware()
{
    uint8_t payload[1] = {0x11};
    ft85bd_send_packet(payload, 1);
}

// Новая функция для включения авто-опроса (команда 0x1A)
void ft85bd_enable_auto_data(bool enable, uint16_t frequency_hz)
{
    uint8_t payload[4];
    payload[0] = 0x1A;
    payload[1] = enable ? 1 : 0;
    payload[2] = (frequency_hz >> 8) & 0xFF;
    payload[3] = frequency_hz & 0xFF;
    
    ft85bd_send_packet(payload, 4);
}

// ===== ФУНКЦИЯ УПРАВЛЕНИЯ ТОКОМ С ПЕРЕДАЧЕЙ (20H) =====
void ft85bd_set_current_gear_and_obtain(float current_amps, uint8_t gear)
{
    // current_amps: ток в амперах (например, 15.5)
    // gear: 0=нет, 1=низкая, 2=средняя, 3=высокая, 4=реверс
    
    int32_t current_value = (int32_t)(current_amps * 1000.0f);
    uint8_t payload[6];
    
    payload[0] = 0x20;  // CMD
    payload[1] = (current_value >> 24) & 0xFF;
    payload[2] = (current_value >> 16) & 0xFF;
    payload[3] = (current_value >> 8) & 0xFF;
    payload[4] = current_value & 0xFF;
    payload[5] = gear;  // Передача
    
    ft85bd_send_packet(payload, 6);
}

void ft85bd_update()
{
    if(millis() - bikeData.last_packet_ms > 1000)
    {
        bikeData.esc_online = false;
    }
    
    if(millis() - lastRequest > 100)
    {
        lastRequest = millis();
        ft85bd_request_data();  // Базовый запрос
    }
    
    static uint8_t rxbuf[256];
    static int pos = 0;
    
    while(FT85BD.available())
    {
        uint8_t b = FT85BD.read();
        
        if(pos < sizeof(rxbuf))
        {
            rxbuf[pos++] = b;
        }
        else
        {
            pos = 0;
        }
        
        if(b == 0xDD)  // Конец пакета
        {
            if(pos >= 4)  // Минимальный размер: STX + DLEN + CMD + ETX
            {
                uint16_t data_len = 0;
                uint8_t *data = nullptr;
                bool is_extended = false;
                
                // === 1. Определяем формат пакета (по инструкции) ===
                if(rxbuf[0] == 0xAA)
                {
                    data_len = rxbuf[1];
                    data = &rxbuf[2];
                    is_extended = false;
                }
                else if(rxbuf[0] == 0xBB)
                {
                    data_len = ((uint16_t)rxbuf[1] << 8) | (uint16_t)rxbuf[2];
                    data = &rxbuf[3];
                    is_extended = true;
                }
                else
                {
                    // Некорректный стартовый байт
                    pos = 0;
                    continue;
                }
                
                // Проверяем, что буфер содержит полный пакет
                int header_len = is_extended ? 3 : 2;
                if(pos < (data_len + header_len + 3))  // +3: CRC(2) + ETX(1)
                {
                    // Пакет ещё не полный
                    continue;
                }
                
                // === 2. Проверка CRC16 ===
                uint16_t data_crc16_calc = crc16_ftesc(data, data_len);
                uint16_t data_crc16_rece = ((uint16_t)data[data_len] << 8) | (uint16_t)data[data_len + 1];
                
                if(data_crc16_calc != data_crc16_rece)
                {
                    // CRC ошибка
                    pos = 0;
                    continue;
                }
                
                // === 3. Проверка конца пакета (0xDD) ===
                if(data[data_len + 2] != 0xDD)
                {
                    pos = 0;
                    continue;
                }
                
                // === 4. Парсинг в зависимости от команды ===
                uint8_t uart_cmd = data[0];
                
                switch(uart_cmd)
                {
                    case 0x00:  // UART_OBTAIN_DATA_ONCE
                        parse_realtime_packet(data);
                        break;
                        
                    case 0x02:  // UART_CONTROL_AND_OBTAIN_DATA_ONCE
                        parse_extended_packet(data);
                        break;
                        
                    case 0x11:  // UART_OBTAIN_FIRMWARE_VERSION
                        parse_firmware_packet(data);
                        break;

                    case 0x20:  // UART_SET_CURRENT_GEAR_AND_OBTAIN_DATA
                        parse_extended_packet(data);  // Тот же формат, что и 0x02
                        break;    
                        
                    default:
                        // Неизвестная команда
                        break;
                }
                
                update_bike_data();
            }
            
            pos = 0;  // Сброс буфера
        }
    }
}