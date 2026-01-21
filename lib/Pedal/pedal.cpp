#include "pedal.h"
#include <Arduino.h>

// read pedal 5V
// read pedal 3V3
// scale pedals to common range (R)
// if scaled pedals/R differ by >0.1:
//     pedals are faulty
// else:
//     pedals are not faulty
// if (pedals are faulty) for > 100ms:
//     set motor to 0
//     set status to INIT
// else:
//     torque = f(pedals)

bool detectFault(uint16_t pedal1, uint16_t pedal2) {
    uint16_t pedal1_scaled = pedal1 * pedal2_ratio;
    uint16_t pedal2_scaled = pedal2 * pedal1_ratio;
    if (pedal1_scaled > pedal2_scaled) {
        return (pedal1_scaled - pedal2_scaled) <= tolerance_threshold;
    }
    return (pedal2_scaled - pedal1_scaled) <= tolerance_threshold;
}

constexpr int16_t torqueMap(uint16_t pedal, bool flip_dir) {
    int16_t torque_ = 0;
    if (pedal <= torque_points[0].in) { // lower clamp
        return 0;
    }
    if (pedal >= torque_points[TORQUE_POINT_COUNT-1].in) {// upper clamp
        torque_ = torque_points[TORQUE_POINT_COUNT-1].out;
    }
    else for (int i = 1; i < TORQUE_POINT_COUNT; i++){ //loops through all possible non-trivial divisions
        if (pedal <= torque_points[i].in) {
            const point &pl = torque_points[i-1];
            const point &ph = torque_points[i];
            uint32_t Din = ph.in - pl.in;
            uint32_t Dout = ph.out - pl.out;
            torque_ = pl.out + ((uint32_t)(pedal - pl.in) * Dout) / Din;
            break;
        }
    }
    if (flip_dir) {
        torque_ = -1 * torque_;
    }
    return torque_;
    // lin interp
}

int16_t torqueMap(uint16_t pedal) {
    return torqueMap(pedal, FLIP_MOTOR_DIR);
}