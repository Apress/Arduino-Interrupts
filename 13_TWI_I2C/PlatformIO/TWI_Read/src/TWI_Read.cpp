//------------------------------------------------------------
// A sketch to use direct register access to read data from 
// an LM75A temperature sensor.
//
// No interrupts are used. Polling only.
//
// The Arduino is running in "controller-receiver" mode.
//
// Norman Dunbar
// 14th August 2020
//------------------------------------------------------------

#include <stdint.h>
#include <util/delay.h>
#include "twi_defines.h"
#include "USARTinterrupt.h"

#define LM75A_ADDRESS 0x4F

// Default prescaler = 1. Atmel says leave alone!
#define PSCALER 1

// Calculate TWBR from desired SCL frequency.
#define SCL_HZ_TO_TWBR(F) (2 * PSCALER) * ((F_CPU/(F)) - 16)


void waitForStatus(uint8_t requiredStatus) {

    // Wait for the TWI to respond.
    while (!(TWCR & (1 << TWINT)));

    // Grab & print the status byte.
    uint8_t crxStatus = CRX_STATUS;
    USARTwriteText(" ");


    // Did we expect that status?
    if (crxStatus != requiredStatus) {
        USARTwriteText("Invalid status, expected 0x");
        USARTwriteIntHEX(requiredStatus);
        USARTwriteText(", received 0x");
        USARTwriteIntHEX(requiredStatus);

        
        // Light the LED on errors.
        PORTB |= (1 << PORTB5);

        // Try to stop TWI communications.
        TWCR = CRX_STOP;
        
        // Loop the loop forever.
        while (1) ;
    }
}


int main() {

    // SETUP ROUTINES.

    // Setup USART for 9600 baud.
    USARTinit(9600);  

    // Don't forget ... 
    sei();

    // Write a message;
    USARTwriteTextln("LM75A - Temperature Measurement");

    // Builtin LED is output and low.
    DDRB |= (1 << DDB5);
    PORTB &= ~(1 << PORTB5);

    // VERY VERY IMPORTANT!
    // Set  the SCL clock speed to 200 KHz.
    TWBR = SCL_HZ_TO_TWBR(200000);


    // MAIN LOOP Code.
    while (1) {
        uint8_t tempData[2];

        // Send start condition.
        TWCR = CRX_START;
        waitForStatus(CRX_START_SENT);

        // Send LM75A read address.
        TWDR = (((LM75A_ADDRESS) << 1) | 1);

        TWCR = CRX_SEND_READ_ADDRESS;
        waitForStatus(CRX_SLAR_ACK_RCVD);

        // Read first byte with ack.
        TWCR = CRX_READ_ACK;
        waitForStatus(CRX_DATA_ACK_SENT);
        tempData[0] = TWDR;
        
        // Read last byte with nack.
        TWCR = CRX_READ_NACK;
        waitForStatus(CRX_DATA_NACK_SENT);
        tempData[1] = TWDR;

        // And stop.
        TWCR = CRX_STOP;


        // Did we get a temperature?
        USARTwriteText(" ");
        USARTwriteInt((int)tempData[0]);
        USARTwriteTextln((tempData[1] & 0x80) ? ".5 C" : ".0 C");

        _delay_ms(5000);
    }
}
