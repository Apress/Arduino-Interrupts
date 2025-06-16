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
    ACSR &= ~((1 << ACIS1) | (1 << ACIS0));

    // Clear interrupt flag.
    ACSR |= (1 << ACI);

    // Enable AC Interrupt.
    ACSR |= (1 << ACIE);
}
