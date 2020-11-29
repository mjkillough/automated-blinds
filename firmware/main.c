#include <FreeRTOS.h>
#include <task.h>

#include <espressif/esp_common.h>
#include <esp/uart.h>
#include <esp8266.h>

#include "steppers.h"

void step(void *pvParameters)
{
    struct steppers *s;
    steppers_init(&s);

    int steps = 0;
    while (1) {
        steps += 1;

        if (steps % 800 < 400) {
            steppers_step(s, 0, CLOCKWISE);
            steppers_release(s, 1);
        } else {
            steppers_release(s, 0);
            steppers_step(s, 1, CLOCKWISE);
        }

        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}

void user_init(void)
{
    uart_set_baud(0, 115200);

    xTaskCreate(step, "step", 256, NULL, 2, NULL);
}

