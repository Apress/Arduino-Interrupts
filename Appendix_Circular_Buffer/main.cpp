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

#include <stdio.h>
#include "cBuffer.h"


void displayBufferSpace(circularBuffer *buf) {

    uint8_t freeSpace = cBufferBytesFree(buf);
    uint8_t usedSpace = cBufferBytesUsed(buf);
    
    printf("\n----------------------------------------------"
           "--------------\n");

    // Check the available space (for writing).
    printf("The buffer has %d free bytes for writing to.\n",
            freeSpace);

    // Check the available space (for reading).
    printf("The buffer has %d bytes to be read from.\n",
            usedSpace);

    // Print the buffer indices.
    printf("Buffer headIndex = %d, tailIndex = %d.\n",
            buf->headIndex, buf->tailIndex);

    // Full? Empty?
    if (cBufferEmpty(buf))
        printf("Buffer is EMPTY.\n");

    if (cBufferFull(buf))
        printf("Buffer is FULL.\n");

    printf("----------------------------------------------"
           "--------------\n");
}



int main() {
    // Define a new circular buffer.
    circularBuffer demoBuffer;

    // Create a pointer to the buffer.
    circularBuffer *buf = &demoBuffer;

    // Initialise the buffer.
    cBufferInit(buf);
    printf("\n1. The buffer has been initialised.\n");

    // Check the available space.
    displayBufferSpace(buf);


    // We have (BUFFER_SIZE - 1) bytes of space available.
    uint8_t freeSpace = cBufferBytesFree(buf);

    printf("\n2. Filling buffer with %d bytes.\n\n", freeSpace);

    for (uint8_t x = 0; x < freeSpace;  x++) {
        if (cBufferAdd(buf, (x + '0'))) {
            printf("cBufferAdd(): Byte %2d = '%c'\n", 
                   x + 1, x + '0');
        } else {
            printf("cBufferAdd() failed with x = %d\n", x);
        }
    }

    // Check the available space.
    displayBufferSpace(buf);


    // Try adding one more byte, should fail.
    printf("\n3. Adding one more byte - this will fail!\n\n");
    if (cBufferAdd(buf, '!')) {
        printf("cBufferAdd() OK! Something wrong here!\n");
    } else {
        printf("cBufferAdd() failed, as expected.\n");
    }


    // Remove 10 bytes.
    printf("\n4. Removing 10 bytes.....\n\n");
    for (uint8_t x = 0; x < 10; x++) {
        uint8_t ch;

        if (!cBufferGet(buf, &ch)) {
            printf("cBufferGet() failed with x = %d\n", x);
            break;
        }

        printf("cBufferGet(): Byte %2d = '%c'\n", x + 1, ch);
    }

    // Check the available space.
    displayBufferSpace(buf);

}


