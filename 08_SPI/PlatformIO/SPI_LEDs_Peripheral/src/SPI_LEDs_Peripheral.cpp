/*============================================================
 * Using SPI on a controller "normdunio" to send a message to 
 * an Uno. The Uno will receive each byte and display the 
 * binary value on a set of 8 LEDs attached to pins D0 through
 * D7.
 *===========================================================*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#if USE_SERIAL
#include "USARTinterrupt.h"
#include "printf.h"
#endif

#if CONTROLLER
    // A message to be flashed on the peripheral's LEDs.
    const char *message = "The quick brown fox jumps"
                          " over the lazy dog!\n";
#else
    // Store a single character from SPI.
    char receivedByte = 0; 
    bool newByteReceived = false;   
#endif

// Pin definitions. These are not setup in <avr/io.h> and
// its siblings. These are the pins that must be configured
// as only MISO is done automatically when this device is 
// an SPI controller.
#define DDB_SCK    DDB5     // Clock pin.
#define DDB_MISO   DDB4     // Data in.
#define DDB_MOSI   DDB3     // Data out.
#define DDB_SS     DDB2     // Controller's SS pin.

// Short is the delay between characters.
#define THE_DELAY 100


#if CONTROLLER
// The SPI interrupt handler triggers when a byte has been
// transmitted and received. All we have to do here is 
// disconnect the UNO from SPI.
ISR(SPI_STC_vect) {
    // Disable the Uno from SPI.
    PORTB |= (1 << DDB_SS);
}
#else
ISR(SPI_STC_vect) {
    // When the peripheral receives a byte, save it.
    receivedByte = SPDR;
    newByteReceived = true;

    // Light up the LEDs.
    PORTD = receivedByte;
}
#endif

void setupSPI(bool controller) {
    // Disable global interrupts.
    cli();

    // Enable SPI as a controller;
    // Shift data MSB first;
    // F_CPU / 128 = 125 KHz;
    // SPI interrupts are enabled.
    // Phase and polarity are 0|0 or 1|1
    // as we need to clock data on a rising edge.
      SPCR = (1 << SPE)  | 
             (1 << DORD) |
             (1 << SPR1) | (1 << SPR0) | 
             (0 << CPOL) | (0 << CPHA) |
             (1 << SPIE);
          
      if (controller) {
          SPCR |= (1 << MSTR);
      }      

    // Clear SPI interrupt flag, if set. Volatile to stop
    // compiler optimising it away.
    volatile uint8_t ignoreMe = SPSR;
    ignoreMe = SPDR;

    // Enable global interrupts.
    sei();
}


int main() {

//------------------------------------------------------------
//                                                   S E T U P 
//------------------------------------------------------------
#if CONTROLLER
    // MOSI, SCK and SS need to be output. MISO is 
    // correctly configured already automatically.
    // WARNING: If the SS pin for the Arduino is 
    // not an output pin, then SPI will not work!
    DDRB = ((1 << DDB_SCK) | (1 << DDB_MOSI) | (1 << DDB_SS));   


    // Make sure our SS is pulled high. This
    // disconnects the Uno from SPI. 
    PORTB |= (1 << DDB_SS);

    // Now configure the Duemilanove as a controller.
    setupSPI(true);
#else
    // MOSI, SCK and SS are automatically configured
    // as inputs. Only MISO needs to be configured as 
    // an output 
    DDRB |= (1 << DDB_MISO);
    
    // Set up the Uno as a peripheral.
    setupSPI(false);

#if USE_SERIAL
    // Fire up the Serial Monitor.
    USARTinit(9600);
#endif

    // PD0 to PD7 are all output for the LEDs.
    DDRD = 0xFF; 
#endif    

//------------------------------------------------------------
//                                                     L O O P
//------------------------------------------------------------
    while (true) {
    #if CONTROLLER
        // Copy some data to SPI. (Need to lose constness!)
        char *p = (char *)message;
        
        while (*p) { 
            // Enable the Uno.
            PORTB &= ~(1 << DDB_SS);

            // Copy each byte of the message to SPI.
            SPDR = *p;
            _delay_ms(THE_DELAY);

            // Don't forget to increment to the next character!
            p++;

            // The interrupt handler will disconnect between each byte.
        }
    #else
        // The peripheral will output the received byte. If a space
        // is received, turn off the LEDS.

#if USE_SERIAL
        printf("%c", newByteReceived);  // REMOVE this and it stops working!!!!!!!
#endif
        // It seems that by the time we get here, newByteReceived has been falsified!
        if (newByteReceived) {  
#if USE_SERIAL
            printf("%c", receivedByte);
#endif
            if (receivedByte == 0x20) {
                PORTD = 0;
            }

            newByteReceived = false;
        }
    #endif
    }
}
