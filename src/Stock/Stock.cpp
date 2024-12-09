//
// Created by Ryan Shores on 12/9/24.
//
#include <Arduino.h>
#include <ServoControl/ServoControl.h>
#include <Remote/Remote.h>

#include "Stock.h"
#include "RemoteDefinitions.h"

auto remote = Remote(9);
auto servo_control = ServoControl(10,11,12);

void RunArduino::setup() {
    remote.setup();
    servo_control.setup();
}

void RunArduino::loop() {
    switch (remote.get_command()) {
        //this is where the commands are handled

        case up:
            servo_control.upMove(1);
            break;
        case down:
            servo_control.downMove(1);
            break;
        case left:
            servo_control.leftMove(1);
            break;
        case right:
            servo_control.rightMove(1);
            break;
        case ok:
            servo_control.fire();
            break;
        case star:
            servo_control.fireAll();
            delay(50);
            break;
        case cmd1:
            servo_control.shakeHeadYes();
            break;
        case cmd2:
            servo_control.shakeHeadNo();
        case cmd3:
            servo_control.randomRoulette();

        default:
            break;
    }
    delay(5);
}