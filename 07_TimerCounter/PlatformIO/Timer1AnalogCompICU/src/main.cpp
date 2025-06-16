//==========================================================
// A sketch to use the Analog Comparator, connected to the
// Timer/counter1 Input Capture Unit, to light one LED on
// D8 when the ACO toggles, plus, gets the ICU Interrupt to
// turn on another LED on D9 when it sees a FALLING edge 
// from the ACO bit.
// The main loop spends all its time flashing the built in 
// LED on a 5 second delay.
//
// Norman Dunbar.
// 27th February 2020.
//==========================================================

#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"


// ==========================================================
// The ISR simply sets pin D8 HIGH or LOW according to the
// ACO bit in the ACSR register.
//===========================================================
ISR(ANALOG_COMP_vect) {
    // Read the ACO bit and if on, light the LED on PB0.
    // otherwise, extinguish it.
    if (ACSR & (1 << ACO)) {
        PORTB |= (1 << PORTB0);
    } else {
        PORTB &= ~(1 << PORTB0);
    }    
}


//===========================================================
// This function initialises the analog comparator with pin
// AIN0/D6 as the reference voltage and AIN1/D7 (-ve input)
// as the voltage to be compared with AIN0/D6 (+ve input). 
//
// When AIN0/D6 is higher than AIN1/D7 then the ACO bit 
// will be set and this will trigger the interrupt.
//
// This function sets up the comparator to fire an interrupt
// each time the ACO bit toggles. It doesn't consider the
// use of the internal bandgap voltage, or any of the ADC 
// input pins either.
//===========================================================

void setupComparator() {

    // Disable AC interrupts.
    ACSR &= ~(1 << ACIE);

    // Enable AC by disabling the AC Disable bit!
    ACSR &= ~(1 << ACD);

    // Disable digital I/O on D6 and D7.
    DIDR1 |= ((1 << AIN0D) | (1 << AIN1D));

    // D6 will be the reference voltage.
    ACSR &= ~(1 << ACBG);

    // D7 to compare with D6.
    ADCSRB &= ~(1 << ACME);

    // Fire AC interrupt on ACO toggle.
    ACSR |= ((0 << ACIS1) | (0 << ACIS0));

    // Clear interrupt flag.
    ACSR |= (1 << ACI);

    // Enable AC Interrupt.
    ACSR |= (1 << ACIE);
}


int main() {

    // Setup:
    setupComparator();
    sei();

	// PB5 = LED_BUILTIN, PB0 = D8.
	DDRB = ((1 << DDB5) | (1 << DDB0));

    // Loop:
    while (1) {
        // Flash the built in LED every 5 seconds.
	    PINB |= (1 << PORTB5);
        _delay_ms(5000);
    }
}
