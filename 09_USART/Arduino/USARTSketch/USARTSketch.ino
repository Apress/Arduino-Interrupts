//------------------------------------------------------------
// This sketch demonstrates printing to the USART using the
// underlying interrupts. There's nothing unusual here - apart
// from the use of "printf()" which is not notrmally available
// on the Arduino. See the text (in the book) for details.
//
// Norman Dunbar.
// 1st June 2020.
//------------------------------------------------------------

#include <LibPrintf.h>

//------------------------------------------------------------
// A simple sign-on message.
//------------------------------------------------------------
const char Message[] = {"Welcome to Arduino interrupted"
                        " USART communications."};



//------------------------------------------------------------
// Initialise the Serial Monitor to 115200 baud and sign on.
// A few numerical examples of printf() are then exercised.
// Unless, of course, they are commented out!
//------------------------------------------------------------
void setup() {

    // Setup USART for 115200 baud.
    Serial.begin(115200);  
    
    // Write sign-on message;
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
    
    printf("'aLong' in binary is 0b%*lb.\n", sizeof(aLong), aLong);
    printf("Negative 'aLong' in binary is 0b%*lb.\n", sizeof(aLong), -aLong);

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
    Serial.flush();

*/

    // Let the user know we are ready.
    printf("\nPlease type some text...\n");
}



//------------------------------------------------------------
// Simply loop around copying user input back to the USART
//------------------------------------------------------------
void loop() {
    while(Serial.available()) {
        uint8_t c = Serial.read();
        printf("%c", c);
    }
}
