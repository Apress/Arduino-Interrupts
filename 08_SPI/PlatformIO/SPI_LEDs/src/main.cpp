/*============================================================
 * Using SPI to drive an 74HC595 shift register which is in
 * turn, driving a set of 560R resistors to eight LEDs.
 *
 * Arduino ----------> 74HC595
 * ----------------------------
 * PB0/D8/14       --> 12/RCLK
 * PB1/D9/15       --> 13/~OE (active low)
 * PB3/D11/MOSI/17 --> 14/SER
 * PB5/D13/SCK/19  --> 11/SRCLK  
 * 5V              --> 16/VCC
 * GND             -->  8/GND

 * 74HC595 
 * -------
 *  
 * QA/15  --> 560R --> LED
 * QB/1   --> 560R --> LED
 * QC/2   --> 560R --> LED
 * QD/3   --> 560R --> LED
 * QE/4   --> 560R --> LED
 * QF/5   --> 560R --> LED
 * QG/6   --> 560R --> LED
 *
 * ~OE/13 --> 10K --> VCC
 * ~MR/10 --> 10K --> VCC
 *
 * NONE of the 74HC595 tutorials on t'web mention 
 * these pullups. How do those tutorials work?
 *===========================================================*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>


// Pin definitions. These are not setup in <avr/io.h> and
// its siblings. These are the pins that must be configured
// as only MISO is done automatically when this device is 
// an SPI controller.
#define DDB_SCK    DDB5     // Clock pin.
#define DDB_MOSI   DDB3     // Data pin to 74HC595.
#define DDB_SS     DDB2     // Controller's SS pin.

// And extras for the 74HC595.
#define HC595_OE   DDB1     // D9/PB1 = 74HC595 output enable.
#define HC595_CS   DDB0     // D8/PB0 = 74HC595's CS pin.

// Short is the delay between numbers.
#define SHORT_DELAY 100

// The SPI interrupt handler triggers when a byte has been
// transmitted and received. All we have to do here is 
// disconnect the 74HC595 from SPI and latch the new data
// onto the output pins to light up the LEDS.
ISR(SPI_STC_vect) {
    // Read the received data here. As we are not expecting
    // any data from the 74HC595, we simply ignore it. We 
    // don't even have to read it, but we do anyway.
    // Volatile to stop compiler optimising it away.
    volatile uint8_t ignoreMe = SPDR;

    // Disable the 74HC595 from SPI.
    PORTB |= (1 << HC595_CS);

    // Then latch the new data onto the 74HC595's output pins.
    PORTB &= ~(1 << HC595_OE);
}


void setupSPI() {
    // Disable global interrupts.
    cli();

    // Enable SPI as a controller;
    // Shift data LSB first;
    // F_CPU / 128 = 125 KHz;
    // SPI interrupts are enabled.
    // Phase and polarity are 0|0 or 1|1
    // as we need to clock data on a rising edge.
      SPCR = (1 << SPE)  | (1 << MSTR) | 
             (1 << DORD) |
             (1 << SPR1) | (1 << SPR0) | 
             (0 << CPOL) | (0 << CPHA) |
             (1 << SPIE);

    // Clear SPI interrupt flag, if set. Volatile to stop
    // compiler optimising it away.
    volatile uint8_t ignoreMe = SPSR;
    ignoreMe = SPDR;

    // Enable global interrupts.
    sei();
}

void displayNumber(uint8_t value) {
    // Enable the 74HC595 by pulling its CS pin low...
    PORTB &= ~(1 << HC595_CS);

    // while disabling its output latch ~OE by pulling
    // it high. Prevents flicker as data are loaded.
    PORTB |= (1 << HC595_OE);

    // Load the value into the data register.
    SPDR = value;
}


int main() {
    // MOSI, SCK and SS need to be output. MISO is 
    // correctly configured already automatically.
    // WARNING: If the SS pin for the Arduino is not
    // an OUTPUT pin, then SPI will  not work!
    DDRB = (1 << DDB_MOSI) | (1 << DDB_SCK)  |
           (1 << DDB_SS)   | (1 << HC595_OE) |
           (1 << HC595_CS);

    // Make sure our SS is pulled high.
    PORTB |= (1 << DDB_SS);

    // Ensure 74HC595 is disconnected from SPI.
    PORTB |= (1 << HC595_CS);
    
    // Disable 74HC595's output pins before SPI is setup.
    PORTB |= (1 << HC595_OE);
    setupSPI();

    while (1) {
        // Display 0 - 255 with a delay between each digit.
        for (uint8_t i = 0; i <= 255; i++) {
            displayNumber(i);
            _delay_ms(SHORT_DELAY);
        }
    }
}
