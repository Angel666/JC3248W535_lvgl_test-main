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
#include "ui_adapter.h"


static void parse_realtime_packet(uint8_t *data);
static void parse_extended_packet(uint8_t *data);
static void parse_firmware_packet(uint8_t *data);
static void update_bike_data(void);
static uint16_t crc16_ftesc(uint8_t *buf, uint16_t len);
static int16_t read_i16(const uint8_t *data, int &index);
static int32_t read_i32(const uint8_t *data, int &index);
static float read_f16(const uint8_t *data, float scale, int &index);
static float read_f32(const uint8_t *data, float scale, int &index);

HardwareSerial FT85BD(1);

FT85BD_Data esc_data;

#define FT85BD_RX_PIN 16
#define FT85BD_TX_PIN 17


static uint32_t lastRequest = 0;

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
    uint8_t packet[] =
    {
        0xAA,
        0x01,
        0x00,
        0x70,
        0x00,
        0xDD
    };

    FT85BD.write(packet, sizeof(packet));
}

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

static uint32_t lastDistanceUpdate = 0;

// Для расчёта энергии

static uint32_t lastEnergyUpdate = 0;

// Последний сохранённый пробег

static uint32_t lastSavedOdoKm = 0;

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
    
    // Новые поля
    esc_data.direction_switching = data[index++];
    esc_data.motor_direction = data[index++];
    esc_data.speed_limit_gear = data[index++];
    
    index += 3;  // Пропускаем D7,D8,D9 (horn, light, brake light)
    
    esc_data.cruise_enabled = data[index++];
    esc_data.cruise_active = data[index++];
    esc_data.multi_mode_enabled = data[index++];
    esc_data.multi_mode_type = data[index++];
}

// Функция парсинга версии прошивки
static void parse_firmware_packet(uint8_t *data)
{
    int index = 1;
    uint8_t major = data[index++];
    uint8_t minor = data[index++];
    
    snprintf(esc_data.firmware_version, 
             sizeof(esc_data.firmware_version),
             "%d.%d", major, minor);
}

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

    if(
        bikeData.odometer_km >=
        lastSavedOdoKm + 1
)
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

    snprintf(bikeData.firmware_version, 
             sizeof(bikeData.firmware_version),
             "%s", esc_data.firmware_version);
    
    bikeData.speed_limit_gear = esc_data.speed_limit_gear;
    bikeData.motor_direction = esc_data.motor_direction;
    bikeData.cruise_enabled = esc_data.cruise_enabled;
    bikeData.cruise_active = esc_data.cruise_active;
    bikeData.multi_mode_type = esc_data.multi_mode_type;
    bikeData.cpu_load = esc_data.cpu_load;
    bikeData.encoder_angle = esc_data.encoder_angle;
    bikeData.pas_rpm_display = esc_data.pas_rpm;
    bikeData.error_code = esc_data.error_code;
    bikeData.controller_id = esc_data.controller_id;
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
        ft85bd_request_data();
    }
    
    static uint8_t rxbuf[128];
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
            if(pos >= 5 && rxbuf[0] == 0xAA)
            {
                uint8_t dlen = rxbuf[1];
                
                // ВАЖНО: получаем команду из ответа (3-й байт после STX и DLEN)
                // Структура пакета: [0xAA][DLEN][CMD][DATA...][CRC][0xDD]
                uint8_t response_cmd = rxbuf[2];
                
                // Вычисляем CRC для проверки целостности
                uint16_t received_crc = (rxbuf[2 + dlen] << 8) | rxbuf[2 + dlen + 1];
                uint16_t calculated_crc = crc16_ftesc(&rxbuf[2], dlen);
                
                if(received_crc == calculated_crc)
                {
                    // ВЫБИРАЕМ ПАРСЕР В ЗАВИСИМОСТИ ОТ КОМАНДЫ
                    switch(response_cmd)
                    {
                        case 0x00:  // Базовый ответ (команда 0x00)
                            parse_realtime_packet(&rxbuf[2]);
                            break;
                            
                        case 0x02:  // Расширенный ответ (команда 0x02)
                            parse_extended_packet(&rxbuf[2]);
                            break;
                            
                        case 0x11:  // Ответ с версией прошивки (команда 0x11)
                            parse_firmware_packet(&rxbuf[2]);
                            break;
                            
                        default:
                            // Неизвестная команда — игнорируем
                            Serial.printf("Unknown response cmd: 0x%02X\n", response_cmd);
                            break;
                    }
                    
                    update_bike_data();  // Обновляем bikeData из esc_data
                }
                else
                {
                    Serial.printf("CRC error: recv=0x%04X calc=0x%04X\n", 
                                  received_crc, calculated_crc);
                }
            }
            
            pos = 0;  // Сброс буфера
        }
    }
}