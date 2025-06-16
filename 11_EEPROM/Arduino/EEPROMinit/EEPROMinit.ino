// EEPROMinit.
//
// A small sketch to initialise the EEPROM with some data to be
// read using the EEPROMinterruptRead sketch. It uses the AVRlib
// to write the EEPROM data.
//
// Norman Dunbar
// Friday 13 March 2020 (what could possibly go wrong?)


#include <avr/eeprom.h>

// Example data for the EEPROM.
uint8_t helloEEPROM[] = "Hello EEPROM World Again!";

// We need the zero terminator to be written too.
uint8_t dataSize = strlen(helloEEPROM) + 1;

void setup() {
    Serial.begin(9600);
    
    // Write the EEPROM data.
    Serial.println("Writing ...");
    eeprom_update_block(helloEEPROM, 0, dataSize);
    Serial.println("Done.\n");

    // Effectively deletes the data.
    helloEEPROM[0]='\0';
}


void loop() {
    // Read the data back.
    eeprom_read_block(helloEEPROM, 0, dataSize);
    Serial.print("Reading...: ");
    Serial.println((char *)helloEEPROM);
    delay(1000);
}
