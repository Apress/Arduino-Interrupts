//------------------------------------------------------------
// A sketch to use direct register access to read data from 
// an LM75A temperature sensor.  
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

// Save RAM by not using the error messages
#define NO_ERROR_DATA_REQUIRED

// Get the TWI library
#include "TWIlib.h"

// Delay, milli-seconds to wait for the TWI to become ready
// or before attempting 
#define TWI_READY_DELAY 100


//------------------------------------------------------------
// Global variables.
//------------------------------------------------------------

// LM75A 7bit & 8bit write address
const byte LM75A_ADDRESS = 0x4F;
const byte LM75A_WR_ADDRESS = (LM75A_ADDRESS << 1) & 0xfe;

// LM75A register zero configuration data.
const byte LM75A_TEMP_REGISTER = 0x00;

// Configure the LM75A to use register zero.
byte configData[2] = {LM75A_WR_ADDRESS,
                      LM75A_TEMP_REGISTER};


//------------------------------------------------------------
// Checks for success or failure of a TWI action.
//------------------------------------------------------------
void checkTWIAction(const char *function) {
    // Normally, we would go off and do something and leave
    // the interupts to get on with it. But here we will wait
    // for the transmit to complete. It's done when we get a
    // status code that isn't TWI_NO_RELEVANT_INFO.
    while (TWIInfo.errorCode == TWI_NO_RELEVANT_INFO)
        delay(1);

    // Grab the error code.
    byte errorCode = TWIInfo.errorCode;

    // Did it work?
    if (errorCode != TWI_SUCCESS) {
        Serial.println("");
        Serial.print(function);
        Serial.print(": Error: 0x");
        Serial.print(errorCode);
#ifndef NO_ERROR_DATA_REQUIRED
        Serial.print(' ');
        Serial.print(TWIGetLastError(errorCode));
#endif
        Serial.println();
    } 
}



//------------------------------------------------------------
// Waits for TWI to complete the previous action. This is
// required as this example sketch is not doing any useful
// work between requesting data and reading it. If it was
// then this function would not normally be required.
//------------------------------------------------------------
void waitForTWI() {
    while(!isTWIReady()) {
        delay(TWI_READY_DELAY);
    }
}


//------------------------------------------------------------
// Send some configuration data and wait for it to be written.
// We will be reading next, so request a repeated start.
//------------------------------------------------------------
void sendConfig(void *data, byte size) {

    // Wait for the transmit to successfully initiate.
    TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;
    byte TXInitiated = TWI_TX_RX_NOT_READY;

    // This could hang if the TWI bus is up that famous creek!
    while (TXInitiated == TWI_TX_RX_NOT_READY)
        TXInitiated = TWITransmitData(data, size, true);

    // Did it work?
    checkTWIAction("sendConfig");
}


//------------------------------------------------------------
// Read some data back from the TWI. We don't need a repeated
// start, so we don't request one. We only need 2 bytes here.
//------------------------------------------------------------
void readData(byte TWIaddress, void *data, byte size) {
    // Try to get a successful read initiated. First, clear
    // any TWI Error codes.
    TWIInfo.errorCode = TWI_NO_RELEVANT_INFO;

    // Then initialise the read.
   byte TXInitiated = TWI_TX_RX_NOT_READY;
    
    // This could hang if the TWI bus is up that famous creek!
    while (TXInitiated == TWI_TX_RX_NOT_READY)
        TXInitiated = TWIReadData(TWIaddress, size, data, false);

    // Did it work?
    checkTWIAction("readData");
}


void setup() {
    // Initialise the TWIlib to 200 KHz
    TWIInit(200);

    Serial.begin(9600);

    // Sign on message;
    Serial.println("\nLM75A Interrupt Driven Example\n");    



    // Make sure that TWI is initialised before we send 
    // the configuration data.
    waitForTWI();
    sendConfig((void *)configData, 2);
}


void loop() {
    byte temperature[2];

    // Make sure that TWI is ready for use again before we
    // read the temperature data. ReadData handles converting
    // the 7 bit address to the SLA+R address in 8 bits.
    waitForTWI();
    readData(LM75A_ADDRESS, (void *)temperature, 2);

    Serial.print("Current temperature: "); 
    Serial.print(temperature[0]);
    Serial.println(temperature[1] & 0x80 ? ".5" : ".0");

    // Delay for a bit.
    delay(5000);
}
