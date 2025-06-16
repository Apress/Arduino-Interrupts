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

void enableTimer1CompMatch(uint16_t ocr1a, uint16t ocr1b) {
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