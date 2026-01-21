#include <Arduino.h>
#include <mcp2515.h>
#include "pinMap.h"
#include "enums.h"
#include "pedal.h"

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
MCP2515 mcp2515_motor(CS_CAN_MOTOR);
MCP2515 mcp2515_BMS(CS_CAN_BMS);
MCP2515 mcp2515_debug(CS_CAN_DEBUG);

// variable setup
CarStatus car_status = INIT;
uint16_t status_timestamp = 0;
uint16_t fault_timestamp = 0;
uint16_t pedal_out = 0;
int16_t torque_out = 0;
bool brake_pressed = false;
bool pedal_fault = true;

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
}

void loop()
{
    // brakes
    brake_pressed = static_cast<uint16_t>(analogRead(BRAKE_IN)) >= BRAKE_THRESHOLD;
    digitalWrite(BRAKE_LIGHT, brake_pressed);

    switch (car_status) {
        default: {
            // error handling
            car_status = PROBLEM;
            // motor.stop();
        }
            break;
        case INIT: {
            // motor.stop();
            if (digitalRead(START_BTN) == true && brake_pressed) {
                status_timestamp = ms;
                car_status = STARTING;
            }
        }
            break;
        case STARTING: {
            // motor.stop();
            if (digitalRead(START_BTN) == true && brake_pressed) {
                if (ms - status_timestamp >= STARTING_DELAY) {
                    // wait till BMS sends thing
                    car_status = BUZZING;
                    status_timestamp = ms;
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
            if (detectFault(pedal_out, analogRead(APPS_3V3))) {
                if (!pedal_fault){
                    fault_timestamp = ms;
                    pedal_fault = true;
                }
                else if (ms - fault_timestamp > MIN_FAULT_DURATION) {
                    // motor.stop();
                    car_status = INIT;
                    break;
                }
            }
            // motor.setTorque(torqueMap(pedal_out));
        }
            break;
    }
}