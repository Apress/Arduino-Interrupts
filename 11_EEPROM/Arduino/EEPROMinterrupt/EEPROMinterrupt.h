#ifndef EEPROMINTERRUPT_H
#define EEPROMINTERRUPT_H

// Arduino doesn't need this line.
#include <stdint.h>


// Various status codes that apply to EEPROM actions.
enum EEPROMstatus : uint8_t {
    EEPROM_ready = 1,           // Read for action. Not busy.
    EEPROM_writing,             // Busy - writing data.
    EEPROM_reading,             // Busy - reading data.
    EEPROM_error                // Oh dear, it went belly up.
} ;


// Various error codes that apply to EEPROM actions.
enum EEPROMerror : uint8_t {
    EEPROM_noError = 1,         // No errors detected.
    EEPROM_busy,                // EEPROM in use, please wait.
    EEPROM_dataSize,            // Datasize is not >= 0.
    EEPROM_addressError         // EEPROM address > E2END.
} ;


// This structure holds information about current
// EEPROM operations. It is volatile as it will be
// updated in the ISR.
typedef struct EEPROMinfo_t {
    uint8_t *bufferAddress;     // Read/write data here.
    uint16_t dataSize;          // Bytes to read/write.
    uint16_t currentByte;       // Bytes done so far.
    uint16_t rwAddress;         // Current EEPROM address.
    EEPROMstatus status;        // Current EEPROM status.
    EEPROMerror errorCode;      // Current EEPROM error code.
    uint16_t bytesProcessed;    // Bytes written or read.
} EEPROMinfo_t;


// This make things look like function calls.
#define STOP_INTERRUPT() EECR &= ~(1 << EERIE)
#define START_INTERRUPT() EECR |= (1 << EERIE)


// Initialise the EEPROM.
void EEPROMinit();

// Read data from EEPROM.
EEPROMerror EEPROMread(const uint8_t *buffer, 
                       const uint16_t dataSize, 
                       const uint16_t readAddress,
                       const bool waitComplete);

// Write data to EEPROM.
EEPROMerror EEPROMwrite(const uint8_t *buffer, 
                        const uint16_t dataSize, 
                        const uint16_t writeAddress,
                        bool waitComplete);                        

// Write data to EEPROM if changed only.
EEPROMerror EEPROMupdate(const uint8_t *buffer, 
                         const uint16_t dataSize, 
                         const uint16_t writeAddress);                        

#endif // EEPROMINTERRUPT_H

