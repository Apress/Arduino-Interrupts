// A small sketch to Write some data to the EEPROM and then to
// read it back. This sketch does not use interrupts but simply
// demonstrates the code supplied by Atmel in the ATmega328P 
// data sheet.
//
// The code has been very slightly modified by me for the
// purposes of the Arduino Interrupts book example sketches
// and to asvoid writes whenever possible. When writing an
// erase/write cycle is used as opposed to an erase then a 
// write.
//
// Norman Dunbar
// 11 March 2020

// Required for cli(), sei() etc.
#include <avr/interrupt.h>


// A function to read one byte of data from the EEPROM.
uint8_t EEPROMread(uint16_t EEaddress) {

    // Wait for existing write to complete. 
    while(EECR & (1<<EEPE)) 
        ; // Wait here ...

    // EEPROM address.
    EEAR = EEaddress; 

    // Trigger the read.
    EECR |= (1<<EERE); 

    // Read the EEPROM byte.
    return EEDR;
}


// A function to write one byte of data to the EEPROM. The
// data will be erased and written only if it is different
// to the existing data byte to save too many writes to
// the EEPROM - which has a limited number of write cycles.
void EEPROMwrite(uint16_t EEaddress, uint8_t EEdata) {
    
    // Check current data and return if the same
    // as we intend to write.
    uint8_t currentByte = EEPROMread(EEaddress);
    if (currentByte == EEdata) 
        return;

    // Wait for existing write to complete. This should
    // be fine as we had to do it in EEPROMread().
    while(EECR & (1<<EEPE)) 
        ; // Wait here ...

    // EEPROM address.
    EEAR = EEaddress; 

    // EEPROM Data byte.
    EEDR = EEdata; 

    // Disable interrupts and set master write enable. 
    // BUT: What about 0 in EEPE? It's already 0!
    uint8_t oldSREG = SREG;
    cli();
    EECR |= (1<<EEMPE); 

    // You have 4 clocks to do this...
    // Will cause EEPROM to be written.
    EECR |= (1<<EEPE); 

    // Restore interrupts - if they were on.
    SREG = oldSREG;
}


void setup() {
    // Initialise the EEPROM control register
    // this should be already set, but better
    // to be explicit.
    EECR = 0;
    
    Serial.begin(9600);
    Serial.println("Writing data to the EEPROM");
    Serial.println("--------------------------");
    Serial.println();

    
    // Write some data to the EEPROM at address 0.
    uint8_t dataBuffer[] = "Hello EEPROM World!";
    uint8_t *p = dataBuffer;
    uint16_t EEAddress = 0;

    // Write the data count.
    EEPROMwrite(EEAddress++, (uint8_t)strlen(dataBuffer));

    while (*p) {
        Serial.print("Address ");
        Serial.print(EEAddress);
        Serial.print(" = '");
        Serial.print((char)*p);
        Serial.println("'");
        EEPROMwrite(EEAddress++, *p++);
    }

    Serial.println();
    Serial.println("Reading data from the EEPROM");
    Serial.println("----------------------------");
    Serial.println();
    delay(5000);
}

void loop() {
    // Read the EEPROM data back in the loop.
    uint16_t EEAddress = 0;

    // How many characters?
    uint8_t dataBytes = EEPROMread(EEAddress++);

    // Read back the characters.
    for (uint8_t x = 0; x < dataBytes; x++) {
        Serial.print("Address: ");
        Serial.print(EEAddress);
        Serial.print(" = '");
        Serial.print((char)EEPROMread(EEAddress++));
        Serial.println("'");
    }

    Serial.println("----------------------------");
    Serial.println();

    // And breathe!
    delay(5000);    
}
