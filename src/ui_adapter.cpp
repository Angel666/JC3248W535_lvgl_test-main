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