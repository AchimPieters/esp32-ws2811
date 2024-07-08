/**

   Copyright 2024 Achim Pieters | StudioPieters®

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

   for more information visit https://www.studiopieters.nl

 **/

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
