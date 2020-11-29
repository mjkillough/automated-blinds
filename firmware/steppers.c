#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include <i2c/i2c.h>
#include <pca9685/pca9685.h>

#include "steppers.h"

#define ADDR 0x60

#define I2C_BUS 0
#define SCL_PIN 5
#define SDA_PIN 4

#define PWM_FREQ 1525

#define STEPPERS_DEBUG true

#ifdef STEPPERS_DEBUG
#include <stdio.h>
#define debug(fmt, ...) printf("%s: " fmt "\n", "steppers", ## __VA_ARGS__)
#else
#define debug(fmt, ...)
#endif

enum pin {
    AIN1 = 0x01,
    AIN2 = 0x02,
    BIN1 = 0x04,
    BIN2 = 0x08,
};

static enum pin steps[] = { AIN2|BIN2, AIN2|BIN1, AIN1|BIN1, AIN1|BIN2 };

struct pins {
    uint8_t pwma;
    uint8_t pwmb;
    uint8_t ain1;
    uint8_t ain2;
    uint8_t bin1;
    uint8_t bin2;
};

struct stepper {
    i2c_dev_t *dev;
    uint8_t step;
    struct pins pins;
};

struct steppers {
    i2c_dev_t dev;
    struct stepper steppers[2];
};

bool steppers_init(struct steppers **s) {
    *s = malloc(sizeof(struct steppers));
    if (*s == NULL) {
        debug("steppers_init: cannot allocate struct steppers");
        return false;
    }

    int status = i2c_init(I2C_BUS, SCL_PIN, SDA_PIN, I2C_FREQ_100K);
    if (status < 0) {
        debug("i2c_init failed: %d", status);
        goto error;
    }

    (*s)->dev = (i2c_dev_t){
        .addr = ADDR,
        .bus = I2C_BUS,
    };

    pca9685_init(&(*s)->dev);
    bool ok = pca9685_set_pwm_frequency(&(*s)->dev, PWM_FREQ);
    if (!ok) {
        debug("pca9685_set_pwm_frequency: not ok");
        goto error;
    }

    debug("pca9685: desired freq is %d Hz, real freq %d Hz\n", PWM_FREQ, pca9685_get_pwm_frequency(&(*s)->dev));

    // See schematic: https://learn.adafruit.com/assets/34460
    (*s)->steppers[0].dev = &(*s)->dev;
    (*s)->steppers[0].step = 0;
    (*s)->steppers[0].pins = (struct pins){
        .pwma = 8,
        .pwmb = 13,
        .ain1 = 10,
        .ain2 = 9,
        .bin1 = 11,
        .bin2 = 12,
    };
    (*s)->steppers[1].dev = &(*s)->dev;
    (*s)->steppers[1].step = 0;
    (*s)->steppers[1].pins = (struct pins){
        .pwma = 2,
        .pwmb = 7,
        .ain1 = 4,
        .ain2 = 3,
        .bin1 = 5,
        .bin2 = 6,
    };

    return true;

error:
    free(*s);
    return false;
}

struct stepper* steppers_stepper(struct steppers *s, size_t idx) {
    if (idx > 1) {
        debug("steppers_stepper: idx out of bounds: %d", idx);
        return NULL;
    }

    return &s->steppers[idx];
}

void steppers_release(struct steppers *s, size_t idx) {
    struct stepper *stepper = steppers_stepper(s, idx);
    if (stepper == NULL) {
        return;
    }
    stepper_release(stepper);
}

void steppers_step(struct steppers *s, size_t idx, enum direction dir) {
    struct stepper *stepper = steppers_stepper(s, idx);
    if (stepper == NULL) {
        return;
    }
    stepper_step(stepper, dir);
}

static void stepper_set_pin(struct stepper *s, uint8_t pin, bool on) {
    pca9685_set_pwm_value(s->dev, pin, on ? 4096 : 0);
}

void stepper_release(struct stepper *s) {
    stepper_set_pin(s, s->pins.pwma, false);
    stepper_set_pin(s, s->pins.pwmb, false);
    stepper_set_pin(s, s->pins.ain1, false);
    stepper_set_pin(s, s->pins.ain2, false);
    stepper_set_pin(s, s->pins.bin1, false);
    stepper_set_pin(s, s->pins.bin2, false);
}

void stepper_step(struct stepper *s, enum direction dir) {
    if (dir == CLOCKWISE) {
        s->step++;
    } else {
        s->step--;
    }

    // Limit current to motors via PWM:
    pca9685_set_pwm_value(s->dev, s->pins.pwma, 0x2FF);
    pca9685_set_pwm_value(s->dev, s->pins.pwmb, 0x2FF);

    enum pin pins = steps[s->step % 4];
    stepper_set_pin(s, s->pins.ain1, pins & AIN1);
    stepper_set_pin(s, s->pins.ain2, pins & AIN2);
    stepper_set_pin(s, s->pins.bin1, pins & BIN1);
    stepper_set_pin(s, s->pins.bin2, pins & BIN2);
}

