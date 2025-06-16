//------------------------------------------------------------
// An Arduin sketch demonstrating the use of interrupts to 
// write data to, and read data from, the Arduino EEPROM.
//
// Norman Dunbar
// 14 March 2020
//------------------------------------------------------------

#include "EEPROMinterrupt.h"

// The address in the EEPROM to be  used for the data.
#define EEPROM_ADDRESS 0

// The data itself.
char Message[] = {"Greetings Interrupted EEPROM world!"};

// How much data to read back, plus a buffer for it.
#define READ_THIS_MUCH_DATA 35
char loopBuffer[READ_THIS_MUCH_DATA + 1];


void setup() {
    Serial.begin(9600);  
    EEPROMinit();  

    // Write the data to the EEPROM. Only changed
    // bytes will be written to save lifespan.
    EEPROMerror result = EEPROMupdate(Message, 
                                     strlen(Message), 
                                     EEPROM_ADDRESS);

    if (result != EEPROM_noError) {
        Serial.print("In setup(), EEPROMupdate() error: ");
        Serial.println(result);
        // Will carry on, but results undefined.
    }

/* // Use the following to debug ...
extern EEPROMinfo_t EEPROMinfo;
    Serial.print("CHECKING: Bytes Requested: ");
    Serial.println(EEPROMinfo.dataSize);
    Serial.print(" Bytes Processed: ");
    Serial.println(EEPROMinfo.bytesProcessed);

    
    Serial.println("In the loop....");
*/
}


void loop() {
    loopBuffer[0] = '\0';

    // Read data from EEPROM.
    EEPROMerror result = EEPROMread(loopBuffer, 
                                    READ_THIS_MUCH_DATA, 
                                    EEPROM_ADDRESS, 
                                    false);

    // Strings must be terminated.
    loopBuffer[READ_THIS_MUCH_DATA] = '\0';
    Serial.println(loopBuffer);
    delay(1000);    
}
