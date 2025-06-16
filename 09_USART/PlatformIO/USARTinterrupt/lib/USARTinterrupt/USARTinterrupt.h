#ifndef USARTINTERRUPT_H
#define USARTINTERRUPT_H

#include <stdint.h>
#include <USARTbuffer.h>
#include <avr/interrupt.h>
#include "printf.h"


//============================================================
// USART TX Stuff follows.
//============================================================

//------------------------------------------------------------
// This makes things look like function calls.
//------------------------------------------------------------
#define START_UDRIE_INTERRUPT() UCSR0B |= (1 << UDRIE0)
#define STOP_UDRIE_INTERRUPT()  UCSR0B &= ~(1 << UDRIE0)
#define STOP_TX()               UCSR0B &= ~(1 << TXEN0)


//------------------------------------------------------------
// Initialise the USART. We always use 8N1 here -- this is
// just a demo after all.
//------------------------------------------------------------
void USARTinit(uint32_t baudRate);


//------------------------------------------------------------
// End usage of the USART.
//------------------------------------------------------------
void USARTend();


//------------------------------------------------------------
// Flush all data to TX from the buffer to the USART.
//------------------------------------------------------------
void USARTflush();


//------------------------------------------------------------
// Send a single byte to the USART. 
// _putchar() is caled internally by printf() and must be
// defined. USARTputChar() is just there for consistency!
//------------------------------------------------------------
void _putchar(char ch);
void USARTputChar(uint8_t ch);


//============================================================
// USART RX Stuff follows.
//============================================================


//------------------------------------------------------------
// This makes things look like function calls.
//------------------------------------------------------------
#define STOP_RX() UCSR0B &= ~((1 << RXEN0) | (1 << RXCIE0))

#define RX_ERROR_BITS ((1 << FE0) | (1 << DOR0) | (1 << UPE0))
#define USART_CHECK_ERRORS() UCSR0A & (RX_ERROR_BITS)

#define RX_FRAME_ERROR(x) (x) & (1 << FE0)
#define RX_OVERRUN(x) (x) & (1 << DOR0)
#define RX_PARITY_ERROR(x) (x) & (1 << UPE0)
#define RX_BUFFER_EMPTY -1


//------------------------------------------------------------
// Read a single byte from the USART's rxBuffer. Returns -1
// on error - which means, buffer empty.
//------------------------------------------------------------
int USARTreadByte();

//------------------------------------------------------------
// Read a number of bytes into a buffer. Returns count of
// bytes actually read.
//------------------------------------------------------------
int USARTreadBytes(uint8_t *buffer, int count);

//------------------------------------------------------------
// Are there any data in the rxBuffer not yet read by the code
// if so, return how many bytes are available.
//------------------------------------------------------------
uint8_t USARTavailable();


//------------------------------------------------------------
// Any errors in the USART receiver?
//------------------------------------------------------------
uint8_t USARTerror();

#endif // USARTINTERRUPT_H

