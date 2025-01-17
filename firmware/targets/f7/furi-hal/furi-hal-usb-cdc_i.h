#pragma once

#include <stdint.h>
#include "usb_cdc.h" 

#define CDC_DATA_SZ 64

typedef struct {
    void (*tx_ep_callback)(void);
    void (*rx_ep_callback)(void);
    void (*state_callback)(uint8_t state);
    void (*ctrl_line_callback)(uint8_t state);
    void (*config_callback)(struct usb_cdc_line_coding* config);
} CdcCallbacks;

void furi_hal_cdc_set_callbacks(uint8_t if_num, CdcCallbacks* cb);

struct usb_cdc_line_coding* furi_hal_cdc_get_port_settings(uint8_t if_num);

uint8_t furi_hal_cdc_get_ctrl_line_state(uint8_t if_num);

void furi_hal_cdc_send(uint8_t if_num, uint8_t* buf, uint16_t len);

int32_t furi_hal_cdc_receive(uint8_t if_num, uint8_t* buf, uint16_t max_len);
