/*
    Prints out the ID of any chip you scan.

    Created on 4 Apr 2019 by
    Luai Malek and
    Roman Schulte
*/

#include "SimpleRFID.h"

String ID = "";

// Setup RFID reader on pin 2 and 3 (D2 on base shield).
const uint8_t rfid_rx_pin = 2;
const uint8_t rfid_tx_pin = 3;
SimpleRFID simple_rfid(rfid_rx_pin, rfid_tx_pin);

void setup() {
    Serial.begin(9600);
    Serial.println("SimpleRead.ino");
}

void loop() {
    // check if data is ready to be read
    if (simple_rfid.available()) {
        // read the ID
        ID = simple_rfid.readID();
        // print the String
        Serial.println(ID);
    }

    // Here you can add additional behaviour if wanted.
}
