#ifndef CBUFFER_H
#define CBUFFER_H

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

#include <stdint.h>

//============================================================
// The HEAD index is where the most recent byte was added to
// the buffer.
//
// The TAIL index is where the most recent byte removed from
// the buffer was located.
//
// For best results, the buffer should be a power of two in 
// size. It will hold one less byte than the size of the 
// buffer.
//
// The buffer is EMPTY when HEAD == TAIL.
//
// The buffer is FULL when HEAD + 1 = TAIL.
//
// The next index is (index + 1 ) MOD buffer_size.
//
// The previous index is (index - buffer_size - 1) MOD
//                        buffer_size.
//
// Bytes used so far is (buffer_size + head - tail) MOD
//                      buffer_size.
//
// Bytes free is buffer_Size - 1 - bytes used so far.
//============================================================


//------------------------------------------------------------
// Buffer size must be a power of 2 and less than 256. 256
// is the maximum as the indexes are 8 bits wide.
//
// NOTE: Because of the use of two pointers, head and tail, 
// a buffer of size 'n' can only hold 'n'-1 items.
//------------------------------------------------------------
#define BUFFER_SIZE 32


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
    uint8_t cBuffer[BUFFER_SIZE];
} circularBuffer;


//------------------------------------------------------------
// Initialise a new buffer.
//------------------------------------------------------------
void cBufferInit(circularBuffer *buf);


//------------------------------------------------------------
// Anything in a buffer to read? Unsigned return, buffers are
// always a positive length! 
//------------------------------------------------------------
uint8_t cBufferBytesUsed(circularBuffer *buf);


//------------------------------------------------------------
// Any space in buffer to add to? Unsigned return, buffers are
// always a positive length!
//------------------------------------------------------------
uint8_t cBufferBytesFree(circularBuffer *buf);


//------------------------------------------------------------
// Add a byte to a buffer.
//------------------------------------------------------------
bool cBufferAdd(circularBuffer *buf, uint8_t aByte);


//------------------------------------------------------------
// Get a byte from a buffer.
//------------------------------------------------------------
bool cBufferGet(circularBuffer *buf, uint8_t *aByte);


//------------------------------------------------------------
// Is the buffer empty?
//------------------------------------------------------------
bool cBufferEmpty(circularBuffer *buf);


//------------------------------------------------------------
// Is the buffer full?
//------------------------------------------------------------
bool cBufferFull(circularBuffer *buf);



#endif // CBUFFER_H
