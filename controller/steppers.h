// Driver for Adafruit DC+Stepper FeatherWing board.
//
// Supports 2 stepper motors attached to a single board, with a simple full-step
// rotation in either direction.
//
// Useful documentation:
//
//  - Schematic:
//    https://learn.adafruit.com/assets/34460
//  - PCA9685 PWM datasheet:
//    https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf
//  - TB6612FNG motor driver datasheet:
//    https://cdn-shop.adafruit.com/datasheets/TB6612FNG_datasheet_en_20121101.pdf

#ifndef _STEPPERS_H_
#define _STEPPERS_H_

#include <stdbool.h>
#include <stddef.h>

enum direction {
    CLOCKWISE,
    ANTICLOCKWISE,
};

// Two stepper motors connected to DC+Stepper FeatherWing board.
struct steppers;

// Initialize the DC+Stepper FeatherWing board.
bool steppers_init(struct steppers **s);

// Get a single stepper by index.
struct stepper* steppers_stepper(struct steppers *s, size_t idx);

// Release a stepper motor by index.
void steppers_release(struct steppers *s, size_t idx);

// Step a stepper motor by index.
void steppers_step(struct steppers *s, size_t idx, enum direction dir);

// A single stepper motor from a struct steppers.
struct stepper;

// Release the stepper motor, allowing it to turn freely.
// If motor doesn't need to be kept stationary, this reduces power consumption
// and avoids unnecessary whine.
void stepper_release(struct stepper *s);

// Step the stepper motor.
void stepper_step(struct stepper *s, enum direction dir);

#endif // _STEPPERS_H_

