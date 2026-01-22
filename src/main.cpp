#include <Arduino.h>
#include <mcp2515.h>
#include "pinMap.h"
#include "enums.h"
#include "pedal.h"
#include "motor.hpp"
#include "debug.hpp"

// I HATE WRITING MILLIS
#define ms millis()

// pin setup
constexpr int INPUT_PIN_COUNT = 4;
constexpr uint8_t pin_in[INPUT_PIN_COUNT] = {START_BTN, BRAKE_IN, APPS_5V, APPS_3V3};
constexpr int OUTPUT_PIN_COUNT = 3;
constexpr uint8_t pin_out[OUTPUT_PIN_COUNT] = {BRAKE_LIGHT, DRIVE_MODE_LED, BUZZER_OUT};

// settings
constexpr uint16_t STARTING_DELAY = 2000;   // ms, duration the start button needs to be held down for
constexpr uint16_t BUZZING_DELAY = 2000;    // ms, duration to hold buzzer on
constexpr int MIN_FAULT_DURATION = 100;     // ms, duration for pedal_fault to be raised before it registers as actual fault
constexpr uint16_t BRAKE_THRESHOLD = 6942;  // arbritrary units

// CAN setup
MCP2515 motor_can(CS_CAN_MOTOR);
MCP2515 BMS_can(CS_CAN_BMS);
MCP2515 debug_can(CS_CAN_DEBUG);
constexpr int CAN_COUNT = 3;
MCP2515 MCPS[CAN_COUNT] = {motor_can, BMS_can, debug_can};
struct can_frame BMS_frame;

// variable setup
CarStatus car_status = INIT;
uint32_t status_timestamp = 0;
uint32_t fault_timestamp = 0;
uint16_t pedal_out = 0;
uint16_t pedal_extra = 0;
bool brake_pressed = false;
bool pedal_fault = true;

// objects
Motor motor(motor_can);

void setup()
{
    // pin config

    // init inputs
    for (int i = 0; i < INPUT_PIN_COUNT; i++) {
        pinMode(pin_in[i], INPUT);
    }
    // init outputs
    for (int i = 0; i < OUTPUT_PIN_COUNT; i++) {
        pinMode(pin_out[i], OUTPUT);
    }
    // init CAN
    for (int i = 0; i < CAN_COUNT; i++) {
        MCPS[i].reset();
        MCPS[i].setBitrate(CAN_500KBPS);
        MCPS[i].setNormalMode();
    }
}

void loop()
{
    DBG_TIMESTAMP(ms);
    motor.update();
    DBG_STATUS(car_status);
    // brakes
    brake_pressed = static_cast<uint16_t>(analogRead(BRAKE_IN)) >= BRAKE_THRESHOLD;
    digitalWrite(BRAKE_LIGHT, brake_pressed);
    
    switch (car_status) {
        case INIT: {
            motor.stop();
            if (digitalRead(START_BTN) == true && brake_pressed) {
                status_timestamp = ms;
                car_status = STARTING;
            }
        }
            break;
        case STARTING: {
            motor.stop();
            if (digitalRead(START_BTN) == true && brake_pressed) {
                if (ms - status_timestamp >= STARTING_DELAY) {
                    if (BMS_can.readMessage(&BMS_frame) == MCP2515::ERROR_OK) {
                        if (BMS_frame.can_id == BMS_COMMAND && BMS_frame.data[6] == 0x50) {
                            DBG_BMS_READY();
                            car_status = BUZZING;
                            status_timestamp = ms;
                        }
                        else {
                            DBG_BMS_AWAIT();
                        }
                    }
                }
            }
            else {
                car_status = INIT;
            }
        }
            break;
        case BUZZING: {
            digitalWrite(BUZZER_OUT, HIGH);
            if (ms - status_timestamp >= BUZZING_DELAY) {
                status_timestamp = ms;
                car_status = DRIVE;
            }
        }
            break;
        case DRIVE: {
            digitalWrite(DRIVE_MODE_LED, HIGH);
            pedal_out = analogRead(APPS_5V);
            pedal_extra = analogRead(APPS_3V3);
            if (detectFault(pedal_out, pedal_extra)) { // fault frame for DIFF is sent automatically if detected
                if (!pedal_fault){
                    DBG_PEDAL_FAULT(DIFF_RISING);
                    fault_timestamp = ms;
                    pedal_fault = true;
                }
                else if (ms - fault_timestamp > MIN_FAULT_DURATION) {
                    DBG_PEDAL_FAULT(DIFF_EXCEED_DURATION);
                    motor.stop();
                    car_status = INIT;
                    break;
                }
            }
            else {
                DBG_PEDAL_FAULT(DIFF_FALLING);
            }
            motor.setTorque(torqueMap(pedal_out));
        }
        break;
        default: {
            // error handling
            motor.stop();
            DBG_STATUS_FAULT();
            car_status = PROBLEM;
        }
            break;
    }
}