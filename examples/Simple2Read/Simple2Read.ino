/*
    Prints out the ID of any chip you scan.

    based on 4 Apr 2019 by
    Luai Malek and
    Roman Schulte
    modified for SimpleRFID2 by Stefan Krüger
*/

#include "SimpleRFID2.h"

uint32_t id = "";

// Setup RFID reader on pin 2 and 3 (D2 on base shield).
const uint8_t rfid_rx_pin = 2;
const uint8_t rfid_tx_pin = 3;
SimpleRFID2 simple_rfid(rfid_rx_pin, rfid_tx_pin);

void setup() {
    Serial.begin(115200);
    Serial.println("Simple2Read.ino");
}

void loop() {
    // check if data is ready to be read
    if (simple_rfid.available()) {
        // read the id
        id = simple_rfid.readDecodedID();
        // check again vor valid id / tag
        if (id > 0) {
            Serial.print("Found Tag: ");
            Serial.print(id);
            Serial.println();
        }
    }
    // Here you can add additional behaviour if wanted.
}
