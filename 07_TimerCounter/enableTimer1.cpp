//=============================================================
// A function for Arduino and AVR to enable Timer/counter 1
// in normal mode and with a 256 prescaler and the overflow
// interrupt enabled. The interrupt will trigger every 1.048576
// seconds. 
// 
// NOTE: This will mess up PWM on pins D9 and D10 if used in an
// Arduino sketch, Timer/counter 1 allows analogWrite() on 
// those two pins.
//=============================================================

void enableTimer1() {
    // Choose a timer/counter mode.
    // This overrides the Arduino setup.
    TCCR1A = ((0 << WGM11) | (0 << WGM10));
    TCCR1B = ((0 << WGM13) | (0 << WGM12));
    
    // Clear the interrupt flag.
    TIFR1 = (1 << TOV1);
    
    // Enable the Overflow Interrupt.
    TIMSK1 = (1 << TOIE1);
    
    // Choose a prescaler to start the timer/counter.
    // Divide by 256 is chosen.
    TCCR1B |= ((1 << CS12) | (0 << CS11) | (0 << CS10));
}
