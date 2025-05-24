/*
    SimpleRFID2

    based on 
    - SimpleRFID by Roman Schulte and Luai Malek, April 4, 2019

*/

#include "SimpleRFID2.h"
#include "Arduino.h"
#include <SoftwareSerial.h>

SimpleRFID2::SimpleRFID2(uint8_t rx_pin, uint8_t tx_pin) {
    _simpleRFID_SoftSerial = new SoftwareSerial(rx_pin, tx_pin);
    // The RFID readers baudrate is fixed at 9600
    _simpleRFID_SoftSerial->begin(9600);
    // activate
    _simpleRFID_SoftSerial->listen();
}

SimpleRFID2::~SimpleRFID2() { delete _simpleRFID_SoftSerial; }

bool SimpleRFID2::available() {
    return _simpleRFID_SoftSerial->available() > 0;
}

// uint32_t SimpleRFID2::readDecodedID() {
//     String result = "";
//     unsigned char temp_char;
//     int check_int;
//     do {
//         temp_char = (unsigned char)_simpleRFID_SoftSerial->read();
//         // used to check, if temp_char is valid data
//         check_int = (int)temp_char;
//         // Control-characters are rendered invalid data.
//         // Consult ASCII-table for further information.
//         if (check_int != 2 && check_int != 3 && check_int != 255) {
//             // append valid character to output-string
//             result += (char)temp_char;
//         }
//     } while (check_int != 3);
//     // the control-character "end of message" ends the reading process
//     while (_simpleRFID_SoftSerial->available()) {
//         // clear input buffer in case the reader read twice
//         char junk = _simpleRFID_SoftSerial->read();
//     }
//     return result;
// }

// uint32_t SimpleRFID2::readDecodedID(bool detailed_output = false) {
uint32_t SimpleRFID2::readDecodedID() {
    // returns tag id if tag is read. 0 otherwise.
    bool detailed_output = false;
    int32_t tag = 0;
    while (_simpleRFID_SoftSerial->available() > 0) {
        bool call_extract_tag = false;

        int inputChar = _simpleRFID_SoftSerial->read(); // read
        // Serial.print("inputChar : ");
        // Serial.print(inputChar, DEC);
        // Serial.print(" - 0x");
        // Serial.print(inputChar, HEX);
        // Serial.print(" - ");
        // Serial.print(char(inputChar));
        // Serial.println("");
        if (inputChar == -1) {
            // no data was read
            // should not happen - as we first check for available...
        } else {
            // RDM630/RDM6300 found a tag => tag incoming
            if (inputChar == MSG_HEAD) {
                buffer_index = 0;
            } else if (inputChar == MSG_TAIL) {
                // tag has been fully transmitted
                // extract tag at the end of the function call
                call_extract_tag = true;
            } else {
                // checking for a buffer overflow (It's very unlikely that
                // an buffer overflow comes up!)
                if (buffer_index >= BUFFER_SIZE) {
                    // Serial.println("Error: Buffer overflow detected! ");
                } else {
                    // everything is alright => copy current value to buffer
                    buffer.raw[buffer_index++] = inputChar;
                }
            }

            if (call_extract_tag == true) {
                if (buffer_index == PAYLOAD_SIZE) {
                    tag = extract_tag(detailed_output);
                } else {
                    // something is wrong...
                    // start again looking for MSG_HEAD
                    buffer_index = 0;
                    return;
                }
            }
        }
    }
    return tag;
}

