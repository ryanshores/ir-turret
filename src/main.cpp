#include <Arduino.h>
#include <Stock/Stock.h>

Stock stock;

void setup() {
    Serial.begin(9600); // initializes the Serial communication between the computer and the microcontroller
    stock.setup();
}

void loop() {
    stock.loop();
}
