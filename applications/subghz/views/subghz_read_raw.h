#pragma once

#include <gui/view.h>
#include "../helpers/subghz_custom_event.h"

typedef struct SubghzReadRAW SubghzReadRAW;

typedef void (*SubghzReadRAWCallback)(SubghzCustomEvent event, void* context);

void subghz_read_raw_set_callback(
    SubghzReadRAW* subghz_read_raw,
    SubghzReadRAWCallback callback,
    void* context);

SubghzReadRAW* subghz_read_raw_alloc();

void subghz_read_raw_free(SubghzReadRAW* subghz_static);

void subghz_read_raw_add_data_statusbar(
    SubghzReadRAW* instance,
    const char* frequency_str,
    const char* preset_str);

void subghz_read_raw_update_sample_write(SubghzReadRAW* instance, size_t sample);

void subghz_read_raw_add_data_rssi(SubghzReadRAW* instance, float rssi);

View* subghz_read_raw_get_view(SubghzReadRAW* subghz_static);
