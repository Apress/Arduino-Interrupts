/*
 * File: TWIlib.cpp
 *
 * Written 03/13/2015 (mm/dd/yyyy) by Chris Herring,
 * originally.
 *
 * Modified for Arduino, bugfixed and enhanced for C++
 * compilation by Norman Dunbar. April 2022.
 *
 *
 * Licence:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * The Software is provided "as is", without warranty of any kind.
 */

// See: http://www.chrisherring.net/all/tutorial-interrupt-driven-twi-interface-for-avr-part1/
// for more information.



// These two headers must be included before TWIlib.h.
#include <avr/io.h>
#include <avr/interrupt.h>
#include "TWIlib.h"
#include <string.h>

#ifndef NO_ERROR_DATA_REQUIRED
    #include "twiErrors.h"
#endif

//-------------------------------------------------------------
// Title: Global Variables
//-------------------------------------------------------------



//-------------------------------------------------------------
// Variable: TWIInfo
//
// TWIInfo is the <TWIInfoStruct> structure holding status
// information relating to the current TWI conversation.
//-------------------------------------------------------------
TWIInfoStruct TWIInfo;

//-------------------------------------------------------------
// Varible: TWITransmitBuffer
//
// The address from where data are being transmitted
// over TWI.
//-------------------------------------------------------------
uint8_t *TWITransmitBuffer;

//-------------------------------------------------------------
// Varible: TWIReceiveBuffer
//
// The address to which data are being written to
// after being received over TWI.
//-------------------------------------------------------------
volatile uint8_t *TWIReceiveBuffer;

//-------------------------------------------------------------
// Buffer indexes.
//-------------------------------------------------------------

//-------------------------------------------------------------
// Variable: TXBuffIndex
//
// Index into <TWITransmitBuffer>. This is volatile as the ISR 
// changes it as data are transmitted. 
//-------------------------------------------------------------
volatile int TXBuffIndex; 

//-------------------------------------------------------------
// Variable: RXBuffIndex
//
// Index into <TWIReceiveBuffer>. This is volatile as the ISR 
// changes it as data are received and copied into the buffer. 
//-------------------------------------------------------------
volatile int RXBuffIndex;


//-------------------------------------------------------------
// Buffer lengths.
//-------------------------------------------------------------

//-------------------------------------------------------------
// Variable: TXBuffLen
//
// Global. The number of bytes of data to be transmitted from 
// <TWITransmitBuffer>.
//-------------------------------------------------------------
int TXBuffLen;

//-------------------------------------------------------------
// Variable: RXBuffLen
//
// Global. The number of bytes of data to be received into 
// <TWIReceiveBuffer> from TWI.
//-------------------------------------------------------------
int RXBuffLen;

//-------------------------------------------------------------
// Title: Function declarations
//-------------------------------------------------------------


//-------------------------------------------------------------
// Function: TWIInit
//
// Initialize the TWI hardware. This function must be called
// before any other TWI functions can be used however it
// should be called after global interrupts have been enabled.
// 
// Parameter:
//
//   SCLfreq_KHz - Desired frequency of the TWI clock. The 
//                 value is in KHz, and defaults to 100 KHz.
//
// Returns:
//  None.
//-------------------------------------------------------------
void TWIInit(uint32_t SCLfreq_KHz)
{
    // Set pre-scalers (no pre-scaling)
    TWSR = 0;

    // Set bit rate
    TWBR = ((F_CPU / (SCLfreq_KHz*1000)) - 16) / 2;

    // Enable TWI and interrupt
    TWCR = (1 << TWIE) | (1 << TWEN);

    // Update the TWIInfo.
    TWIInfo.mode = Ready;
    TWIInfo.errorCode = TWI_SUCCESS;
    TWIInfo.repStart = 0;
}


//------------------------------------------------------------
// Function: isTWIReady
//
// Checks the ready status of the TWI interface.
//
// Parameter:
//  None.
//
// Returns:
//
//    <TWI_READY> - TWI is ready to be used.
//    <TWI_BUSY> - TWI is not ready.
//-------------------------------------------------------------
uint8_t isTWIReady()
{
  if ((TWIInfo.mode == Ready) ||
      (TWIInfo.mode == RepeatedStartSent)) {
    return TWI_READY;
  } else  {
    return TWI_BUSY;
  }
}


