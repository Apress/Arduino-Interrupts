//============================================================
// Demonstrates an AVR Pin Change Interrupt which is triggered
// by a change on ANY pin in bank 2 (D0-D7/PD0-PD7).
//
// We are only interested in a FALLING interrupt on D2/PD2 and
// use it to toggle the LED on D8/PD0.
//
// Any other switches etc on bank 2 can be checked and used as
// required in the ISR.
//
// The built in LED on D13/PB5 will continue to flash at 5
// second intervals to emulate "work".
//
// Norman Dunbar
// 15 February 2020
//============================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t prevState = 0;

// Helper function to return only changed pins.
uint8_t whatChanged(uint8_t prevState, uint8_t currState) {
	return (prevState ^ currState);
}


// PB0 is Arduino pin D8.
ISR(PCINT2_vect) {
     // What changed?
    uint8_t currState = PIND;
    uint8_t chgdPins = whatChanged(prevState, currState);

    // Did D2/PD2/PCINT18 change? Was it FALLING.
    if (chgdPins & (1 << PCINT18)) {

        if (!(currState & (1 << PCINT18))) {
            // Must be FALLING, toggle PB0.
            PINB |= (1 << PINB0);
        }
    }

    // Do other pin checks here.
    // For example, check if D3/PD3/PCINT19 changed.
    if (chgdPins & (1 << PCINT19)) {
        // do something
    }

    // Must remember to set previousState.
    prevState = currState;
}


int main() {
    // Setup: Set pin modes.
    // D8/PB0 and D13/PB5 are outputs.
    DDRB |= (1 << DDB0) | (1 << DDB5);

    // D2/PD5 is input with pullup.
    DDRD |= (0 << DDD2);
    PORTD |= (1 << PORTD5);

    // Setup. All Bank 2 pins will interrupt on change.
    cli();
    PCICR = (1 << PCIE2);
    PCMSK2 = 0xFF;
    PCIFR |= (1 << PCIF2);
    
    // Grab the initial state of PIND.
    prevState = PIND;
    sei();

    // Loop. Flashes D13/PB5 every 5 seconds.
    while (true) {
        PINB |= (1 << PINB5);
        _delay_ms(5000);
    }
}
