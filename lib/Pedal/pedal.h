#ifndef PEDAL_H
#define PEDAL_H

#include <stdint.h>
#include "pinMap.h"

// settings

constexpr bool FLIP_MOTOR_DIR = false;
// most reduced form of ratio between pedal voltages that has both values as a multiple of 10
#define pedal1_ratio 50 // analog input 0-1023
#define pedal2_ratio 33 // analog input 0-675
#define tolerance_threshold 3055 // numerically calculated, desmos graph used: https://www.desmos.com/calculator/alqb99rk5p
// threshold chosen to eliminate any chance of faulty inputs being read as non faulty, based on manufacturer specifications
uint16_t;

// function declarations

bool detectFault(uint16_t pedal1, uint16_t pedal2);

int16_t torqueMap(uint16_t pedal, bool flip_dir);

int16_t torqueMap(uint16_t pedal) {
    torqueMap(pedal, FLIP_MOTOR_DIR);
}

#endif // PEDAL_H