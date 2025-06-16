//============================================================
// An AVR application to flash an LED on pin D8 and another on
// D9 using Timer/counter 1 in normal mode, with a 256 
// prescaler and using the two compare match interrupts.
// 
// Norman Dunbar
// 26 February 2020.
//============================================================

#include "avr/io.h"
#include "avr/interrupt.h"
#include <util/delay.h>


ISR(TIMER1_COMPA_vect) {
	// Toggle pin PB0 every interrupt.
	PINB |= (1 << PINB0);
}


ISR(TIMER1_COMPB_vect) {
	// Toggle pin PB1 every interrupt.
	PINB |= (1 << PINB1);
}


//============================================================
// A function for Arduino and AVR to enable Timer/counter 1
// in normal mode and with a 256 prescaler and the compare A/B
// interrupts enabled. The interrupts will trigger every 
// 1.048576 seconds. 
// 
// NOTE: This will mess up PWM on pins D9 and D10 if used in
// an Arduino sketch, Timer/counter 1 allows analogWrite() on 
// those two pins.
//============================================================
void enableTimer1CompMatch(uint16_t ocr1a, uint16_t ocr1b) {
    // Choose normal timer/counter mode.
    // This overrides the Arduino setup.
    TCCR1A = ((0 << WGM11) | (0 << WGM10));
    TCCR1B = ((0 << WGM13) | (0 << WGM12));
    
    // Clear the interrupt flags.
    TIFR1 = ((1 << OCF1A) | (1 << OCF1B));
    
	// Set the compare match registers.
	OCR1A = ocr1a;
	OCR1B = ocr1b;
    
    // Enable both the Compare Match Interrupts.
    TIMSK1 = ((1 << OCIE1A) | (1 << OCIE1B));
    
    // Choose a prescaler to start the timer/counter.
	// Divide by 256 is chosen here.
    TCCR1B |= ((1 << CS12) | (0 << CS11) | (0 << CS10));
}



int main() {
    // setup.
    // PB0, PB1 and PB5 are output pins.
    DDRB = ((1 << DDB0) | (1 << DDB1) | (1 << DDB5));

	// Enable Timer/counter 1.
	// PB0 = COMP1A toggles at 43,692 roughly 2/3 of the MAX.
	// PB1 = COMP1B toggles at 21,845 roughly 1/3 of the MAX.
	enableTimer1CompMatch(43692, 21845);
    sei();

    // Loop. Flash PB5 every 5 seconds. 
    while (true) {
        PINB |= (1 << PINB5);
        _delay_ms(5000);
    }
}
