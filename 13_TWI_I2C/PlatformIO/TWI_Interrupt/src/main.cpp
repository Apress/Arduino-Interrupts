//------------------------------------------------------------
// A sketch to use direct register access to read data from 
// an LM75A temperature sensor.  Written for PlatformIO.
//
// This version uses Norman Dunbar's slightly amended version
// of Chris Herring's TWIlib -- an interrupt driven TWI library
// for the ATmega328P et al microcontrollers. 
//
// https://github.com/NormanDunbar/AVRTWIlib
//
//
// Norman Dunbar
// 17th April 2022
//------------------------------------------------------------

// Get the USART library
#include "USARTinterrupt.h"

// Get the TWI library
#include "TWIlib.h"

#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>


// Delay, milli-seconds to wait for the TWI to become ready
// or before attempting 
#define TWI_READY_DELAY 100


// Checks for success or failure of a TWI action.
void checkTWIAction(const char *function) {
    // Normally, we would go off and do something and leave
    // the interupts to get on with it. But here we will wait
    // for the transmit to complete. It's done when we get a
    // status code that isn't TWI_NO_RELEVANT_INFO.
    while (TWIInfo.errorCode == TWI_NO_RELEVANT_INFO)
        _delay_ms(1);

    // Grab the error code.
    uint8_t errorCode = TWIInfo.errorCode;

    // Did it work?
    if (errorCode != TWI_SUCCESS) {
        USARTwriteTextln("");
        USARTwriteText(function);
        USARTwriteText(": Error: 0x");
        USARTwriteIntHEX(errorCode);
#ifndef NO_ERROR_DATA_REQUIRED
        USARTwriteByte(' ');
        USARTwriteText(TWIGetLastError(errorCode));
#endif
        USARTwriteTextln("");
    } 
}



// Waits for TWI to complete the previous action. This is
// required as this example sketch is not doing any useful
// work between requesting data and reading it. If it was
// then this function would not normally be required.
void waitForTWI() {
    while(!isTWIReady()) {
        _delay_ms(TWI_READY_DELAY);
    }
}

// Send some configuration data and wait for it to be written.
// We will be reading next, so request a repeated start.
void sendConfig(void *data, uint8_t size) {

    // Wait for the transmit to successfully initiate.
    TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;
    uint8_t TXInitiated = TWI_TX_RX_NOT_READY;

    // This could hang if the TWI bus is up that famous creek!
    while (TXInitiated == TWI_TX_RX_NOT_READY)
        TXInitiated = TWITransmitData(data, size, true);

    // Did it work?
    checkTWIAction("sendConfig");
}


// Read some data back from the TWI. We don't need a repeated
// start, so we don't request one. We only need 2 bytes here.
void readData(uint8_t TWIaddress, void *data, uint8_t size) {
    // Try to get a successful read initiated. First, clear
    // any TWI Error codes.
    TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;

    // Then initialise the read.
   uint8_t TXInitiated = TWI_TX_RX_NOT_READY;
    
    // This could hang if the TWI bus is up that famous creek!
    while (TXInitiated == TWI_TX_RX_NOT_READY)
        TXInitiated = TWIReadData(TWIaddress, size, data, false);

    // Did it work?
    checkTWIAction("readData");
}


int main() {
    //========================================================
    //                                               S E T U P
    //========================================================

    // Don't forget, the Arduino does this for you. 
    sei();

    // Setup USART for 9600 baud.
    USARTinit(9600);  

    // Initialise the TWIlib to 200 KHz
    TWIInit(200);

    // Sign on message;
    USARTwriteTextln("\nLM75A Interrupt Driven Example\n");    

    // LM75A 7bit & 8bit write address
    const uint8_t LM75A_ADDRESS = 0x4F;
    const uint8_t LM75A_WR_ADDRESS = 
        (LM75A_ADDRESS << 1) & 0xfe;

    // LM75A register zero config.
    const uint8_t LM75A_TEMP_REGISTER = 0x00;

    // Configure the LM75A to use register zero.
    uint8_t configData[2] = {LM75A_WR_ADDRESS,
                             LM75A_TEMP_REGISTER};

    // Make sure that TWI is initialised before we send 
    // the configuration data.
    waitForTWI();
    sendConfig((void *)configData, 2);


    //========================================================
    //                                                 L O O P
    //========================================================
    while (1) {

        uint8_t temperature[2];

        // Make sure that TWI is ready for use again before we
        // read the temperature data. ReadData handles
        // converting the 7 bit address to the SLA+R address
        // into 8 bits.
        waitForTWI();
        readData(LM75A_ADDRESS, (void *)temperature, 2);
        USARTwriteInt(temperature[0]);
        USARTwriteTextln(temperature[1] & 0x80 ? ".5" : ".0");

        // Delay for a bit.
        _delay_ms(5000);
    }
}