//-------------------------------------------------------------
// Function: SCLfreq
//
// Return the SCL clock frequency in KHz.
//
// Parameter:
//  None.
//
// Returns:
//  The clock speed in KHz.
//-------------------------------------------------------------
uint32_t SCLfreq() {
    return F_CPU / (16 + (2 * TWBR)) / 1000;
}


#ifndef NO_ERROR_DATA_REQUIRED
//-------------------------------------------------------------
// Function: TWIGetLastError
//
// Parameter:
//  errorCode - The errorCode from <TWIInfo> should be 
//              passed in to this function.
//
// Returns:
// The most recent TWI error message. The errorCode from
// <TWIInfo> should be passed in.
//
// Notes:
// 1. This function could read <TWIInfo> to get the error code
// but as interrupts are in progress, that code may have
// changed when it does so, returning an incorrect error 
// message for the actual code.
//
// 2. If your Atmega328P, for example, is short on code and 
// data space, defining NO_ERROR_DATA_REQUIRED will omit this 
// function and all the data defined in <twiErrors.h>. As an 
// example, an Arduino UNO, using PlatformIO as the development
// system, went from 1992/2048 and 2882/32K memory usage to 
// 244/2048 and 1798/32K when this macro was defined. Saving 
// 1048 and 2888 bytes of Static and Flash RAM respectively.
// 
// Using the Arduino Language would have resulted in even 
// larger RAM usage.

//-------------------------------------------------------------
const char *TWIGetLastError(uint8_t errorCode) {
    uint8_t index = 0;

    do {
        if (errorCode == TWIStatusMessages[index].errorCode)
            break;

        index++;
    } while (errorCode != 0xFF);

    return TWIStatusMessages[index].errorMessage;
}
#endif 


//-------------------------------------------------------------
// Function: TWITransmitData
//
// TWITransmitData begins a TWI transmission. 
// 
// This function accepts a void pointer, which will then be 
// cast to a uint8_t*. This allows any datatype to be sent one
// byte at a time.
//
// As with any application, the programmer should ensure that 
// dataLen is not longer than the data, or else this function
// will attempt to index out of bounds.
//
// *WARNING*: The data buffer must not be allowed to go out of
// scope until the data tranmission has completed fully.
//
// When initiating a transmission, the first byte of the data
// to be sent must be the write address of the sensor device
// that will be written to.
//
// Parameters:
//
//    TXdata  - Void pointer to the first byte of data 
//              stream to be sent. Must remain in scope
//              until the data transmission has fully
//              completed.
//
//    dataLen - Length of the data to be transmitted, in bytes.
//
//    repStart- Should the device relinquish control of the
//              bus after the transmission or should it send
//              a repeated start signal? 
//                1 = send the repeated start; 
//                0 = send stop.
//
// Returns:
//
//    <TWI_TX_RX_SUCCESS> Transmission successfully started. 
// 
//    <TWI_TX_RX_NOT_READY>. The TWI bus is not ready, no
//    transmission has been started yet. Try again later.
//
// Usage:
//
// ---C++
// // Wait for the transmit to successfully initiate.
// TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;
// uint8_t TXInitiated = TWI_TX_RX_NOT_READY;
// 
// // This could hang if the TWI bus is up that famous creek!
// while (TXInitiated == TWI_TX_RX_NOT_READY)
//     TXInitiated = TWITransmitData(data, size, true);
// ---
// At this point the code could carry on, do some useful work
// and later on, check if the transmission had completed. If
// it must wait for TWI, then:
//
// ---C++
// while (TWIInfo.errorCode == TWI_NO_RELEVANT_INFO)
//     _delay_ms(1);
// 
// // Grab the error code.
// uint8_t errorCode = TWIInfo.errorCode;
// 
// // Did it work?
// if (errorCode != TWI_SUCCESS) {
//    handleTWIErrorsHere(errorCode);
// }
// ---
//-------------------------------------------------------------
uint8_t TWITransmitData(void *const TXdata, 
                        uint8_t dataLen, 
                        uint8_t repStart)
{
    if (!isTWIReady()) {
        return TWI_TX_RX_NOT_READY;
    }
    
    //---------------------------------------------------------
    // Set repeated start mode.
    //---------------------------------------------------------
    TWIInfo.repStart = repStart;

    //---------------------------------------------------------
    // Copy data  uffer address into the transmit buffer
    // pointer.
    //---------------------------------------------------------
    TWITransmitBuffer = (uint8_t *)TXdata;

    //---------------------------------------------------------
    // Copy transmit info to global variables.
    //---------------------------------------------------------
    TXBuffLen = dataLen;
    TXBuffIndex = 0;
    
    //---------------------------------------------------------
    // If a repeated start has been sent, then devices are
    // already listening for an address and another start
    // does not need to be sent. 
    //---------------------------------------------------------
    if (TWIInfo.mode == RepeatedStartSent) {
        TWIInfo.mode = Initializing;

        //-----------------------------------------------------
        // Load data to transmit buffer.
        //-----------------------------------------------------
        TWDR = TWITransmitBuffer[TXBuffIndex++];

        //-----------------------------------------------------
        // Send the data
        //-----------------------------------------------------
        TWISendTransmit();
    } else {
        //-----------------------------------------------------
        // Otherwise, just send the normal start signal
        // to begin transmission.
        //-----------------------------------------------------
        TWIInfo.mode = Initializing;
        TWISendStart();
    }
    
  //-----------------------------------------------------------
  // All's well.
  //-----------------------------------------------------------
  return TWI_TX_RX_SUCCESS;
}


