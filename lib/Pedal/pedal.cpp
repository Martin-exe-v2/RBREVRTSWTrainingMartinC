#include "pedal.h"
#include "debug.hpp"

bool detectFault(uint16_t pedal1, uint16_t pedal2) {
    uint16_t pedal1_scaled = pedal1 * pedal2_ratio;
    uint16_t pedal2_scaled = pedal2 * pedal1_ratio;
    DBG_PEDAL_VALUES(pedal1, pedal2, pedal1_scaled, pedal2_scaled);
    uint16_t dapps;
    // not int16_t due to maximum values of pedal#_scaled being > 32767, so simple subtraction cast into int16_t has edge cases
    if (pedal1_scaled > pedal2_scaled) { // split is due to the maximum scaled values being > 32767
        dapps = pedal1_scaled - pedal2_scaled;
    }
    else {
        dapps = pedal2_scaled - pedal1_scaled;
    }
    if (dapps <= tolerance_threshold) {
        return true;
    }
    DBG_PEDAL_FAULT(DIFF_DAPPS, dapps);
    return false;
}

constexpr int16_t torqueMap(uint16_t pedal, bool flip_dir) { // note that the output range of flipped direction os -32767 - 0
    int16_t torque_ = 0;
    if (pedal <= torque_points[0].in) { // lower clamp
        return 0;
    }
    if (pedal >= torque_points[TORQUE_POINT_COUNT-1].in) {// upper clamp
        torque_ = torque_points[TORQUE_POINT_COUNT-1].out;
    }
    else for (int i = 1; i < TORQUE_POINT_COUNT; i++){ //loops through all possible non-trivial divisions
        if (pedal <= torque_points[i].in) { // linear interp
            const point &pl = torque_points[i-1];
            const point &ph = torque_points[i];
            uint32_t Din = ph.in - pl.in;
            uint32_t Dout = ph.out - pl.out;
            torque_ = pl.out + ((uint32_t)(pedal - pl.in) * Dout) / Din;
            break;
        }
    }
    if (flip_dir) { // apply flip
        torque_ = -1 * torque_;
    }
    return torque_;
    // lin interp
}

int16_t torqueMap(uint16_t pedal) {
    return torqueMap(pedal, FLIP_MOTOR_DIR);
}