uint32_t SimpleRFID2::extract_tag(bool detailed_output = false) {
    uint32_t tagDecimal = hexStrToDecimal(buffer.data.tag, DATA_TAG_SIZE);
    uint32_t checksumDecimal = hexStrToDecimal(buffer.checksum, CHECKSUM_SIZE);

    // Serial.println("calculate decimal checksum: ");
    uint32_t checksumDecimalCalculated = 0;
    for (int i = 0; i < DATA_SIZE; i += 2) {
        // why ever - this does not work..
        // uint32_t val = hexStrToDecimal(buffer.data.raw[i], 2);
        // this AI generated code snippet works as expected.
        // should be the same...
        char byteStr[3];
        byteStr[0] = buffer.data.raw[i];
        byteStr[1] = buffer.data.raw[i + 1];
        byteStr[2] = '\0'; // null-terminate the string
        uint8_t byteValue = (uint8_t)strtol(byteStr, NULL, 16);

        // Serial.print("  ");
        // Serial.print(i);
        // Serial.print(" ");
        // Serial.print("'");
        // Serial.print(char(buffer.data.raw[i]));
        // Serial.print("'");
        // Serial.print(" → ");
        // Serial.print(val);
        // Serial.print(" → ");
        // Serial.print(byteValue);
        // Serial.println();
        checksumDecimalCalculated ^= byteValue;
    }

    // print message that was sent from RDM630/RDM6300
    if (detailed_output) {
        Serial.println("------------------------------------------");

        Serial.println("Buffer content:");
        Serial.print("HEX  : ");
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            Serial.print(buffer.raw[i]);
            Serial.print(" ");
        }
        Serial.println(";");
        Serial.print("char  : ");
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            Serial.print(char(buffer.raw[i]));
            Serial.print(" ");
        }
        Serial.println(";");

        Serial.println("------------------------------------------");

        Serial.println("DATA content:");

        Serial.print("ASCII: ");
        for (int i = DATA_VERSION_SIZE; i < DATA_SIZE; ++i) {
            Serial.print(char(buffer.raw[i]));
            Serial.print(" ");
        }
        Serial.println("; char()");

        Serial.print("ASCII: ");
        for (int i = 0; i < DATA_SIZE; ++i) {
            Serial.print(char(buffer.data.raw[i]));
            Serial.print(".");
        }
        Serial.println("; char()");

        Serial.println("------------------------------------------");
        // Serial.print("hexCStrToString: ");
        // Serial.print(hexCStrToString(buffer));

        Serial.println("Message (HEX): ");

        // Serial.print("  head    : ");
        // Serial.print(char(buffer.head));
        // Serial.println("");

        Serial.print("  version : ");
        for (int i = 0; i < DATA_VERSION_SIZE; ++i) {
            Serial.print(char(buffer.data.version[i]));
            Serial.print(" ");
        }
        Serial.println("");

        Serial.print("  tag     : ");
        for (int i = 0; i < DATA_TAG_SIZE; ++i) {
            Serial.print(char(buffer.data.tag[i]));
            Serial.print(" ");
        }
        Serial.println("");

        Serial.print("  checksum: ");
        for (int i = 0; i < CHECKSUM_SIZE; ++i) {
            Serial.print(char(buffer.checksum[i]));
        }
        Serial.println("");

        // Serial.print("  tail    : ");
        // Serial.print(char(buffer.tail));
        // Serial.println("");

        Serial.println("------------------------------------------");

        Serial.print("Extracted Tag: ");
        Serial.println(tagDecimal);

        Serial.println(tagDecimalToString(tagDecimal));

        Serial.println("checksum: ");
        Serial.print("  ");
        // compare calculated checksum to retrieved checksum
        if (checksumDecimalCalculated == checksumDecimal) {
            // checksum matches.
            Serial.print("OK");
        } else {
            // checksums mismatch
            Serial.print("mismatch");
            Serial.println();
            Serial.print("  checksumDecimal: ");
            Serial.println(checksumDecimal);
            Serial.print("  checksumDecimalCalculated: ");
            Serial.println(checksumDecimalCalculated);
        }
        Serial.println();

        Serial.println("------------------------------------------");
    }

    if (checksumDecimalCalculated != checksumDecimal) {
        // checksum mismatch
        // so we return 0.
        tagDecimal = 0;
    }
    return tagDecimal;
}

// converts a hexadecimal value (encoded as ASCII string) to a numeric value
uint32_t SimpleRFID2::hexStrToDecimal(char *str, unsigned int length) {

    char *copy = malloc((sizeof(char) * length) + 1);
    // the variable "copy" is a copy of the parameter "str".
    memcpy(copy, str, sizeof(char) * length);
    // "copy" has an additional '\0' element to make sure that "str" is
    // null-terminated.
    copy[length] = '\0';
    // strtol converts a null-terminated string to a long value
    uint32_t value = strtol(copy, NULL, 16);
    // clean up
    free(copy);
    return value;
}

String SimpleRFID2::tagDecimalToString(uint32_t decimal, uint8_t fillWidth) {
    String decimalStr = String(decimal);
    while (decimalStr.length() < fillWidth) {
        decimalStr = "0" + decimalStr;
    }
    return decimalStr;
}
