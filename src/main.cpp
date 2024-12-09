/*

************************************************************************************
* MIT License
*
* Copyright (c) 2023 Crunchlabs LLC (IRTurret Control Code)
* Copyright (c) 2020-2022 Armin Joachimsmeyer (IRremote Library)

* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is furnished
* to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
* OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
************************************************************************************
*/

#include <Arduino.h>
#include <ServoControl/ServoControl.h>
#include <Remote/Remote.h>

#include "RemoteDefinitions.h"

auto remote = Remote(9);
auto servo_control = ServoControl(10,11,12);

void setup() {
    Serial.begin(9600); // initializes the Serial communication between the computer and the microcontroller
    remote.setup();
    servo_control.setup();
}

void loop() {
    switch (remote.get_command()) {
        //this is where the commands are handled

        case up: //pitch up
            servo_control.upMove(1);
            break;

        case down: //pitch down
            servo_control.downMove(1);
            break;

        case left: //fast counterclockwise rotation
            servo_control.leftMove(1);
            break;

        case right: //fast clockwise rotation
            servo_control.rightMove(1);
            break;

        case ok: //firing routine
            servo_control.fire();
            break;

        case star:
            servo_control.fireAll();
            delay(50);
            break;

        case cmd1:
            servo_control.shakeHeadYes();
            break;

        default:
            break;
    }
    delay(5);
}
