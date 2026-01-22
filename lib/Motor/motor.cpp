#include "motor.hpp"

Motor::Motor(MCP2515 &motor_can_) :
motor_can(motor_can_)
{
    torque_frame.can_id = MOTOR_COMMAND;
    torque_frame.can_dlc = 3;
    torque_frame.data[0] = 0x90;
}

void Motor::stop() { // stops motor
    motor_can.sendMessage(&stop_frame);
    Motor::setTorque(0);
    // debug motor stop
}

void Motor::setTorque(int16_t torque_) {
    torque_out = torque_;
}

void Motor::update() {
    torque_frame.data[1] = torque_out & 0xFF;
    torque_frame.data[2] = (torque_out >> 8) & 0xFF;
    motor_can.sendMessage(&torque_frame);
    // debug motor torque value 
}