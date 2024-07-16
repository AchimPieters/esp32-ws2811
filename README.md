# WS2811 RGBIC LED Strip Driver

This driver is designed for addressable RGBIC LED's the WS2811, where the LED's are controlled by a single data line.


<img  style="float: right;" src="https://github.com/AchimPieters/ESP32-SmartPlug/blob/main/images/MIT%7C%20SOFTWARE%20WHITE.svg" width="150"> 

# Hardware Setup




# Example

```
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
 
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ws2811.h>

#define LED_STRIP_LENGTH 3
#define GPIO_NUM 5


void app_main(void)
{
    led_strip_t *strip = led_strip_init(RMT_CHANNEL_0, GPIO_NUM, LED_STRIP_LENGTH);

    if (!strip) {
        printf("Failed to initialize LED strip\n");
        return;
    }

    while (1) {
        for (int i = 0; i < LED_STRIP_LENGTH; i++) {
            led_strip_set_pixel(strip, i, 255, 0, 0);  // Red
        }
        led_strip_refresh(strip);
        vTaskDelay(pdMS_TO_TICKS(1000));

        for (int i = 0; i < LED_STRIP_LENGTH; i++) {
            led_strip_set_pixel(strip, i, 0, 255, 0);  // Green
        }
        led_strip_refresh(strip);
        vTaskDelay(pdMS_TO_TICKS(1000));

        for (int i = 0; i < LED_STRIP_LENGTH; i++) {
            led_strip_set_pixel(strip, i, 0, 0, 255);  // Blue
        }
        led_strip_refresh(strip);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    led_strip_free(strip);
}

```
