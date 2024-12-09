//
// Created by Ryan Shores on 12/8/24.
//
#include "IRRemote.h"
#include "StringHelper.h"

IRRemote::IRRemote(int pin) {
  this -> pin = pin;
}

void IRRemote::setup() {
  // Just to know which program is running on my microcontroller
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(pin, ENABLE_LED_FEEDBACK);

  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.println(F("at pin" STR(pin)));
}

bool IRRemote::decode() {
  if (!IrReceiver.decode()) return false;

  // Print a short summary of received data
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

  return true;
}

uint16_t IRRemote::get_command() {
  return IrReceiver.decodedIRData.command;
}