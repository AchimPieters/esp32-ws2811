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

#include "ws2811_led_strip.h"
#include <string.h>
#include <stdlib.h>

#define WS8211_T0H 14  // 0 bit high time
#define WS8211_T0L 34  // 0 bit low time
#define WS8211_T1H 34  // 1 bit high time
#define WS8211_T1L 14  // 1 bit low time

#define LED_STRIP_RMT_CLK_DIV 2
#define LED_STRIP_RMT_TICKS_BIT 42  // Number of ticks per bit, based on clock divider

static void ws8211_rmt_adapter(const void *src, rmt_item32_t *dest, size_t src_size, size_t wanted_num, size_t *translated_size, size_t *item_num)
{
        if (src == NULL || dest == NULL) {
                *translated_size = 0;
                *item_num = 0;
                return;
        }

        const uint8_t *p = (const uint8_t *)src;
        size_t size = 0;
        size_t num = 0;

        while (size < src_size && num < wanted_num) {
                for (int i = 7; i >= 0; i--) {
                        if (p[size] & (1 << i)) {
                                dest[num].duration0 = WS8211_T1H;
                                dest[num].level0 = 1;
                                dest[num].duration1 = WS8211_T1L;
                                dest[num].level1 = 0;
                        } else {
                                dest[num].duration0 = WS8211_T0H;
                                dest[num].level0 = 1;
                                dest[num].duration1 = WS8211_T0L;
                                dest[num].level1 = 0;
                        }
                        num++;
                }
                size++;
        }

        *translated_size = size;
        *item_num = num;
}

led_strip_t *led_strip_init(rmt_channel_t channel, gpio_num_t gpio, uint16_t strip_length)
{
        rmt_config_t config = RMT_DEFAULT_CONFIG_TX(gpio, channel);
        config.clk_div = LED_STRIP_RMT_CLK_DIV;
        rmt_config(&config);
        rmt_driver_install(config.channel, 0, 0);

        led_strip_t *strip = (led_strip_t *)malloc(sizeof(led_strip_t));
        if (!strip) return NULL;

        strip->channel = channel;
        strip->gpio = gpio;
        strip->strip_length = strip_length;
        strip->items = (rmt_item32_t *)malloc(strip_length * 24 * sizeof(rmt_item32_t));
        if (!strip->items) {
                free(strip);
                return NULL;
        }

        rmt_translator_init(strip->channel, ws8211_rmt_adapter);
        return strip;
}

void led_strip_free(led_strip_t *strip)
{
        if (strip) {
                if (strip->items) free(strip->items);
                free(strip);
        }
}

void led_strip_set_pixel(led_strip_t *strip, uint16_t index, uint8_t red, uint8_t green, uint8_t blue)
{
        if (index < strip->strip_length) {
                uint32_t pos = index * 24;
                uint8_t color[] = {green, red, blue};

                for (int i = 0; i < 3; i++) {
                        for (int j = 7; j >= 0; j--) {
                                if (color[i] & (1 << j)) {
                                        strip->items[pos].duration0 = WS8211_T1H;
                                        strip->items[pos].level0 = 1;
                                        strip->items[pos].duration1 = WS8211_T1L;
                                        strip->items[pos].level1 = 0;
                                } else {
                                        strip->items[pos].duration0 = WS8211_T0H;
                                        strip->items[pos].level0 = 1;
                                        strip->items[pos].duration1 = WS8211_T0L;
                                        strip->items[pos].level1 = 0;
                                }
                                pos++;
                        }
                }
        }
}

void led_strip_refresh(led_strip_t *strip)
{
        rmt_write_items(strip->channel, strip->items, strip->strip_length * 24, true);
        rmt_wait_tx_done(strip->channel, portMAX_DELAY);
}

void led_strip_clear(led_strip_t *strip)
{
        memset(strip->items, 0, strip->strip_length * 24 * sizeof(rmt_item32_t));
        led_strip_refresh(strip);
}
