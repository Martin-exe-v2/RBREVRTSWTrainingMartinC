#ifndef DEBUG_CAN_HPP
#define DEBUG_CAN_HPP

#include "enums.h"
#include <mcp2515.h>

namespace debugCAN 
{
    extern MCP2515 *can_interface;

    void init(MCP2515 *can_interface);

    void motor_stop();
    void motor_torque(int16_t torque_);

    void bms_ready();
    void bms_await();

    void pedal_values(uint16_t pedal1, uint16_t pedal2, uint16_t pedal1_scaled, uint16_t pedal2_scaled);
    void status_car(CarStatus car_status_);
    void status_fault();
    void pedal_fault(fault_status fault_status_);
    void pedal_fault(fault_status fault_status_, int32_t dapps);
} // namespace debugCAN 

/*
motor:
stop                    29F
torque value            290
bms:
ready                   B50
await                   B5F
debug:
pedal value             D01
car status              D02
state fault             BAD
pedal fault             BAD
pedal fault dapps       BAD
pedal fault rising      BAD
pedal fault exceed      BAD
pedal fault falling     BAD
*/
#endif // DEBUG_CAN_HPP