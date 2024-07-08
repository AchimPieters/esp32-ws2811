#ifndef LED_STRIP_H
#define LED_STRIP_H

#include "driver/rmt.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    rmt_channel_t channel;
    gpio_num_t gpio;
    uint16_t strip_length;
    rmt_item32_t *items;
} led_strip_t;

led_strip_t *led_strip_init(rmt_channel_t channel, gpio_num_t gpio, uint16_t strip_length);
void led_strip_free(led_strip_t *strip);
void led_strip_set_pixel(led_strip_t *strip, uint16_t index, uint8_t red, uint8_t green, uint8_t blue);
void led_strip_refresh(led_strip_t *strip);
void led_strip_clear(led_strip_t *strip);

#ifdef __cplusplus
}
#endif

#endif // LED_STRIP_H
