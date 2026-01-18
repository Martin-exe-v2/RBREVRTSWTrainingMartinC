#ifndef PINMAP_H
#define PINMAP_H

// CAN bus pins
#define CS_CAN_MOTOR PB2    // motor
#define CS_CAN_BMS PB1      // battery management system
#define CS_CAN_DEBUG PB0    // debug

// APPS (accelerator pedal position sensor) pins
#define APPS_5V PC0         // pedal 5V
#define APPS_3V3 PC1        // pedal 3.3V
// Brake pins
#define BRAKE_LIGHT PD2     // brake light
#define BRAKE_IN PC3        // brakes

// Drive mode pins
#define DRIVE_MODE_LED PD3  // drive mode indicator
#define START_BTN PC4       // start button

// Buzzer pin
#define BUZZER_OUT PD4      // buzzer

#endif // PINMAP_H