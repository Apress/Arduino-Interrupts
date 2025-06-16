#include <avr/interrupt.h>
#include "EEPROMinterrupt.h"

volatile EEPROMinfo_t EEPROMinfo;


void EEPROMinit() {
    EEPROMinfo.status = EEPROM_ready;
    EECR = 0;
}


EEPROMerror EEPROMwrite(const uint8_t *buffer, 
                        const uint16_t dataSize, 
                        const uint16_t writeAddress,
                        bool waitComplete) {
                            
    // Is EEPROM busy?
    if (EEPROMinfo.status != EEPROM_ready) {
        return EEPROM_busy;
    }

    // Anything to write?
    if (!dataSize) {
        return EEPROM_dataSize;
    }

    // EEPROM address out of range?
    if ((writeAddress > E2END) || 
        (writeAddress + dataSize > E2END)) {
       return EEPROM_addressError;
    }

    // Set busy status, copy details and start interrupts.
    EEPROMinfo.status = EEPROM_writing;
    EEPROMinfo.bufferAddress = (uint8_t *)buffer;
    EEPROMinfo.dataSize = dataSize;
    EEPROMinfo.errorCode = EEPROM_noError;
    EEPROMinfo.rwAddress = writeAddress;
    EEPROMinfo.currentByte = 0;
    EEPROMinfo.bytesProcessed = 0;
    START_INTERRUPT();

    // Should I wait or return I wonder?
    if (waitComplete) {
        while (EEPROMinfo.status == EEPROM_writing) {
            ; // Do nothing.
        }
    }
    return EEPROM_noError;
}


EEPROMerror EEPROMupdate(const uint8_t *buffer, 
                         const uint16_t dataSize, 
                         const uint16_t writeAddress) {
                            
    // Is EEPROM busy?
    if (EEPROMinfo.status != EEPROM_ready) {
        return EEPROM_busy;
    }

    // Anything to write?
    if (!dataSize) {
        return EEPROM_dataSize;
    }

    // EEPROM address out of range?
    if ((writeAddress > E2END) || 
        (writeAddress + dataSize > E2END)) {
       return EEPROM_addressError;
    }

    // Buffer for each character read from EEPROM.
    uint8_t oneCharacter;

    // Result of the read/write.
    EEPROMerror result;

    // How many bytes did we write?
    uint16_t bytesProcessed = 0;

    // Scan the write buffer and the EEPORM to match
    // characters. Only write those which differ.
    // Always waits for completion - for obvious reasons!
    for (uint16_t x = 0; x < dataSize; x++) {
        result = EEPROMread(&oneCharacter, 
                            1, 
                            (x + writeAddress), 
                            true);
                            
        if (result != EEPROM_noError)
            return result;
         
        if (oneCharacter != buffer[x]) {
            result = EEPROMwrite(&buffer[x], 
                                 1, 
                                 (x + writeAddress), 
                                 true);
                                 
            if (result != EEPROM_noError)
                return result;

            bytesProcessed++;
        }        
    }

    // Fiddle in the EEPROMinfo structure to show the
    // original requested bytes and buffers etc.
    EEPROMinfo.bufferAddress = (uint8_t *)buffer;
    EEPROMinfo.dataSize = dataSize;
    EEPROMinfo.rwAddress = writeAddress;
    EEPROMinfo.currentByte = dataSize + 1;
    EEPROMinfo.bytesProcessed = bytesProcessed;
    return EEPROM_noError;
}


EEPROMerror EEPROMread(const uint8_t *buffer, 
                       const uint16_t dataSize, 
                       const uint16_t readAddress,
                       const bool waitComplete) {
                            
    // Is EEPROM busy? Return if so.
    if (EEPROMinfo.status != EEPROM_ready) {
        return EEPROM_busy;
    }

    // Anything to read?
    if (!dataSize) {
        return EEPROM_dataSize;
    }

    // EEPROM address out of range?
    if ((readAddress > E2END) || 
        (readAddress + dataSize > E2END)) {
       return EEPROM_addressError;
    }

    // Set busy status, copy details and start interrupts.
    EEPROMinfo.status = EEPROM_reading;
    EEPROMinfo.bufferAddress = (uint8_t *)buffer;
    EEPROMinfo.dataSize = dataSize;
    EEPROMinfo.errorCode = EEPROM_noError;
    EEPROMinfo.rwAddress = readAddress;
    EEPROMinfo.currentByte = 0;
    EEPROMinfo.bytesProcessed = 0;
    START_INTERRUPT();

    // Should I wait or return I wonder? Check
    // the status if waiting.
    if (waitComplete) {
        while (EEPROMinfo.status == EEPROM_reading) {
            ; // Do nothing.
        }
    }
    return EEPROM_noError;
}



ISR(EE_READY_vect) {
    // Out of EEPROM memory? Flag error and terminate.
    // Sets the EEPROM_ready status in case further actions 
    // are required.
    if (EEPROMinfo.rwAddress > E2END) {
        STOP_INTERRUPT();
        EEPROMinfo.errorCode = EEPROM_addressError;
        EEPROMinfo.status = EEPROM_ready;
        return;
    }

    // Out of data? We must be finished.
    // Set the ready status and disable the interrupt.
    if (EEPROMinfo.currentByte >= EEPROMinfo.dataSize) {
        STOP_INTERRUPT();
        EEPROMinfo.status = EEPROM_ready;
        return;
    }
    
    // Reads and writes need the next EEPROM address
    // and byte counter.
    EEAR = EEPROMinfo.rwAddress++;
    EEPROMinfo.currentByte++;
    EEPROMinfo.bytesProcessed++;

    // Reading or Writing?
    if (EEPROMinfo.status == EEPROM_reading) {
        // Reading: copy the data from EEPROM to buffer.        
        EECR |= (1 << EERE);
        *EEPROMinfo.bufferAddress++ = EEDR;
    } else {
        // Writing. copy the data to the EEPROM.
        EEDR = *EEPROMinfo.bufferAddress++;
        EECR |= (1 << EEMPE);
        EECR |= (1 << EEPE); 
    }
}

