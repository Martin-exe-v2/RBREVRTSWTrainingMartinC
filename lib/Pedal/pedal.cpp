/**
 * @file pedal.cpp
 * @author Martin C
 * @brief Implementation of pedal related functions
 * @version 1.618
 * @date 2026-01-22
 * @see pedal.h
 */
#include "pedal.h"
#include "debug.hpp"

/**
 * @brief Checks for fault between 2 pedal readings
 * Scales both pedals up to a common scale, then calculates a difference
 * A fault is flagged if the difference in the scaled pedal inputs exceeds the threshold (10%, 3055)
 * @param pedal1 5V APPS input.
 * @param pedal2 3.3V APPS input.
 * @return true if difference exceeds threshold (fault detected), else false
 */
bool detectFault(uint16_t pedal1, uint16_t pedal2) {
    uint16_t pedal1_scaled = pedal1 * pedal2_ratio;
    uint16_t pedal2_scaled = pedal2 * pedal1_ratio;
    DBG_PEDAL_VALUES(pedal1, pedal2, pedal1_scaled, pedal2_scaled);
    int32_t dapps = pedal1_scaled - pedal2_scaled;
    // not int16_t due to maximum values of pedal#_scaled being > 32767, so simple subtraction cast into int16_t has edge cases
    if (abs(dapps) <= tolerance_threshold) {
        return true;
    }
    DBG_PEDAL_FAULT(DIFF_DAPPS, dapps);
    return false;
}

/**
 * @brief Maps pedal ADC value to a torque value.
 * @param pedal Pedal ADC in range of 0-1023, meant to be the 5V pedal.
 * @param flip_dir flips motor direction if needed.
 * @return Mapped torque value as defined by torque_points.
 */
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

/**
 * @brief Maps pedal ADC value to a torque value.
 * Overload that defaults to flip defined by FLIP_MOTOR_DIR
 * @param pedal Pedal ADC in range of 0-1023, meant to be the 5V pedal.
 * @return Mapped torque value as defined by torque_points.
 */
int16_t torqueMap(uint16_t pedal) {
    return torqueMap(pedal, FLIP_MOTOR_DIR);
}