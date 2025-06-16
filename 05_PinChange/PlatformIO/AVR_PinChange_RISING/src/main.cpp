// Demonstrates an AVR Pin Change Interrupt
// which is triggered by ANY change on pin PD2
// but we are only actioning the interrupt when
// we detect a RISING change.
//
// Norman Dunbar
// 15 February 2020
//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// PB0 is Arduino pin D8.
ISR(PCINT2_vect) {
    // PD2/PCINT18 changed, toggle PB0 if RISING.
    if (PIND & (1 << PCINT18)) {
        PINB |= (1 << PINB0);
    }
}

int main() {
    // Setup: Set pin modes.
    // PB0 and PB5 are outputs.
    DDRB |= (1 << DDB0) | (1 << DDB5);

    // PD5 is input with pullup.
    DDRD |= (0 << DDD2);
    PORTD |= (1 << PORTD5);
    
    // Setup. Pin D2/PD2/PCINT18 to interrupt on change.
    cli();
    PCICR = (1 << PCIE2);
    PCMSK2 = (1 << PCINT18);
    PCIFR |= (1 << PCIF2)
    sei();

    // Loop. Flashes PB5 every 5 seconds.
    while (true) {
        PINB |= (1 << PINB5);
        _delay_ms(5000);
    }
}
