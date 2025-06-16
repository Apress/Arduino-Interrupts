#include <avr/interrupt.h>
#include <stdlib.h>
#include "USARTinterrupt.h"
#include "USARTbuffer.h"

// The buffers live elsewhere.
extern  volatile circularBuffer rxBuffer;
extern  volatile circularBuffer txBuffer;


//------------------------------------------------------------
// Initialise the USART. For this example we only require the
// baud rate. We assume, always a bad idea, that we are using
// 8 bits of data, no parity and 1 stop bit.
// Calling here also initialises the two circular buffers.
//------------------------------------------------------------
void USARTinit(const uint32_t baudRate) {
    // Baud rate factor.
    uint16_t baudFactor;

    // Initialise the two buffers. Do it first to stop
    // occasional random garbage being transmitted by 
    // the USART.
    cBufferInit(&rxBuffer);
    cBufferInit(&txBuffer);

    // Power up the USART.
    PRR &= ~(1 << PRUSART0);

    // Initialise the USART. This sets 1 stop bit and
    // no parity as a side effect. (They are the defaults.)
    UCSR0A = UCSR0B = UCSR0C = 0;

    // 8 bit data size.
    UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00));

    // Attempt high speed comms, required for 115200 baud
    // which doesn't work otherwise.
    UCSR0A = (1 << U2X0);

    // Calculate baud rate factor from baud rate. Assumes
    // high speed comms is in use.
    baudFactor = (F_CPU / (8 * baudRate)) - 1;

    // We need to keep the baudFactor below 4096 and
    // on 16 MHz boards, we need low speed if baudRate is 
    // 57600. Because both speeds give same actual baud
    // and error rates. Single speed more accurate perhaps?
    if ((baudFactor > 4095) ||
        ((F_CPU == 16000000L) && (baudRate == 57600))) {
            UCSR0A = 0;
            baudFactor = (F_CPU / (16 * baudRate)) - 1;
    }

    // Set UBRR0 with correct factor.
    UBRR0 = baudFactor;

    // Enable and start RX, enable TX but don't start yet.
    UCSR0B |= ((1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0));
}


//------------------------------------------------------------
// We are done with the USART. Stop the interrupts and power
// it down to save a couple of microamps. Waste not want not!
// This frees up pins PD0 and PD1 (D0 and D1) for GPIO.
//------------------------------------------------------------
void USARTend() {
    // Make sure TX is completed.
    USARTflush();

    // Stop UDRIE interrupt, TX and RX.
    STOP_UDRIE_INTERRUPT();
    STOP_TX();
    STOP_RX();

    // Clear both buffers.
    cBufferInit(&txBuffer);
    cBufferInit(&rxBuffer);

    // Power off the USART.
    PRR |= (1 << PRUSART0);
}

//------------------------------------------------------------
// This function is called from printf.c and is responsible
// for sending every character to be printed by printf, to the
// USART.
//------------------------------------------------------------
void _putchar(char ch) {

    // Add data to the buffer. Will wait for space if full.
    cBufferAdd(&txBuffer, ch);

    // Fire up the UDRE0 interrupt.
    START_UDRIE_INTERRUPT();
}

//------------------------------------------------------------
// This is not strictly necessary, we could just call 
// _putchar() but as all the other USART functions are named
// USARTxxxx, this is consitent. That is all!
//------------------------------------------------------------
void USARTputChar(uint8_t ch) {
    _putchar(ch);
}


//------------------------------------------------------------
// Calling here will not return until the current txBuffer has
// been completely sent to the USART. It's simply a busy
// wait until UDRE0 is disabled.
// It is probably not wise to call this function if:
//
// * The TXEN0 bit in USCR0B is not set; OR
// * If global interrupts are off;
//------------------------------------------------------------
void USARTflush() {
    volatile circularBuffer *tx = &txBuffer;
    if (tx->headIndex == tx->tailIndex)
        // Buffer empty.
        return;

    // Wait for interrupts to run down the buffer contents.
    while ((UCSR0B & UDRE0)) ;  // Wait ...
}


//------------------------------------------------------------
// Data Register Empty interrupt. Copy next byte to be sent
// from the txBuffer to UDR0. Will disable transmitter when we
// run out of bytes. No need for TX Complete interrupt.
//------------------------------------------------------------
ISR(USART_UDRE_vect) {
    // Grab next byte from txBuffer for transmission.
    volatile circularBuffer *tx = &txBuffer;
    int aByte= cBufferGet(tx);

    // Is txBuffer empty? Will be -1 if so.
    if (aByte != -1) {
        UDR0 = aByte;
    } else {
        // txBuffer is empty, disable UDRE interrupt.
        STOP_UDRIE_INTERRUPT();
    }

    // Clear the TX Complete interrupt flag.
    // The interrupt is unused but still sets the flag.
    UCSR0A |= (1 << TXC0);
}


//------------------------------------------------------------
// Read 1 byte of data from USART. Returns -1 on errors in
// receipt of the data, or buffer empty.
//------------------------------------------------------------
int USARTreadByte() {
    // This returns -1 on error.
    return cBufferGet(&rxBuffer);
}


//------------------------------------------------------------
// Read a number of bytes into a buffer. Returns count of
// bytes actually read. May include CR/LF characters in the
// output buffer.
//------------------------------------------------------------
int USARTreadBytes(uint8_t *buffer, int count) {
    int bytesRead = 0;
    int aByte;
    while (count) {
        aByte = USARTreadByte();
        if (aByte >= 0) {
            *buffer++ = aByte;
            count--;
            bytesRead++;
        } else {
            break;
        }
    }

    return bytesRead;
}


//------------------------------------------------------------
// Are there any data in the rxBuffer not yet read by the code
// if so, return how many bytes are available.
//------------------------------------------------------------
uint8_t USARTavailable() {
    return cBufferAvailable();
}

//------------------------------------------------------------
// Any errors in the USART receiver?
//------------------------------------------------------------
uint8_t USARTerror() {
    return rxBuffer.lastError;
}

//------------------------------------------------------------
// Receive complete interrupt. Copy data byte received to 
// circular buffer. We must check for errors before reading 
// the data from UDR0.
//------------------------------------------------------------
ISR(USART_RX_vect) {
    // Check for errors first.
    uint8_t errors = USART_CHECK_ERRORS();    

    // Read data from USART.
    uint8_t aByte = UDR0;

    // If no errors, add the byte to the RX buffer, if
    // there is free space, otherwise just drop the byte.
    volatile circularBuffer *rx = &rxBuffer;
    if (!errors) {
        if (!cBufferFull(rx)) {
            cBufferAdd(rx, aByte);
        }
    } else {
        // Save the error bits in case the code is
        // interested.
        rx->lastError = errors;
    }
}
