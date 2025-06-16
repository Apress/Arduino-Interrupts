//------------------------------------------------------------
// A sketch to use direct register access to read data from 
// an LM75A temperature sensor.
//
// Interrupts are used.
//
// The Arduino is running in "controller-receiver" mode.
//
// Norman Dunbar
// 14th August 2020
//------------------------------------------------------------

#include <util/delay.h>
#include <stdint.h>
#include "twi_defines.h"
#include "USARTinterrupt.h"

#define LM75A_ADDRESS 0x4F

// Default prescaler = 1. Atmel says leave alone!
#define PSCALER 1

// Calculate TWBR from desired SCL frequency.
#define SCL_HZ_TO_TWBR(F) (2 * PSCALER) * ((F_CPU/(F)) - 16)


// Storage for two data bytes from the sensor. The ISR will
// fill these in, so they are declared volatile. However we
// also need to be sure we have received both data bytes.
volatile uint8_t tempData[2];
volatile uint8_t tempAvailable;

// An error flag. If the ISR can't deal with a problem,
// this will tell us and the loop() will sort it out.
volatile uint8_t twiStatus;

int main() {

    // SETUP ROUTINES.

    // Setup USART for 9600 baud.
    USARTinit(9600);  
    USARTwriteTextln("LM75A - Temperature Measurement");

    // Don't forget ... 
    sei();

    // Builtin LED is output and low.
    DDRB |= (1 << DDB5);
    PORTB &= ~(1 << PORTB5);


    // VERY VERY IMPORTANT!
    // Set  the SCL clock speed to 200 KHz.
    TWBR = SCL_HZ_TO_TWBR(200000);


    // MAIN LOOP Code.
    while (1) {

        // No ISR errors, so far.
        twiStatus = 0;    
        
        // Send start condition with interrupts enabled.
        TWCR = CRX_START;

        // Did we get a temperature?
        if (twiStatus != -1) {
            if (tempAvailable) {
            USARTwriteText(" ");
            USARTwriteInt((int)tempData[0]);
            USARTwriteTextln((tempData[1] & 0x80) ? ".5 C" : ".0 C");


            }
        } else {
            // Oops! problems in the ISR.
            PORTB |= (1 << PORTB5);
            while (1) ;
        }

        _delay_ms(5000);
    }
}


//------------------------------------------------------------
// Interrupt Service Routine for the TWI Interrupt.
//------------------------------------------------------------
ISR(TWI_vect) {
    // When we are here, the TWI hardware has completed
    // an action. CRX_STATUS will return that status and
    // we determine the next action depending on that. If
    // we get an unexpected status, we close down TWI
    // communications.

    // Get the status for the action which just completed.
    uint8_t crxStatus = CRX_STATUS;
    twiStatus = 0;

    //--------------------------------------------------------
    // Controller Receiver Status Codes.
    //--------------------------------------------------------
    // Ok, lets figure it out, engage top gear for your mind!
    // From the top, we are looking for:
    // CRX_START_SENT - send the sensor address if detected.
    // CRX_SLAR_ACK_RCVD - sensor found. Read first byte, ACK
    // CRX_SLAR_NACK_RCVD - no sensor found, Stop.
    // CRX_DATA_ACK_SENT - first byte read ok, read next byte.
    // CRX_DATA_NACK_SENT - last byte read, NACK, stop.

    switch (crxStatus) {
        case CRX_START_SENT:
            // Start sent ok. Send the sensor read address,
            // flag up no data available yet.
            tempAvailable = 0;
            TWDR = (((LM75A_ADDRESS) << 1) | 1);
            TWCR = CRX_SEND_READ_ADDRESS;
            return;
            
        case CRX_SLAR_ACK_RCVD:
            // Sensor responded to its address. Request one 
            // data byte and send an acknowledgement.
            TWCR = CRX_READ_ACK;
            return;
            
        case CRX_SLAR_NACK_RCVD:
            // Sensor not responding, sent a stop and
            // clear the TWIE bit to disable interrupts..
            TWCR = CRX_STOP;
            return;

        case CRX_DATA_ACK_SENT:
            // We have received a byte of data and sent an
            // acknowledgement. This is the first of two
            // bytes we need. Fetch the byte and request the
            // next one.
            tempData[0] = TWDR;
            TWCR = CRX_READ_NACK;
            return;
            
        case CRX_DATA_NACK_SENT:
            // We have received a byte of data and NOT sent
            // an acknowledgement. This is the last of two
            // bytes we need. Fetch the byte and stop the
            // TWI communications and the interrupt.
            tempData[1] = TWDR;
            TWCR = CRX_STOP;

            // We have all our data.
            tempAvailable = 1;
            return;

        default:
            // Oops! Problem.
            TWCR = CRX_STOP;
            twiStatus = -1;
            return;            
    }
}
