/**
 * @file debugCAN.cpp
 * @author Martin C
 * @brief Implementation for debugCAN namespace for CAN debug messages
 * @version 1.0
 * @date 2026-01-22
 * @see debugCAN.hpp
 */
#include "debugCAN.hpp"

MCP2515 *can_interface;

/**
 * @brief Declares the interface debugCAN uses.
 * Should be called before using any other debugCAN functions.
 * @param can Pointer to the MCP2515 CAN controller instance for debugging.
 */
void debugCAN::init(MCP2515 *can) {
    if (can == nullptr) {
        return;
    }
    can_interface = can;
}

/**
 * @brief Sends a debug motor stop message over CAN
 */
void debugCAN::motor_stop() {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = MOTOR_STOP;
    txmsg.can_dlc = 0;

    can_interface -> sendMessage(&txmsg);
}

/**
 * @brief sends a debug torque message over CAN
 * @param torque_ Torque value sent to motor
 */
void debugCAN::motor_torque(int16_t torque_) {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = TORQUE_VAL;
    txmsg.can_dlc = 2;
    
    txmsg.data[0] = torque_ && 0xFF;
    txmsg.data[1] = (torque_ >> 8) && 0xFF;

    can_interface -> sendMessage(&txmsg);
}

/**
 * @brief Sends a debug message to indicate BMS is ready over CAN
 */
void debugCAN::bms_ready() {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = BMS_READY;
    txmsg.can_dlc = 0;
    
    can_interface -> sendMessage(&txmsg);
}

/**
 * @brief Sends a debug message indicating program is awaiting BMS ready over CAN
 */
void debugCAN::bms_await() {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = BMS_AWAIT;
    txmsg.can_dlc = 0;
    
    can_interface -> sendMessage(&txmsg);
}

/**
 * @brief Sends debug pedal input message over CAN
 * @param pedal1 Raw 5V ADC value
 * @param pedal2 Raw 3.3V ADC value
 * @param pedal1_scaled Scaled 5V ADC value
 * @param pedal2_scaled Scaled 3.3V ADC value
 */
void debugCAN::pedal_values(uint16_t pedal1, uint16_t pedal2, uint16_t pedal1_scaled, uint16_t pedal2_scaled) {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = PEDAL_VAL;
    txmsg.can_dlc = 8;
    
    txmsg.data[0] = pedal1 && 0xFF;
    txmsg.data[1] = (pedal1 >> 8) && 0xFF;
    txmsg.data[2] = pedal2 && 0xFF;
    txmsg.data[3] = (pedal2 >> 8) && 0xFF;
    txmsg.data[4] = pedal1_scaled && 0xFF;
    txmsg.data[5] = (pedal1_scaled >> 8) && 0xFF;
    txmsg.data[6] = pedal2_scaled && 0xFF;
    txmsg.data[7] = (pedal2_scaled >> 8) && 0xFF;

    can_interface -> sendMessage(&txmsg);
}

/**
 * @brief Sends a debug message containing the car status over CAN
 * @param car_status_ current car status at the start of each loop
 */
void debugCAN::status_car(CarStatus car_status_) {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = CAR_STATE;
    txmsg.can_dlc = 1;
    
    txmsg.data[0] = car_status_ && 0xFF;

    can_interface -> sendMessage(&txmsg);
}

/**
 * @brief Sends a debug car status fault message over CAN 
 */
void debugCAN::status_fault() {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = FAULT_DATA;
    txmsg.can_dlc = 1;

    txmsg.data[0] = STATUS_INVALID;

    can_interface -> sendMessage(&txmsg);
}

/**
 * @brief Sends debug pedal fault message over CAN
 * @param fault_status_ current pedal fault status as defined in fault_status enum
 */
void debugCAN::pedal_fault(fault_status fault_status_) {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = FAULT_DATA;
    txmsg.can_dlc = 1;
    
    txmsg.data[0] = fault_status_;
    
    can_interface -> sendMessage(&txmsg);
}

/**
 * @brief Sends debug pedal fault message over CAN
 * Intended for usage inside detectFault only
 * Sends the scaled analog difference between the two pedals as well as status 
 * @param fault_status_ current pedal fault status as defined in fault_status enum
 * @param dapps scaled anlog pedal difference
 */
void debugCAN::pedal_fault(fault_status fault_status_, int32_t dapps) {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = FAULT_DATA;
    txmsg.can_dlc = 5;
    
    txmsg.data[0] = fault_status_;
    txmsg.data[1] = dapps && 0xFF;
    txmsg.data[2] = (dapps >> 8) && 0xFF;
    txmsg.data[3] = (dapps >> 16) && 0xFF;
    txmsg.data[4] = (dapps >> 24) && 0xFF;

    can_interface -> sendMessage(&txmsg);
}
