#include "USARTbuffer.h"

volatile circularBuffer rxBuffer;
volatile circularBuffer txBuffer;

//------------------------------------------------------------
// Initialise the circular buffer. Sets the two pointers to
// the start of the buffer and as they are equal, this means
// empty. Clears the error flag - only used by the rxBuffer.
//------------------------------------------------------------
void cBufferInit(volatile circularBuffer *buf) {
    buf->headIndex = buf->tailIndex = 0;
    buf->lastError = 0;
}


//------------------------------------------------------------
// Any data in rxBuffer to get? Unsigned return, buffers are
// always a positive length!
//------------------------------------------------------------
uint8_t cBufferAvailable() {
    volatile circularBuffer *rx = &rxBuffer;
    return ((BUFFER_SIZE + rx->headIndex - rx->tailIndex) %
            BUFFER_SIZE);
}

//------------------------------------------------------------
// Add a byte to a buffer.
//------------------------------------------------------------
void cBufferAdd(volatile circularBuffer *buf, uint8_t aByte) {

    // Temporary new headIndex calculated.
    uint8_t nextPut;
    nextPut = (buf->headIndex + 1) % BUFFER_SIZE;

    // Wait for space in buffer. UDRE0 Interrupt is on so this
    // shouldn't take too long. ONLY FOR TX Buffers!
    while (cBufferFull(buf))  ; // Wait...

    // Free space, write the byte.
    buf->cBuffer[nextPut] = aByte;
    buf->headIndex = nextPut;
}

//------------------------------------------------------------
// Get a byte from a buffer. Returns -1 is buffer is empty.
//------------------------------------------------------------
int cBufferGet(volatile circularBuffer *buf) {
    uint8_t nextGet;
    uint8_t aByte;

    // Test if buffer is empty? If not, fetch a byte.
    if (!cBufferEmpty(buf)) {
        nextGet = (buf->tailIndex + 1) % BUFFER_SIZE;
        aByte = buf->cBuffer[nextGet];
        buf->tailIndex = nextGet;
        buf->lastError = ERR_BUFFER_OK;
        return aByte;
    } 

    // Buffer is empty, return error code.
    return -1;
}

//------------------------------------------------------------
// Is this buffer full? If head == Tail
//------------------------------------------------------------
inline bool cBufferFull(volatile circularBuffer *buf) {
    return (buf->headIndex + 1) % BUFFER_SIZE == 
                 buf->tailIndex;
}

//------------------------------------------------------------
// Is this buffer empty? Head == Tail
//------------------------------------------------------------
inline bool cBufferEmpty(volatile circularBuffer *buf) {
    return buf->headIndex == buf->tailIndex;
}