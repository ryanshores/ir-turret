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

//////////////////////////////////////////////////
//  LIBRARIES  //
//////////////////////////////////////////////////
#include <Arduino.h>
#include <IRremote.hpp>
#include <ServoControl/ServoControl.h>

#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include "RemoteDefinitions.h"

auto servo_control = ServoControl(10,11,12);

//////////////////////////////////////////////////
//  S E T U P  //
//////////////////////////////////////////////////
void setup() {
    Serial.begin(9600); // initializes the Serial communication between the computer and the microcontroller

    // Just to know which program is running on my microcontroller
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(9, ENABLE_LED_FEEDBACK);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.println(F("at pin " STR(9)));

    servo_control.setup();
}

////////////////////////////////////////////////
//  L O O P  //
////////////////////////////////////////////////

void loop() {
    /*
    * Check if received data is available and if yes, try to decode it.
    */
    if (IrReceiver.decode()) {
        /*
        * Print a short summary of received data
        */
        IrReceiver.printIRResultShort(&Serial);
        IrReceiver.printIRSendUsage(&Serial);
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            //command garbled or not recognized
            Serial.println(F(
                "Received noise or an unknown (or not yet enabled) protocol - if you wish to add this command, define it at the top of the file with the hex code printed below (ex: 0x8)"));
            // We have an unknown protocol here, print more info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        Serial.println();

        /*
        * !!!Important!!! Enable receiving of the next value,
        * since receiving has stopped after the end of the current received data packet.
        */
        IrReceiver.resume(); // Enable receiving of the next value


        /*
        * Finally, check the received data and perform actions according to the received command
        */

        switch (IrReceiver.decodedIRData.command) {
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
    }
    delay(5);
}
