#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "enums.h"

#define DEBUG true

#if DEBUG
#include "debugCAN.hpp"
#endif

#define DEBUG_MOTOR true && DEBUG
#   define DEBUG_MOTOR_STOP true && DEBUG_MOTOR
#   define DEBUG_MOTOR_TORQUE true && DEBUG_MOTOR
#define DEBUG_BMS true && DEBUG
#   define DEBUG_BMS_READY true && DEBUG_BMS
#   define DEBUG_BMS_AWAIT true && DEBUG_BMS
#define DEBUG_STATUS true && DEBUG
#   define DEBUG_STATUS_FAULT true && DEBUG_STATUS
#define DEBUG_PEDAL true && DEBUG
#   define DEBUG_PEDAL_VALUES true && DEBUG_PEDAL
#   define DEBUG_PEDAL_FAULT true && DEBUG_PEDAL

inline void DBG_MOTOR_STOP() {
    #if DEBUG_MOTOR_STOP
        debugCAN::motor_stop();
    #endif
}

inline void DBG_MOTOR_TORQUE(int16_t torque_) {
    #if DEBUG_MOTOR_TORQUE
        debugCAN::motor_torque(torque_);
    #endif
}

inline void DBG_BMS_READY() {
    #if DEBUG_BMS_READY
        debugCAN::bms_ready();
    #endif
}

inline void DBG_BMS_AWAIT() {
    #if DEBUG_BMS_AWAIT
        debugCAN::bms_await();
    #endif
}

inline void DBG_STATUS(CarStatus car_status_) {
    #if DEBUG_STATUS
        debugCAN::status_car(car_status_);
    #endif
}

inline void DBG_STATUS_FAULT() {
    #if DEBUG_STATUS_FAULT
        debugCAN::status_fault();
    #endif
}

inline void DBG_PEDAL_VALUES(uint16_t pedal1, uint16_t pedal2, uint16_t pedal1_scaled, uint16_t pedal2_scaled) {
    #if DEBUG_PEDAL_VALUES
        debugCAN::pedal_values(pedal1, pedal2, pedal1_scaled, pedal2_scaled);
    #endif
}

inline void DBG_PEDAL_FAULT(fault_status fault_status_) {
    #if DEBUG_PEDAL_FAULT
        debugCAN::pedal_fault(fault_status_);
    #endif
}

inline void DBG_PEDAL_FAULT(fault_status fault_status_, int32_t dapps) {
    #if DEBUG_PEDAL_FAULT
        debugCAN::pedal_fault(fault_status_, dapps);
    #endif
}

#endif // DEBUG_HPP