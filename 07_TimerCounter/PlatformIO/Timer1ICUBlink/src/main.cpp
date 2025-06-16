//=============================================================
// A sketch to toggle an LED when an Input Capture Unit (ICU) 
// interrupt is received on pin ICP1/PB0. The LED will be on 
// PB1 and the built in LED on D13 will be flashing on a 5 
// second cycle.
//
// Norman Dunbar
// 27 February 2020.
//=============================================================

#include "avr/io.h"
#include "avr/interrupt.h"
#include <util/delay.h>


ISR(TIMER1_CAPT_vect) {
	// Toggle pin PB1 every interrupt.
	PINB |= (1 << PINB1);
}


//=============================================================
// A function for Arduino and AVR to enable Timer/counter 1 in
// normal mode and with a 1024 prescaler and the input capture
// interrupt enabled. The interrupt will trigger every 4.194304
// seconds. 
// 
// NOTE: This will mess up PWM on pins D9 and D10 if used in an
// Arduino sketch, Timer/counter 1 allows analogWrite() on 
// those two pins.
//=============================================================
void enableTimer1ICU() {
    // Choose a timer/counter mode, with ICU enabled. The ICU
    // is a falling edge trigger. (ICES1 = 0).
    // This overrides the Arduino setup.
    TCCR1A = ((0 << WGM11) | (0 << WGM10));
    TCCR1B = ((0 << WGM13) | (0 << WGM12) | (0 << ICES1));
    
    // Clear the ICU interrupt flag.
    TIFR1 = (1 << ICF1);
    
    // Enable the Input Capture Interrupt.
    TIMSK1 = (1 << ICIE1);
    
    // Choose a prescaler to start the timer/counter.
    // Divide by 1024 is chosen
    TCCR1B |= ((1 << CS12) | (0 << CS11) | (1 << CS10));
}


int main() {
    // setup.
    // PB1 and PB5 are output pins.
    DDRB = ((1 << DDB1) | (1 << DDB5));

    // PB0 is input with pullup.
    DDRB |= (0 << DDB0);        // Not strictly necessary!
    PORTB |= (1 << PORTB0);

	// Enable Timer/counter 1 with ICU interrupt.
	enableTimer1ICU();
    sei();

    // Loop. Flash PB5 every 5 seconds. 
    while (true) {
        PINB |= (1 << PINB5);
        _delay_ms(5000);
    }
}
