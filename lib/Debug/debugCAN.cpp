#include "debugCAN.hpp"

MCP2515 *can_interface;

void debugCAN::init(MCP2515 *can) {
    if (can == nullptr) {
        return;
    }
    can_interface = can;
}


void debugCAN::motor_stop() {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = MOTOR_STOP;
    txmsg.can_dlc = 0;

    can_interface -> sendMessage(&txmsg);
}

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


void debugCAN::bms_ready() {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = BMS_READY;
    txmsg.can_dlc = 0;
    
    can_interface -> sendMessage(&txmsg);
}

void debugCAN::bms_await() {
    if (!can_interface) {
        return;
    }
    
    struct can_frame txmsg;
    txmsg.can_id = BMS_AWAIT;
    txmsg.can_dlc = 0;
    
    can_interface -> sendMessage(&txmsg);
}


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
