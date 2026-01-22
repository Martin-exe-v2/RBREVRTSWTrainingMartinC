#ifndef ENUMS_H
#define ENUMS_H

enum CarStatus
{
    INVALID = -1, // maximum minimal hamming distance to all other states
    INIT = 0,
    STARTING = 1,
    BUZZING = 2,
    DRIVE = 3,
};

enum MotorCANID
{
    MOTOR_COMMAND = 0x201,      // MOTOR
    MOTOR_STOP = 0x29F,         // DEBUG
    TORQUE_VAL = 0x290,         // DEBUG
};

enum BMSCANID
{
    BMS_COMMAND = 0x186040F3,   // BMS
    BMS_READY = 0xB50,          // DEBUG
    BMS_AWAIT = 0xB5F,          // DEBUG
};

enum DEBUGCANID
{
    PEDAL_VAL = 0xD01,          // DEBUG
    CAR_STATE = 0xD02,          // DEBUG
    FAULT_DATA = 0xBAD,         // DEBUG
};

enum fault_status
{
    NONE = 0x00,
    DIFF_DAPPS = 0x10,              //0000
    DIFF_RISING = 0x19,             //1001
    DIFF_EXCEED_DURATION = 0x1A,    //1010
    DIFF_FALLING = 0x1C,            //1100
    STATUS_INVALID = 0xFF,
};

#endif // ENUMS_H