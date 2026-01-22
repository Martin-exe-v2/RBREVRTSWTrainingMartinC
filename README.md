# RBREVRTSWTrainingMartinChong

<pseudocode>

startup:
state = INIT


switch:
    if brakes > threshold:
        brake_light = high
    state == INIT
        motor = 0
        if start_button down and brakes > threshold:
            status_timestamp = ms()
            state = STARTING
    state == STARTING
        motor = 0
        if start_button down and brakes > threshold:
            if ms()-status_timestamp > 2000 and CAN.readfiltered(0x186040F3).
                status_timestamp = ms()
                state = BUZZING
        else:
            state = INIT
    state == BUZZING
        buzzer = HIGH
        if ms()-status_timestamp > 2000:
            state = DRIVE
    state == DRIVE
        get pedal in
        scale inputs to [0,1]
        if abs(dinputs) > 0.1:
            if !fault:
                fault = true
                time_fault = ms()
            elif ms() - time_fault > 100:
                brick motor
                state = INIT
        if fault:
            fault = false
            torque = f(pedals)
    default:
        set state to undefined
        brick motor

        




</pseudocode>