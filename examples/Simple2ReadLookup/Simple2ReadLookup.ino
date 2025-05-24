/*
    read tag and check if it is a known *Person*.
*/

#include "SimpleRFID2.h"

// Setup RFID reader on pin 2 and 3 (D2 on base shield).
const uint8_t rfid_rx_pin = 2;
const uint8_t rfid_tx_pin = 3;
SimpleRFID2 simple_rfid(rfid_rx_pin, rfid_tx_pin);

struct person_t {
    uint32_t tag;
    String name;
    uint8_t age;
};

const person_t person_list[] = {
    {7569585, "Bob", 17},
    {7562924, "Martin", 35},
    {334155100, "Sabine", 17},
    {334016718, "Melanie", 13}, 
    {7768928, "Rudolf", 9},
    {13007523, "Monster", 10},
    {6654217, "Juliane", 22},
    // {355189219, "Laura", 11},
};
// get size of memory for person, divide by size of type to get count...
const uint8_t person_list_COUNT = sizeof(person_list) / sizeof(person_t);

void setup() {
    Serial.begin(115200);
    Serial.println("Simple2ReadLookup.ino");
}

void loop() {
    if (simple_rfid.available()) {
        uint32_t id = simple_rfid.readDecodedID();
        if (id > 0) {
            tag_found(id);
        }
    }
}

void tag_found(uint32_t tag) {
    int8_t person_index = find_person_index(tag);
    if (person_index != -1) {
        Serial.print("Found Person: ");
        Serial.print(person_list[person_index].name);
        Serial.print(" (");
        Serial.print(person_list[person_index].age);
        Serial.print(")");
        Serial.println();
    } else {
        Serial.print("Tag Unknown: ");
        Serial.print(tag);
        Serial.println();
    }
}

int8_t find_person_index(int32_t tag) {
    // we return -1 if we do not find a match...
    int8_t person_index = -1;
    for (size_t i = 0; i < person_list_COUNT; i++) {
        if (person_list[i].tag == tag) {
            person_index = i;
        }
    }
    return person_index;
}
