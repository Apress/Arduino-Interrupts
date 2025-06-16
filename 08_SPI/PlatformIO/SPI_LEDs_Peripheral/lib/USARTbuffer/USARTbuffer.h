#ifndef USARTBUFFER_H
#define USARTBUFFER_H

#include <stdint.h>

//============================================================
// We need a buffer for transmitting data. This implementation
// uses a circular buffer. The buffers used here are designed
// to be 255 or fewer characters and so, indexes etc will fit
// a uint8_t.
//============================================================
// BufferFull = ((head + 1) == tail)
// BufferEmpty = (tail == head)
// IncrementIndex = (index + 1) % BUFFER_SIZE
// DecrementIndex = (index - BUFFER_SIZE -1) % BUFFER_SIZE
// Available = (BUFFER_SIZE + (head - tail)) % BUFFER_SIZE
// FreeToWrite = BUFFER_SIZE - available_to_get.
//
// Head = where last byte was added to buffer. Will be 
//        incremented before next add.
//
// Tail = where last byte was read from buffer. Will be
//        incremented before next get.
//============================================================
// Using two pointers in this way means that the maximum 
// number of characters in the buffer is the BUFFER_SIZE - 1
// as head and tail cannot be equal when adding a byte. They
// can be equal when getting a byte - the final one in the
// buffer.
//============================================================


//------------------------------------------------------------
// Buffer size must be a power of 2 and less than 256. If you
// need more than 256 then update the uint8_t data types to 
// uint16_t or results will be "undefined"! You will also have
// to make sure that any amendments to head or tail indexes 
// are protected by an atomic block - the AVR doesn't have a
// single instruction to load a 16 bit variable. 8 bits are
// ok as a load cannot be interrupted.
//
// NOTE: Because of the use of two pointers, head and tail, 
// a buffer of size 'n' can only hold 'n'-1 items.
//------------------------------------------------------------
#define BUFFER_SIZE 64

//------------------------------------------------------------
// Buffer full/empty error returns.
//------------------------------------------------------------
#define ERR_BUFFER_OK 0
#define ERR_BUFFER_FULL -1
#define ERR_BUFFER_EMPTY -2

//------------------------------------------------------------
// The actual buffer. Contains its own head and tail indexes.
// All buffers will be defined with  the same size, for ease
// of use and simplicity. (Mine!)
//
// headIndex is the byte position where the most recent byte
// was added. This is incremented just before adding the next
// byte to the buffer.
//
// tailIndex is the byte position where the most recent byte
// was removed. This is incremented just before getting the
// next byte from the buffer.
//------------------------------------------------------------
typedef struct circularBuffer {
    uint8_t headIndex;
    uint8_t tailIndex;
    int8_t  lastError;
    uint8_t cBuffer[BUFFER_SIZE];
} circularBuffer;


//------------------------------------------------------------
// Initialise a new buffer.
//------------------------------------------------------------
void cBufferInit(volatile circularBuffer *buf);


//------------------------------------------------------------
// Anything in a buffer to get? Unsigned return, buffers are
// always a positive length! Allows code to check if rxBuffer
// has data. Cf Serial.available() on Arduino.
//------------------------------------------------------------
uint8_t cBufferAvailable();


//------------------------------------------------------------
// Add a byte to a buffer.
//------------------------------------------------------------
void cBufferAdd(volatile circularBuffer *buf, uint8_t aByte);


//------------------------------------------------------------
// Get a byte from a buffer.
//------------------------------------------------------------
int cBufferGet(volatile circularBuffer *buf);


//------------------------------------------------------------
// Is this buffer full? If head == Tail
//------------------------------------------------------------
bool cBufferFull(volatile circularBuffer *buf);


//------------------------------------------------------------
// Is this buffer empty? Head == Tail
//------------------------------------------------------------
bool cBufferEmpty(volatile circularBuffer *buf);

#endif // USARTBUFFER_H
