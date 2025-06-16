#ifndef TWI_ERRORS
#define TWI_ERRORS

//-------------------------------------------------------------
// File: twiErrors.h
//
// Note: This file should not be included if 
// NO_ERROR_DATA_REQUIRED is defined. This will save Static 
// and Flash RAM space, if it is tight.
//
// ---C++
// #ifndef NO_ERROR_DATA_REQUIRED
//    #include "twiErrors.h"
// #endif
//
// ...
//
// if (errorCode != TWI_SUCCESS) {
//        printf("Error: %x", errorCode);
//
//#ifndef NO_ERROR_DATA_REQUIRED
//        printf(" %s", TWIGetLastError(errorCode));
//#endif
//
//        printf("\n");
// }  
// ---
//-------------------------------------------------------------



//------------------------------------------------------------
// Struct: TWIStatusMessage
//
// The structure holds a status code from TWI and a messages
// attempting to translate the code into meaningful text.
//
// Members:
//
//  errorCode - Any TWI status codes set by the interrupt 
//              handler <ISR{TWI_vect)>.
//
//  errorMessage - A reasonably explanatory message for the
//                 error code.
//------------------------------------------------------------
typedef struct TWIStatusMessage {
    uint8_t errorCode;
    const char * errorMessage;
} TWIStatusMessage;

//------------------------------------------------------------
//
// Array: TWIStatusMessages[]
//
// TWIStatusMessages is a static array of <TWIStatusMessage>.
// There is one entry in the array for each possible status 
// code that can be returned, at any point in time, from the 
// TWI interrupt handler into <TWIInfo>.
//
// It could be used as follows where we wait in a loop for 
// a success from TWI:
//
// ---C++
//    TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;
//    TWITransmitData(data, size, true);
//    ...
//    uint8_t errorCode = TWIInfo.errorCode;
//    while (errorCode != TWI_SUCCESS) {
//        // Ignore TWI_NO_RELEVANT_INFO.
//        if (errorCode != TWI_NO_RELEVANT_INFO) {
//            printf("TWI Transmit error: %d\n", errorCode);
//            printf("%s\n", TWIGetLastError(errorCode));
//            ...
//        }
//        errorCode = TWIInfo.errorCode;           
//    }
// 
// ---
//
//------------------------------------------------------------
TWIStatusMessage TWIStatusMessages[] = {
    {0x00, "Bus error due to illegal start or stop condition"},
    {0x08, "Start sent"},
    {0x10, "Repeated start sent"},
    {0x18, "SLA+W transmitted, ACK received"},
    {0x20, "SLA+W transmitted, NACK received"},
    {0x28, "Data byte transmitted, ACK received"},
    {0x30, "Data byte transmitted, NACK received"},
    {0x38, "Arbitrations lost"},
    {0x40, "SLA+R transmitted, ACK received"},
    {0x48, "SLA+R transmitted, NACK received"},
    {0x50, "Data byte received, ACK transmitted"},
    {0x58, "Data byte received, NACK transmitted"},
    {0x60, "Own SLA+W receved, ACK transmitted"},
    {0x68, "Arbitration lost in SLA+R/W. Own SLA+W received, ACK transmitted"},
    {0x70, "General call address received, ACK transmitted"},
    {0x80, "In SLA+W mode, data byte has been received, ACK transmitted"},
    {0x88, "In SLA+W mode, data byte has been received, NACK transmitted"},
    {0x90, "In General mode, data byte has been received, ACK transmitted"},
    {0x98, "In General mode, data byte has been received, NACK transmitted"},
    {0xA0, "In SLA+R/W mode, stop or Repeated start received"},
    {0xA8, "Own SLA+R received, ACK transmitted"},
    {0xB0, "Arbitration lost in SLA+R/W. Own SLA+R received, ACK transmitted"},
    {0xB8, "Data byte transmitted, ACK received"},
    {0xC0, "Data byte transmitted, NACK received"},
    {0xC8, "Final data byte transmitted, ACK received"},
    {0xF8, "No relevant state information"},
    {0xFF, "Unknown Error"},
};


#endif
