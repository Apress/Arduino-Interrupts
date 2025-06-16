#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


ISR(INT0_vect) {
	// LED on Arduino D8 = Port B, pin 0.
	PORTB |= (1 << PORTB0);
}


ISR(INT1_vect) {
	// LED on Arduino D8 = Port B, pin 0.
	PORTB &= ~(1 << PORTB0);
}

int main() {
	// Setup D13 (PB5) and D8 (PB0) as output.
	DDRB |= ((1 << DDB5) | (1 << DDB0));

	// Setup Switches on D2 and D3 (PD2 and PD3).
	DDRD &= ~((1 << DDD2) | (1 << DDD3));

	// Configure INT0 and INT1 on a falling edge.
	EICRA = ((1 << ISC11) | (0 << ISC10) |
             (1 << ISC01) | (0 << ISC00));

	// Enable the INT0 and INT1 interrupts.
	EIMSK = ((1 << INT1) | (1 << INT0));

    // Clear flags.
    EIFR |= ((1 << INTF1)|(1 << INTF0));

    // VERY IMPORTANT - Enable global interrupts.
    sei();


	// Loop - flashes D13 (PB5) every 5 seconds.
	while (1) {
		// Sneaky! Use the PIN register to toggle
        // the output pin.
		PINB |= (1 << PINB5);
		_delay_ms(5000);
	}
}