//-------------------------------------------------------------
// Function: TWIReadData
//
// TWIReadData reads begins a master read operation. A write
// operation is usually performed before the read in order to 
// set the slave up to transmit the desired data. 
//
// This function takes a slave address, utilizes the transmit
// function to address the slave with the read bit set, then 
// lets the interrupts do the rest of the work. 
// 
// The receive buffer length global variable is set to the 
// number of bytes to read.
// 
// The logic in the interrupt vector code will reply with ACK 
// until there is only one more byte to read then reply with 
// NACK. So it should be ensured that the number of bytes to
// read is correctly specified. 
// 
// If it is too long then the slave might stop sending and we 
// enter the realm of undefined behavior. 
//
// *WARNING*: The data buffer must not be allowed to go out of
// scope until the data reception has completed fully.
// 
// If a transmit failed due to the TWI module not being ready 
// then this function will return a <TWI_TX_RX_NOT_READY> and
// it should be handled in the same manner as the transmit 
// function. 
// 
// Parameters:
//
//    TWIaddr   - The 7 bit address of the slave to read
//                    from. the 8th bit should be 0, but if it
//                    is not then it will be shifted out
//                    anyway. So it does not really matter.
//
//    bytesToRead - The number of bytes to read. Be careful
//                    this is not more than the slave knows
//                    how to send. Otherwise you may encounter
//                    some undefined behavior.
//
//    RXBuffer - The address of the buffer where "bytesToRead"
//               bytes of data will be written by the interrupt
//               handler. Must remain in scope until the read
//               has fully completed.
//
//    repStart  - 1 to send a repeated start signal after
//                    the transmission and maintain control of
//                    the bus.
//            0 to send a stop signal after the
//                    transmission and relinquish control of
//                    the bus.
//
//
// Returns:
//
//    <TWI_TX_RX_SUCCESS> Receipt successfully started.
// 
//    <TWI_TX_RX_NOT_READY> The TWI bus is not ready yet. No
//    receipt has been started yet. Try again later.
//
//
// Usage:
//
// ---C++
// // Wait for the reception to successfully initiate.
// TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;
// uint8_t RXInitiated = TWI_TX_RX_NOT_READY;
// 
// // This could hang if the TWI bus is up that famous creek!
// while (RXInitiated == TWI_TX_RX_NOT_READY)
//     RXInitiated = TWIReadData(address, data, size, false);
// ---
// At this point the code could carry on, do some useful work
// and later on, check if the reception had completed. If
// it really must wait for TWI, then:
//
// ---C++
// while (TWIInfo.errorCode == TWI_NO_RELEVANT_INFO)
//     _delay_ms(1);
// 
// // Grab the error code.
// uint8_t errorCode = TWIInfo.errorCode;
// 
// // Did it work?
// if (errorCode != TWI_SUCCESS) {
//    handleTWIErrorsHere(errorCode);
// }
// ---
//-------------------------------------------------------------
uint8_t TWIReadData(uint8_t TWIaddr, 
                    uint8_t bytesToRead,
                    void *RXBuffer, 
                    uint8_t repStart) {

    //---------------------------------------------------------
    // Reset buffer index and set RXBuffLen to the number
    // of bytes to read. Any read operation will start from
    // the beginning of the Receive buffer.
    //---------------------------------------------------------
    RXBuffIndex = 0;
    RXBuffLen = bytesToRead;
    TWIReceiveBuffer = (uint8_t *)RXBuffer;

    //---------------------------------------------------------
    // Create the one value array for the address to be
    // transmitted.
    //---------------------------------------------------------
    uint8_t TXdata[1];

    //---------------------------------------------------------
    // Shift the address left and AND a 1 into the 
    // read or write bit (set to write mode).
    //---------------------------------------------------------
    TXdata[0] = (TWIaddr << 1) | 0x01;

    //---------------------------------------------------------
    // Use the TWITransmitData function to initialize the
    // transfer and address the slave.
    //---------------------------------------------------------
    uint8_t txStat = TWITransmitData(TXdata, 1, repStart);
    if (txStat == TWI_TX_RX_NOT_READY) {
        return TWI_TX_RX_NOT_READY;
    }

    //---------------------------------------------------------
    // All's well.
    //---------------------------------------------------------
    return TWI_TX_RX_SUCCESS;
}



