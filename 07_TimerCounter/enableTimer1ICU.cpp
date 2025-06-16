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
