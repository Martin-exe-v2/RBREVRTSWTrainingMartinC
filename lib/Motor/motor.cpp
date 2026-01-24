/**
 * @file motor.cpp
 * @author Martin C
 * @brief Implementation of Motor class for handling sending CAN messages
 * @version 1.4
 * @date 2026-01-22
 * @see motor.hpp
 */
#include "motor.hpp"
#include "debug.hpp"


/**
 * @brief Constructor of the Motor class
 * Initialises the motor CAN frame.
 * @param motor_can_
 */
Motor::Motor(MCP2515 &motor_can_) :
motor_can(motor_can_)
{
    torque_frame.can_id = MOTOR_COMMAND;
    torque_frame.can_dlc = 3;
    torque_frame.data[0] = 0x90;
}

/**
 * @brief Sends stop frame to motor
 * Different from setTorque(0) as it sends the frame immediately. Also logs a debug message.
 */
void Motor::stop() { // stops motor
    motor_can.sendMessage(&stop_frame);
    Motor::setTorque(0);
    DBG_MOTOR_STOP();
}

/**
 * @brief Updates stored torque value
 * Does not convert pedal to torque.
 * Does not send CAN frame, only the value stored in the class.
 * @param torque_ 
 */
void Motor::setTorque(int16_t torque_) {
    torque_out = torque_;
}

/**
 * @brief Updates torque value for motor
 * Will use the last stored torque value.
 */
void Motor::update() {
    torque_frame.data[1] = torque_out & 0xFF;
    torque_frame.data[2] = (torque_out >> 8) & 0xFF;
    motor_can.sendMessage(&torque_frame);
    DBG_MOTOR_TORQUE(torque_out);
}