//------------------------------------------------------------
// This is a test application to demonstrate the use of an 
// interrupt driven USART. This code demonstrates both TX and
// RX. This code has been tested with a buffer size ranging
// from 2 bytes (!) up to 64, all with no problems. The buffer
// size is a factor in how often it fills up, and this causes 
// blocking waits when adding data to the buffer.
//
// Norman Dunbar.
// 1st June 2020.
//------------------------------------------------------------

#include <util/delay.h>
#include "USARTinterrupt.h"
#include "printf.h"

const char Message[] = {"Welcome to AVR interrupted"
                        " USART communications."};

int main() {

    // Setup USART for 115200 baud.
    USARTinit(115200);  

    // Don't forget ... 
    sei();

    // Write a message;
    printf("%s\n\n", Message);


// These lines were used in testing, feel free to uncomment
// them if you want to exercise some of the other functions
// available in "printf".
/* 
    // Write Byte, Short and Long data.
    // It coincidentally reads "DEADBEEF"!
    short aShort = 222;       // DE.
    short aShort2 = 173;      // AD.
    long aLong = 48879L;      // BEEF.

    printf("The value of 'aShort' is %d.\n", aShort);
    printf("The value of 'aShort2' is %d.\n", aShort2);
    printf("The value of 'aLong' is %ld.\n\n", aLong);
    
    printf("'aLong' in binary is 0b%*lb.\n", 
           sizeof(aLong), aLong);
    printf("Negative 'aLong' in binary is 0b%*lb.\n", 
           sizeof(aLong), -aLong);

    // DeadBeef
    printf("Printing 'aShort', 'aShort2' and 'aLong'"
           " in hexadecimal, gives \"%X%X%X\".\n\n",
           aShort, aShort2, aLong);

    // Attempt to fill up the TX buffer. It won't fill up.
    for (uint8_t x = 0; x < 5; x++)  {
        printf("I hope you saw DEADBEEF above.\n");
    }

    // Flush the buffer - basically waits for TX to completely
    // empty the TX buffer. All characters will be sent to the
    // USART.
    USARTflush();

*/

    // Let the user know we are ready.
    printf("\nPlease type some text...\n");

    // Infinite loop.
    while (1) {
        while(USARTavailable()) {
            uint8_t c = USARTreadByte();
            USARTputChar(c);
        }
    }
}
