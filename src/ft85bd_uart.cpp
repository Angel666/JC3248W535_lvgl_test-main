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
    // переполнение буфера
    // начинаем поиск нового пакета

    pos = 0;
}

        if(b == 0xDD)
        {
            if(rxbuf[0] == 0xAA)
            {
                uint16_t len = rxbuf[1];

                uint8_t *payload = &rxbuf[2];

                parse_realtime_packet(payload);

                update_bike_data();
            }

            pos = 0;
        }
    }
}