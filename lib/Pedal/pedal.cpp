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

int16_t torqueMap(uint16_t pedal, bool flip_dir) {
    return 0;
}