//-------------------------------------------------------------
// Here be interrupts!
//-------------------------------------------------------------


//-------------------------------------------------------------
// Function: ISR(TWI_vect)
//
// This is the interrupt handler for the TWI hardware. It is
// entered each and every time that any action has been 
// completed by the hardware. The status code is extracted and
// depending on its value, the next action is initiated by 
// optionally setting TWDR and then setting TWCR to continue
// using the TWI hardware.
//
// The interrupt handler will set the errorCode in <TWIInfo> to 
// <TWI_SUCCESS> when the handler has completed processing for
// <TWITransmitData> or <TWIReadData>. 
//
// Until such time as this code is set, the communication is
// still in progress.
//
// *NOTE:* The ISR currently only handles master transmitter 
// and master receiver modes. The slave transmitter and slave
// receiver modes have yet to be implemented.
//-------------------------------------------------------------
ISR (TWI_vect)
{
    switch (TWI_STATUS) {

    //=========================================================
    //                  MASTER TRANSMITTER
    //=========================================================

    //---------------------------------------------------------
    // SLA+W transmitted. ACK received from the addressed
    // sensor.
    //---------------------------------------------------------
    case TWI_MT_SLAW_ACK:
        // Set mode to Master Transmitter.
        // Drops in below.
        TWIInfo.mode = MasterTransmitter;

    //---------------------------------------------------------
    // Start condition has been transmitted. The next data
    // byte sent will be the SLA+W or SLA+R address byte.
    //---------------------------------------------------------
    case TWI_START_SENT:
        // Drops in below.

    //---------------------------------------------------------
    // A data byte has been transmitted, ACK has been
    // received from the sensor. The code in this library
    // sends ALL bytes from the MT to the sensor and asks
    // for an ACK for all of them. TWI_MT_DATA_NACK is not
    // really possible as a status, unless some error 
    // occurrred in the hardware or or the bus.
    //---------------------------------------------------------
    case TWI_MT_DATA_ACK: 
        if (TXBuffIndex < TXBuffLen) {
        // There is more data to be sent, so, load the
        // next data byte to transmit register.
        TWDR = TWITransmitBuffer[TXBuffIndex++]; 
        TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;
        TWISendTransmit(); // Send the data
        } else
            if (TWIInfo.repStart) {
                // This transmission is complete however 
                // do not release bus yet.
                TWIInfo.errorCode = TWI_SUCCESS;
                TWISendStart();
          } else {
                // All transmissions are complete, exit.
                TWIInfo.mode = Ready;
                TWIInfo.errorCode = TWI_SUCCESS;
                TWISendStop();
          }
        break;
    
    //=========================================================
    //                  MASTER RECEIVER
    //=========================================================

    //---------------------------------------------------------
    // SLA+R has been transmitted, ACK has been received
    // from the adresses sensor. Request some data.
    //---------------------------------------------------------
    case TWI_MR_SLAR_ACK:
        // Switch to Master Receiver mode
        TWIInfo.mode = MasterReceiver;

        // If there is more than one byte to be read,
        // request the next data byte and send an ACK
        // when it is received.
        if (RXBuffIndex < RXBuffLen-1) {
            TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;
            TWISendACK();
        } else {
            // Otherwise request the final data byte and 
            // send a NACK when it is received.
            TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;
            TWISendNACK();
        }
      break;
    
    //---------------------------------------------------------
    // Data has been received from sensor, ACK has been 
    // transmitted back to say we got it. Request more data
    // if any outstanding.
    //---------------------------------------------------------
    case TWI_MR_DATA_ACK: 
    
        // Copy data byte to receive buffer.
        TWIReceiveBuffer[RXBuffIndex++] = TWDR;

        // If there is more than one byte still to be read,
        // request another data byte and return an ACK.
        if (RXBuffIndex < RXBuffLen-1) {
            TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;
            TWISendACK();
        } else {
            // Otherwise request the final data byte and 
            // when it is received, return NACK.
            TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;
            TWISendNACK();
        }
      break;
    
    //---------------------------------------------------------
    // Data byte has been received from sensor, NACK has  
    // been transmitted. End of conversation.
    //---------------------------------------------------------
    case TWI_MR_DATA_NACK: 
    
        // Copy data byte to receive buffer.
        TWIReceiveBuffer[RXBuffIndex++] = TWDR; 

        // This transmission is complete however do not
        // release bus yet if a repeated start was 
        // requested.
        if (TWIInfo.repStart) {
            TWIInfo.errorCode = TWI_SUCCESS;
            TWISendStart();
        } else {
            // All transmissions are complete, exit.
            TWIInfo.mode = Ready;
            TWIInfo.errorCode = TWI_SUCCESS;
            TWISendStop();
        }
      break;
    
    //=========================================================
    //               COMMON CODE - MT and MR
    //=========================================================

    //---------------------------------------------------------
    // SLA+R transmitted, NACK received. We are done here.
    //---------------------------------------------------------
    case TWI_MR_SLAR_NACK:
        // Drops in below.

    //---------------------------------------------------------
    // SLA+W transmitted, NACK received. We are done here.
    //---------------------------------------------------------
    case TWI_MT_SLAW_NACK:
        // Drops in below.

    //---------------------------------------------------------
    // Data byte has been transmitted, NACK has been 
    // received. We are done here.
    //---------------------------------------------------------
    case TWI_MT_DATA_NACK:
        // Drops in below.

    //---------------------------------------------------------
    // Arbitration has been lost. Return error and send 
    // stop and set mode to ready. If a repeated start was
    // previously requested, send a start - this will 
    // succeed when the bus becomes free.
    //---------------------------------------------------------
    case TWI_LOST_ARBIT: 
        if (TWIInfo.repStart) { 
            // Repeated start wanted. Try to get one when
            // the bus is next free.      
            TWIInfo.errorCode = TWI_STATUS;
            TWISendStart();
        } else {
            // All transmissions are complete, exit.
            TWIInfo.mode = Ready;
            TWIInfo.errorCode = TWI_STATUS;
            TWISendStop();
        }
      break;

    //---------------------------------------------------------
    // Repeated start has been transmitted. Set the mode 
    // but DO NOT clear TWINT as the next data is not yet
    // ready.
    //---------------------------------------------------------
    case TWI_REP_START_SENT:
        TWIInfo.mode = RepeatedStartSent;
        break;
    
    //=========================================================
    //                  SLAVE RECEIVER
    //=========================================================
    // TODO  IMPLEMENT SLAVE RECEIVER FUNCTIONALITY

    //=========================================================
    //                SLAVE TRANSMITTER
    //=========================================================
    // TODO  IMPLEMENT SLAVE TRANSMITTER FUNCTIONALITY

    //=========================================================
    //              MISCELLANEOUS STATES
    //=========================================================

    //---------------------------------------------------------
    // It is not really possible to get into this ISR on
    // this condition Rather, it is there to be manually
    // set between operations
    //---------------------------------------------------------
    case TWI_NO_RELEVANT_INFO: 
        break;
            
    //---------------------------------------------------------
    // Illegal Start+Stop transmitted. Abort and retun an
    // error code.
    //---------------------------------------------------------
    case TWI_ILLEGAL_START_STOP:
        TWIInfo.errorCode = TWI_ILLEGAL_START_STOP;
        TWIInfo.mode = Ready;
        TWISendStop();
        break;
    } // end switch.
}
