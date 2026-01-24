#ifndef PEDAL_H
#define PEDAL_H

#include <stdint.h>
#include "enums.h"
// settings

// slips motor direction
constexpr bool FLIP_MOTOR_DIR = false;
/**
 * @brief Most reduced form of ratio between pedal voltages
 * tolerance_threshold was numerically calculated (desmos graph used: https://www.desmos.com/calculator/alqb99rk5p)
 * threshold chosen to eliminate any faulty inputs being read as non faulty due to manufacturer tolerances
 */
#define pedal1_ratio 50 // analog input 0-1023
#define pedal2_ratio 33 // analog input 0-680
#define tolerance_threshold 3055

// helper struct for tables
struct point {
    uint16_t in;
    int16_t out;
};
// table of values for linear interpolation for torque
constexpr int TORQUE_POINT_COUNT = 8;
constexpr point torque_points[TORQUE_POINT_COUNT] = {
    {125, 0},
    {250, 1706},
    {375, 6050},
    {500, 12613},
    {625, 20096},
    {750, 26672},
    {875, 31036},
    {1000, 32767} // ensure maximum value is within +-32767
};
// out = floor(g(in)), g(x) = f(x-minin)/(f(x-minin)-f(maxin+1-x)), f(x) = (x/(maxin-minin+1))^phi

// function declarations

bool detectFault(uint16_t pedal1, uint16_t pedal2);

constexpr int16_t torqueMap(uint16_t pedal, bool flip_dir);
int16_t torqueMap(uint16_t pedal);

#endif // PEDAL_H