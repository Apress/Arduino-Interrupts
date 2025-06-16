/*
 * File: TWIlib.h
 *
 * Written 03/13/2015 (mm/dd/yyyy) by Chris Herring,
 * originally.
 *
 * Modified for Arduino, bugfixed and enhanced for C++
 * compilation by Norman Dunbar. April 2022.
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


#ifndef TWILIB_H_
#define TWILIB_H_

//-------------------------------------------------------------
// Title: Constants
//-------------------------------------------------------------



//-------------------------------------------------------------
// TWITransmitData and TWIReceiveData error codes.
//-------------------------------------------------------------


//-------------------------------------------------------------
// Constant: TWI_TX_RX_SUCCESS
// 
// <TWITransmitData> or <TWIReadData> will return this when a
// successful initiation of the TWI hardware has occurred. The
// interrupt handler will continue to process all the data
// after the call returns.
//-------------------------------------------------------------
#define TWI_TX_RX_SUCCESS 0

//-------------------------------------------------------------
// Constant: TWI_TX_RX_NOT_READY
// 
// <TWITransmitData> or <TWIReadData> will return this when an
// initiation of the TWI hardware has failed. No transmission
// or receipt of data will have been started in this case.
//-------------------------------------------------------------
#define TWI_TX_RX_NOT_READY 1

//-------------------------------------------------------------
// Return codes from isTWIReady().
//-------------------------------------------------------------

//-------------------------------------------------------------
// Constant: TWI_READY
// 
// The TWI interface is ready to be used.
//-------------------------------------------------------------
#define TWI_READY 1

//-------------------------------------------------------------
// Constant: TWI_BUSY
// 
// The TWI interface is not ready to be used.
//-------------------------------------------------------------
#define TWI_BUSY  0



//-------------------------------------------------------------
// Macro: TWI_STATUS
//
// A macro to extract the status of the most recent TWI action,
// from the TWSR register, mask out the unwanted bits (bits
// 0-2), and return the actual status code listed in the data
// sheets.
//-------------------------------------------------------------
#define TWI_STATUS  (TWSR & 0xF8) 


//-------------------------------------------------------------
// Enum: TWIMode
//
// An enum defining various modes that the TWI interface can
// be in at any point in time.
//
//   Ready - The TWI interface is ready for use. Either It has 
//           been initialised; All data has been sent; All data  
//           has been received; or, After errors on the bus.
//
//   Initializing - The TWI interface is currently being
//                  initialized. For USA developers!
//
//   Initialising - The TWI interface is currently being
//                  initialised. For UK developers!
//
//   RepeatedStartSent - A repeated start has been successfully
//                       sent on the bus.
//
//   MasterTransmitter - SLA+W has been sent, and ACK received.
//                       We are currently processing as a
//                       master transmitter. We have the bus!
//
//   MasterReceiver - SLA+R has been sent and ACK received. We
//                    still control the bus, but we are
//                    receiving data.
//
//   SlaveTransmitter - Not currently used by the library.
//
//   SlaveReciever - Not currently used by the library.
//-------------------------------------------------------------
typedef enum {
    Ready,
    Initializing,
    RepeatedStartSent,
    MasterTransmitter,
    MasterReceiver,
    SlaveTransmitter,
    SlaveReciever,
    Initialising = Initializing
} TWIMode;


//-------------------------------------------------------------
// Struct: TWIInfoStruct
//
// TWI interface information. Holds the current state of the
// TWI interface.
//
// Members:
//
//  mode - the current mode of the TWI library. See <TWIMode>.
//  errorCode - Any status codes causing TWI problems, or,
//              the value <TWI_NO_RELEVANT_INFO> when the ISR
//              is working, or <TWI_SUCCESS> when the current
//              transmit or receipt of data has completed.
//  repStrt - False if a repeated start is not required, True
//            if one is to be requested.
//-------------------------------------------------------------
typedef struct TWIInfoStruct{
    TWIMode mode;
    uint8_t errorCode;
    uint8_t repStart; 
} TWIInfoStruct;

//-------------------------------------------------------------
// Title: External Variables
//-------------------------------------------------------------


//-------------------------------------------------------------
// Mark this lot as extern to avoid duplicate definitions.
// They will be declared properly in TWIlib.cpp.
//-------------------------------------------------------------

//-------------------------------------------------------------
// Variable: TWIInfo
//
// The TWIInfo structure. This holds status etc information 
// regarding the current conversation. See <TWIInfoStruct>.
//-------------------------------------------------------------
extern TWIInfoStruct TWIInfo;


//-------------------------------------------------------------
// Variable: TWITransmitBuffer
//
// Address where the data to be transmitted over TWI is held.
// This is a copy of the address of the buffer passed in by
// the caller. It is defined external in this header, but is
// fully declared in <TWIlib.cpp>.
//
// *WARNING*: The data buffer *must not* be allowed to go
// out of scope until after the data transmission has finished.
//-------------------------------------------------------------
extern uint8_t *TWITransmitBuffer;

//-------------------------------------------------------------
// Variable: TWIReceiveBuffer
//
// Address where the data to be received from TWI is held.
// This is a copy of the address of the buffer passed in by
// the caller. It is defined external in this header, but is
// fully declared in <TWIlib.cpp>.
//
// The buffer is declared volatile as it will be modified by
// the interrupt handler.
//
// *WARNING*: The data buffer *must not* be allowed to go
// out of scope until after the data receipt has finished.
//-------------------------------------------------------------
extern volatile uint8_t *TWIReceiveBuffer;


//-------------------------------------------------------------
// Buffer indexes.
//-------------------------------------------------------------

//-------------------------------------------------------------
// Variable: TXBuffIndex
//
// Index into the transmit buffer. It's volatile, and may
// change at any time -- usually by the interrupt handler.
// This is where data to be transmitted is copied from, into
// TWDR in order to be transmitted over TWI.
//-------------------------------------------------------------
extern volatile int TXBuffIndex; 

//-------------------------------------------------------------
// Variable: RXBuffIndex
//
// Index into the receive buffer. It's volatile, and may
// change at any time -- usually by the interrupt handler.
// This is where data received over TWI is copied into from
// TWDR in order to be accessed by the code.
//-------------------------------------------------------------
extern volatile int RXBuffIndex;


//-------------------------------------------------------------
// Buffer lengths.
//-------------------------------------------------------------

//-------------------------------------------------------------
// Variable: TXBuffLen
//
// The size of the data to be transmitted from the TX buffer.
//-------------------------------------------------------------
extern int TXBuffLen;

//-------------------------------------------------------------
// Variable: RXBuffLen
//
// The size of the data to be read into the RX buffer.
//-------------------------------------------------------------
extern int RXBuffLen;


//-------------------------------------------------------------
// Title: TWI Status Codes.
//-------------------------------------------------------------

//-------------------------------------------------------------
// Constant: TWI_START_SENT
//
// Status code returned whan a START has been transmitted to
// the bus and control has been gained.
//-------------------------------------------------------------
#define TWI_START_SENT 0x08

//-------------------------------------------------------------
// Constant: TWI_REP_START_SENT
//
// Status code returned when a REPEATED START has been sent.
//-------------------------------------------------------------
#define TWI_REP_START_SENT 0x10


//-------------------------------------------------------------
// Master Transmitter Mode
//-------------------------------------------------------------

//-------------------------------------------------------------
// Constant: TWI_MT_SLAW_ACK
//
// Status code returned when SLA+W sent and ACK received. This
// puts the TWI into master transmitter mode.
//-------------------------------------------------------------
#define TWI_MT_SLAW_ACK 0x18 

//-------------------------------------------------------------
// Constant: TWI_MT_SLAW_NACK
//
// Status code returned when SLA+W sent and NACK received. The
// sensor we are attempting to communicate with is missing or
// is too busy to respond. 
//-------------------------------------------------------------
#define TWI_MT_SLAW_NACK 0x20

//-------------------------------------------------------------
// Constant: TWI_MT_DATA_ACK
//
// Status code returned when DATA has been sent and and ACK
// has been received.
//-------------------------------------------------------------
#define TWI_MT_DATA_ACK 0x28 

//-------------------------------------------------------------
// Constant: TWI_MT_DATA_NACK
//
// Status code returned when DATA has been sent and and NACK
// has been received.
//-------------------------------------------------------------
#define TWI_MT_DATA_NACK 0x30 


//-------------------------------------------------------------
// Master Receiver Mode.
//-------------------------------------------------------------

//-------------------------------------------------------------
// Constant: TWI_MR_SLAR_ACK
//
// Status code returned when SLA+R sent and ACK received. This
// puts the TWI into master receiver mode.
//-------------------------------------------------------------
#define TWI_MR_SLAR_ACK 0x40 

//-------------------------------------------------------------
// Constant: TWI_MR_SLAR_NACK
//
// Status code returned when SLA+R sent and NACK received. The
// sensor we are attempting to communicate with is missing or
// is too busy to respond. 
//-------------------------------------------------------------
#define TWI_MR_SLAR_NACK 0x48 

//-------------------------------------------------------------
// Constant: TWI_MR_DATA_ACK
//
// Status code returned when the sensor sent us some data, now
// in register TWDR, and we sent back an ACK.
//-------------------------------------------------------------
#define TWI_MR_DATA_ACK 0x50 

//-------------------------------------------------------------
// Constant: TWI_MR_DATA_NACK
//
// Status code returned when the sensor sent us some data, now
// in register TWDR, and we sent back a NACK.
//-------------------------------------------------------------
#define TWI_MR_DATA_NACK 0x58 


//-------------------------------------------------------------
// Miscellaneous States
//
// Constant: TWI_LOST_ARBIT
//
// Status code returned when we lost out in an arbitration to
// gain control of the bus. 
//-------------------------------------------------------------
#define TWI_LOST_ARBIT 0x38 

//-------------------------------------------------------------
// Constant: TWI_NO_RELEVANT_INFO
//
// No relevant information available about the current ISR
// action as it is still in progress.
//-------------------------------------------------------------
#define TWI_NO_RELEVANT_INFO 0xF8 

//-------------------------------------------------------------
// Constant: TWI_ILLEGAL_START_STOP
//
// Illegal START or STOP condition has been detected. It is 
// not permitted to send a START immediately followed by a 
// STOP.
//-------------------------------------------------------------
#define TWI_ILLEGAL_START_STOP 0x00 

//-------------------------------------------------------------
// Constant: TWI_SUCCESS
//
// When the <TWIInfoStruct> errorCode has this value, it
// indicates a successful data transfer. This state is
// impossible from the TWI status register, TWSR, as bit2 is
// always 0 and is read only. Bits 1 and 0 are the prescaler
// bits and these three bits are masked out when obtaining the
// TWI status value. See <TWI_STATUS>.
// TWI_SUCCESS is therefore a value impossible to be returned
// from TWI.
//-------------------------------------------------------------
#define TWI_SUCCESS 0xFF 


//-------------------------------------------------------------
// Constant: TWI_COMMON
//
// These three bits are common to all TWI macros which write a
// value to TWCR to initiate the next action.
//-------------------------------------------------------------
#define TWI_COMMON (1<<TWINT)|(1<<TWEN)|(1<<TWIE) 


//-------------------------------------------------------------
// Title: TWI Helper Macros
//-------------------------------------------------------------

//-------------------------------------------------------------
// Macro: TWISendStart
//
// Sends the START signal to TWI and blocks until the START is
// sent and we gain control of the TWI bus.
//-------------------------------------------------------------
#define TWISendStart()  (TWCR = (TWI_COMMON)|(1<<TWSTA)) 

//-------------------------------------------------------------
// Macro: TWISendStop
//
// Send the STOP signal and releases control of the bus.
//-------------------------------------------------------------
#define TWISendStop()   (TWCR = (TWI_COMMON)|(1<<TWSTO)) 

//-------------------------------------------------------------
// Macro: TWISendTransmit
//
// Used to request a further byte of data to be transmitted or
// received over the bus.
//-------------------------------------------------------------
#define TWISendTransmit()   (TWCR = (TWI_COMMON)) 

//-------------------------------------------------------------
// Macro: TWISendACK
//
// FOR MR mode. Resume a transfer, ensure that TWI and
// interrupts are enabled and respond with an ACK if the 
// device is addressed as a slave or after it receives a byte.
//-------------------------------------------------------------
#define TWISendACK()  (TWCR = (TWI_COMMON)|(1<<TWEA)) 

//-------------------------------------------------------------
// Macro: TWISendNACK
//
// FOR MR mode. Resume a transfer, ensure that TWI and 
// interrupts are enabled but DO NOT respond with an ACK if 
// the device is addressed as a slave or after it receives a 
// byte.
//-------------------------------------------------------------
#define TWISendNACK() (TWCR = (TWI_COMMON))


//-------------------------------------------------------------
// Initiate a write of data to the TWI interface.
//-------------------------------------------------------------
uint8_t TWITransmitData(void *const TXdata, 
                        uint8_t dataLen, 
                        uint8_t repStart);
                        
//-------------------------------------------------------------
// Initialise the TWI interface with a default clock frequency
// of 100 KHz. 
//-------------------------------------------------------------
void TWIInit(uint32_t freqSCLKHz = 100);

//-------------------------------------------------------------
// Initiate a read of data from the TWI interface.
//-------------------------------------------------------------
uint8_t TWIReadData(uint8_t TWIaddr, 
                    uint8_t bytesToRead,
                    void *RXBuffer, 
                    uint8_t repStart);
                    
//-------------------------------------------------------------
// Is the TWI interface ready for use, or is it busy?
//-------------------------------------------------------------
uint8_t isTWIReady();

//-------------------------------------------------------------
// Return the current TWI clock frequency in KHz.
//-------------------------------------------------------------
uint32_t SCLfreq();

#ifndef NO_ERROR_DATA_REQUIRED
//-------------------------------------------------------------
// Return the most recent TWI error message. Uses the error
// code from <TWIInfo> and returns a message explaining what
// might have happened.
//-------------------------------------------------------------
const char *TWIGetLastError(uint8_t errorCode);
#endif

#endif // TWICOMMS_H_ 
