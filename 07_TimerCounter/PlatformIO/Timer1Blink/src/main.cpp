#include "avr/io.h"
#include "avr/interrupt.h"
#include <util/delay.h>


//============================================================
// An AVR applicaton to flash an LED on pin D8
// using Timer/counter 1 in normal mode and with
// a 256 prescaler.
// 
// Norman Dunbar
// 22 February 2020.
//============================================================

ISR(TIMER1_OVF_vect) {
    // Flash D8 every 1.048576 seconds.
    PINB |= (1 << PINB0);

    // Adjust the timer to flash every second.
    TCNT1 = 3036;
}


void enableTimer1() {
    // Choose a timer/counter mode.
    TCCR1A = ((0 << WGM11) | (0 << WGM10));
    TCCR1B = ((0 << WGM13) | (0 << WGM12));
    
    // Clear the interrupt flag.
    TIFR1 = (1 << TOV1);
    
    // Enable the Overflow Interrupt.
    TIMSK1 = (1 << TOIE1);
    
    // Choose a prescaler to start the timer/counter.
    TCCR1B |= ((1 << CS12) | (0 << CS11) | (0 << CS10));
}



int main() {
    // setup.
    // PB0 and PB5 are output.
    DDRB = ((1 << DDB0) | (1 << DDB5));
    enableTimer1();
    sei();

    // Loop. Flash PB5 every 5 seconds. 
    while (true) {
        PINB |= (1 << PINB5);
        _delay_ms(5000);
    }
}


