# Simple2ReadLookup

This example demonstrates how you can use `SimpleRFID2` to read the ID from a near RFID tag 
and print additional (hard-coded) information from a *lookup*

## Materials

To run this example you need these components:

-   [Arduino](https://store.arduino.cc/arduino-genuino/boards-modules)
-   [Groove RFID Reader](http://wiki.seeedstudio.com/Grove-125KHz_RFID_Reader/)
-   At least one RFID tag (chip or card)
-   Grove-Kabel
-   [Grove Shield](http://wiki.seeedstudio.com/Base_Shield_V2/)

## Setup

1. Open the example skecth by selecting `File > Examples > SimpleRFID > Simple2Read`
2. Connect the RFID-reader to your Arduino (RX to PIN2, TX to PIN3 → Port Groove D2)
3. Connect your Ardunio to your PC
4. Upload the code to your Arduino
5. Open the serial monitor by selecting `Tools > Serial Monitor`

## Usage

Hold a RFID tag close to the RFID reader and look at the monitor.
The monitor should print the ID of the tag.
if the tag is *known* the `Name` and `Age` of the person is printed.
you have to edit the `person_list` so that your tag-ids are *known*.
the IDs should be printed on the physical tags. 
They are maximal 10 characters long - leading zeros are ignored.
