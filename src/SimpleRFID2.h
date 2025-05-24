/*
    SimpleRFID2
    Library intended for easy use of the "Grove-125KHz RFID Reader v1.0"
    all RDM6300 readers should work.

    based on
    - SimpleRFID
        by Roman Schulte and Luai Malek, April 4, 2019.
        (public domain/ MIT)
    - www.mschoeffler.de Arduino Examples
        https://mschoeffler.com/2018/01/05/arduino-tutorial-how-to-use-the-rdm630-rdm6300-rfid-reader/
    - Amir Mohammad Shojaee @ Electropeak
        modified on 20 Jan 2020
        https://electropeak.com/learn/interfacing-rdm6300-125khz-rfid-reader-module-with-arduino/
    - RFID.h
        20220531 s-light.eu stefan krüger
        extracted RFID code into extra file and some light tweaks.
        https://github.com/s-light/MYS__52-RFID-Leser/blob/master/examples/RFID_125kHz_RDM6300__withIdDecoding/RFID_125kHz_RDM6300__withIdDecoding.ino

    converted to SimpleRFID2 Library
        by s-light.eu stefan krüger
        24.05.2025


*/

#ifndef SIMPLE_RFID2_H
#define SIMPLE_RFID2_H

#include "Arduino.h"
#include "SoftwareSerial.h"

// slightly advanced handling.
class SimpleRFID2 {
public:
    // Constructor
    SimpleRFID2(uint8_t rx_pin, uint8_t tx_pin);

    // Returns whether the reader is holding data, ready to be read
    bool available();

    // returns the ID as string with leading '0'
    // String readID();

    // returns the last read ID as unsigned long integer (if reading / checksum
    // failed it returns 0)
    uint32_t readDecodedID();

    // convert decimal to String and fill with leading '0'
    static String tagDecimalToString(uint32_t decimal, uint8_t fillWidth=10);

    // Destructor
    ~SimpleRFID2();

private:
    SoftwareSerial *_simpleRFID_SoftSerial;

    // decoding info
    // https://forum.arduino.cc/t/rdm6300-reading-format/1072597/10
    // https://elty.pl/upload/download/RFID/RDM630-Spec.pdf
    // RFID DATA FRAME FORMAT:
    //      1byte head (value: 2),
    //      10byte data (2byte version + 8byte tag),
    //      2byte checksum,
    //      1byte tail (value: 3)
    static const int BUFFER_SIZE = 14;
    // 2byte version (actual meaning of these two bytes may vary)
    static const int DATA_VERSION_SIZE = 2;
    // 8byte tag
    static const int DATA_TAG_SIZE = 8;
    // 10byte data (2byte version + 8byte tag)
    static const int DATA_SIZE = DATA_VERSION_SIZE + DATA_TAG_SIZE;
    // 2byte checksum
    static const int CHECKSUM_SIZE = 2;
    static const int PAYLOAD_SIZE = DATA_SIZE + CHECKSUM_SIZE;

    static const int MSG_HEAD = 0x02;
    static const int MSG_TAIL = 0x03;

    // unions
    // https://legacy.cplusplus.com/doc/tutorial/other_data_types/
    // to hold the incoming message.
    // this way it is easy to access the different parts.
    union data_t {
        char raw[DATA_SIZE];
        struct {
            char version[DATA_VERSION_SIZE];
            char tag[DATA_TAG_SIZE];
        };
    };
    union message_t {
        char raw[PAYLOAD_SIZE];
        struct {
            // char head;
            data_t data;
            char checksum[CHECKSUM_SIZE];
            // char tail;
        };
    };

    // used to store an incoming data frame
    message_t buffer;
    int buffer_index = 0;

    uint32_t extract_tag(bool detailed_output = false);

    // converts a hexadecimal value (encoded as ASCII string) to a numeric
    // value
    static uint32_t hexStrToDecimal(char *str, unsigned int length);

};

#endif
