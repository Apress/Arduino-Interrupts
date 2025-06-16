//------------------------------------------------------------
// This is an AVR version on the Arduino EEPROMInterrupt
// sketch which writes a message to the EEPROM, then reads it
// back and writes it to Serial. 
// 
// In this version, we use interrupts to do all the EEPROM and
// USART (Serial) input and output. Two different interrupts
// for the price of one!
//
// Norman Dunbar.
// 14th March 2020.
//------------------------------------------------------------

#include "EEPROMinterrupt.h"
#include "USARTinterrupt.h"
#include <util/delay.h>
#include <string.h>


#define EEPROM_ADDRESS (const uint8_t)0
const char Message[] = {"Greetings Interrupted AVR"
                        " EEPROM world!!"};

#define READ_THIS_MUCH_DATA 40
char loopBuffer[READ_THIS_MUCH_DATA + 1];


int main() {

    // Setup USART for 9600 baud.
    USARTinit(9600);  

    // Setup EEPROM
    EEPROMinit();

    // Don't forget interrupts! 
    sei();


    EEPROMerror result;
    
    result = EEPROMupdate((const uint8_t *)Message,
                           strlen(Message), 
                           EEPROM_ADDRESS);

    if (result != EEPROM_noError) {
        USARTwriteText("In main(), EEPROMupdate() error: ");
        USARTwriteInt(result);
        USARTwriteText("\r\n");
        // Will carry on, but results are undefined.
    }


    while (1) {
        loopBuffer[0] = '\0';
        result = EEPROMread((const uint8_t *)loopBuffer, 
                            READ_THIS_MUCH_DATA, 
                            EEPROM_ADDRESS, 
                            false);

        if (result != EEPROM_noError) {
            USARTwriteText("In main(), EEPROMread() error: ");
            USARTwriteInt(result);
            USARTwriteText("\r\n");
        }

        // Strings must be terminated.
        loopBuffer[READ_THIS_MUCH_DATA] = '\0';
        USARTwriteTextln(loopBuffer);
        _delay_ms(1000);    
    }
}
