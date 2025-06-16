//==========================================================
// A sketch to use the Analog Comparator, connected to the
// Timer/counter1 Input Capture Unit, to light one LED on
// PB0 when the ACO toggles, plus, gets the ICU Interrupt to
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
// The ISR simply sets pin PB0 HIGH or LOW according to the
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

//=============================================================
// Toggle PB1 when a FALLING ACO event is seen by the ICU.
//=============================================================
ISR(TIMER1_CAPT_vect) {
    // Toggle pin D9 (PB1) every interrupt.
    PINB |= (1 << PINB1);
}

//===========================================================
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
    ACSR &= ((1 << ACIS1) | (1 << ACIS0));

    // Clear interrupt flag.
    ACSR |= (1 << ACI);

    // Enable AC Interrupt and ICU input.
    ACSR |= ((1 << ACIE) | (1 << ACIC));
}


//=============================================================
// A function for Arduino and AVR to enable Timer/counter 1 in
// normal mode and with a 1024 prescaler and the input capture
// interrupt enabled. The input to the ICU is the ACO bit from
// the Analog Comparator and not anything from Timer/counter 1.
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

    // Setup:
    setupComparator();
    enableTimer1ICU();
    sei();

	// PB5 = LED_BUILTIN, PB0 = D8, PB1 = D9.
	DDRB = ((1 << DDB5) | (1 << DDB0) | (1 << DDB1));

    // Loop:
    while (1) {
        // Flash the built in LED every 5 seconds.
	    PINB |= (1 << PORTB5);
        _delay_ms(5000);
    }
}
