//------------------------------------------------------------
// This is a small demonstration of how a circular buffer is
// set up and used. It bears a little resemblance to the code
// used in the USART chapter. It is, hopefully, clear here as
// to how these buffers work.
//
// The example uses a 32 byte buffer which can hold 31 bytes.
// This is easily changed by editing BUFFER_SIZE in the header
// file.
//
//
// Norman Dunbar.
// 1st June 2020.
//------------------------------------------------------------

#include "cBuffer.h"

//------------------------------------------------------------
// Initialise the circular buffer. Sets the two pointers to
// the start of the buffer and as they are equal, this means
// empty.
//------------------------------------------------------------
void cBufferInit(circularBuffer *buf) {

    // Make sure buffer is empty.
    buf->headIndex = buf->tailIndex = 0;
}


//------------------------------------------------------------
// Any data in buffer to get? Unsigned return, buffers are
// always a positive length!
//------------------------------------------------------------
uint8_t cBufferBytesUsed(circularBuffer *buf) {

    // Return number of bytes written to buffer so far.
    return ((BUFFER_SIZE + buf->headIndex - buf->tailIndex) %
            BUFFER_SIZE);
}


//------------------------------------------------------------
// Any space in buffer to add to? Unsigned return, buffers are
// always a positive length!
//------------------------------------------------------------
uint8_t cBufferBytesFree(circularBuffer *buf) {

    // Return BUFFER_SIZE minus space used minus 1.
    return (BUFFER_SIZE - 1 - 
            cBufferBytesUsed(buf));
}


//------------------------------------------------------------
// Add a byte to a buffer.
//------------------------------------------------------------
bool cBufferAdd(circularBuffer *buf, uint8_t aByte) {

    // Any free space? Add the byte.
    if (!cBufferFull(buf)) {
        uint8_t nextPut = (buf->headIndex + 1) % BUFFER_SIZE;
        buf->cBuffer[nextPut] = aByte;
        buf->headIndex = nextPut;
        return true;
    }
    
    // Buffer full.
    return false;
}


//------------------------------------------------------------
// Get a byte from a buffer.
//------------------------------------------------------------
bool cBufferGet(circularBuffer *buf, uint8_t *aByte) {

   // Test if buffer is empty? If not, fetch a byte.
    if (!cBufferEmpty(buf)) {
        uint8_t nextGet = (buf->tailIndex + 1) % BUFFER_SIZE;
        *aByte = buf->cBuffer[nextGet];
        buf->tailIndex = nextGet;
        return true;
    }

    // Buffer is empty..
    return false;
}


//------------------------------------------------------------
// Is this buffer empty? Head == Tail
//------------------------------------------------------------
bool cBufferEmpty(circularBuffer *buf) {
    return buf->headIndex == buf->tailIndex;
}


//------------------------------------------------------------
// Is this buffer full? If ((head + 1) MOD BUFFER_SIZE) == Tail
//------------------------------------------------------------
bool cBufferFull(circularBuffer *buf) {
    return (buf->headIndex + 1) % BUFFER_SIZE == 
                 buf->tailIndex;
}

