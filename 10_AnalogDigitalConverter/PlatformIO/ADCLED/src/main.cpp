//==========================================================
// An application to use the ADC interrupt to control an LED
// attached to pin D9/PB1 based on the reading from ADC pin
// A0/PC0 itself attached to a potentiometer.
//
// Timer/counter 1 is running in Phase Correct PWM mode 3
// with TOP = 1023 to match the ADC readings.
//
//
// Norman Dunbar.
// 20th April 2020.
//==========================================================


#include "avr/io.h"
#include "avr/interrupt.h"

void setupADC() {
    // Ensure ADC is powered.
    PRR &= ~(1 << PRADC);
    
    // Slow the ADC clock down to 125 KHz
    // by dividing by 128 or 64. 128 is for a 16MHz Arduino
    // 64 for an 8MHz NormDuino. Does not cater for other
    // clock speeds here. BEWARE.
   #if F_CPU == 16000000
        ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
   #else
        // Non-standard 8MHz clock in use.
        ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
   #endif
    

    // Initialise the ADC to use the
    // internal AVCC 5V reference voltage. 
    ADMUX = (0 << REFS1) | (1 << REFS0);

    // Ensure result is right aligned.
    ADMUX &= ~(1 << ADLAR);
    
    // Use the ADC multiplexer input
    // ADC0 = Arduino pin A0.
    ADMUX |= ((0 << MUX3) | (0 << MUX2) | 
              (0 << MUX1) | (0 << MUX0));

    // Disable ADC0 Digital input buffer.
    DIDR0 |= (1 << ADC0D);

    // Use the interrupt to advise when a result is available.
    ADCSRA |= (1 << ADIE);

    // Set auto-trigger on, and choose Free Running mode. As 
    // we are not using the Analogue Comparator, we don't  
    // care about the ACME bit in ADCSRB.
    ADCSRA |= (1 << ADATE);
    ADCSRB = 0;
    
    // Enable the ADC. 
    ADCSRA |= (1 << ADEN);
}


void startADC() {
    ADCSRA |= (1 << ADSC);
}


void setupPWM() {
    // Clears everything to a known state.
    TCCR1A = TCCR1B = TCCR1C = 0;

    // Prescaler of 64.
    TCCR1B = ((1 << CS11) | (1 << CS10));

    // Phase correct PWM with TOP = 1023, sets pin
    // OC1A to toggle.
    TCCR1A = ((1 << WGM11) | (1 << WGM10) | (1 << COM1A1));
}


// The interrupt handler.
ISR(ADC_vect) {
	// OCR1A handles pin D9/PB1.
    OCR1A = ADCW;
}


int main() {
    // PB1 is an Output pin.
    DDRB = (1 << DDB1);

    // We need PWM on PB1.
    setupPWM();

    // Initialise the ADC, using Pin PC0.
    setupADC();
    
    // Now, fire up the ADC.
    startADC();

    // Enable global interrupts.
    sei();

    // The main loop:
    while (1) {
        // Do nothing.
    }
}
