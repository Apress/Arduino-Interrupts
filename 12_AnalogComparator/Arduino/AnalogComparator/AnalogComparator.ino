//===========================================================
// A sketch to use the Analog comparator interrupt to light 
// an LED on pin D8 when the ACO bit toggles. The main loop
// spends all its time flashing the built in LED on a 5 
// second delay.
//
// Norman Dunbar.
// 17th February 2020.
//===========================================================



// ==========================================================
// The ISR simply sets pin D8 HIGH or LOW according to the
// ACO bit in the ACSR register. The !! is sneaky as it will
// return 1 if the bit is 1 and 0 if the bit is 0. 1 and 0 
// correspond to HIGH and LOW so the LED takes on the correct
// state.
//===========================================================
ISR(ANALOG_COMP_vect) {
    // Read the ACO bit and if on, light the LED on D8.
    // otherwise, extinguish it.
    digitalWrite(8, !!(ACSR & (1 << ACO)));
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
    ACSR = 0;
    
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

    // Enable AC Interrupt.
    ACSR |= (1 << ACIE);
}


void setup() {
    setupComparator();
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(8, OUTPUT);
}


void loop() {
    // Flash the built in LED every 5 seconds.
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(5000);
}
