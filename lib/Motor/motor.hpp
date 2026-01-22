#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "enums.h"
#include <mcp2515.h>

class Motor
{
    public:
    Motor(MCP2515 &motor_can_);
    void setTorque(int16_t torque_);
    void stop();
    void update();
    private:
    int16_t torque_out;
    MCP2515 &motor_can;
    can_frame torque_frame;

    const can_frame stop_frame {
        MOTOR_COMMAND,  // can_id
        3,              // dlc
        0x90,           // data, torque command
        0x00,           // data, 0 torqueL
        0x00            // data, 0 torqueH
    };
};

#endif // MOTOR_HPP