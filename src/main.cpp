#include <Arduino.h>
#include <mcp2515.h>
#include "pinMap.h"
#include "enums.h"

// pin setup
const int INPUT_PIN_COUNT =2;
uint8_t pin_in[INPUT_PIN_COUNT] = {START_BTN, BRAKE_IN};
const int OUTPUT_PIN_COUNT =3;
uint8_t pin_out[OUTPUT_PIN_COUNT] = {BRAKE_LIGHT, DRIVE_MODE_LED, BUZZER_OUT};

// 

void setup()
{
    // add shit
}

void loop()
{
    // some stuff

    // if brake > threshold:
    //     brake light on

    switch (car_status) {
        case INIT: {
            // set motor to 0
            // if start button down AND brakes depressed:
            //     set status to STARTING
        }
        break;
        case STARTING: {
            // set motor to 0
            // if start button not down OR brakes released:
            //     set status to INIT
            // if (start button down AND brakes held) for >= 2s:
            //     set status to BUZZING 
        }
        break;
        case BUZZING: {
            // set buzzer on
            // if 2s passed:
            //     set status to DRIVE
        }
        break;
        case DRIVE: {
            // drive mode LED on
            // read pedal 5V
            // read pedal 3V3
            // scale pedals to common range (R)
            // if scaled pedals/R differ by >0.1:
            //     pedals are faulty
            // else:
            //     pedals are not faulty
            // if (pedals are faulty) for > 100ms:
            //     set motor to 0
            //     set status to INIT
            // else:
            //     torque = f(pedals)
        }
        break;
        default:{
            // error handling
            // should include set motor to 0
        }
    }
}