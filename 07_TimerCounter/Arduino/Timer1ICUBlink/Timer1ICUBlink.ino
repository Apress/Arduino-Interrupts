//=============================================================
// A sketch to toggle an LED when an Input Capture Unit (ICU) 
// interrupt is received on Arduino pin D8. The LED will be on 
// D7 and the built in LED on D13 will be flashing on a 5 
// second cycle.
//
// Norman Dunbar
// 27 February 2020.
//=============================================================


//=============================================================
// Toggle D9 every time an event is recorded by the ICU.
//=============================================================
ISR(TIMER1_CAPT_vect) {
    // Toggle pin D9 (PB1) every interrupt.
    PINB |= (1 << PINB1);
}

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

void setup() {
    // Enable input and output pins.
    // D9 and D13 are OUTPUT.
    pinMode(9, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    // D8 is INPUT_PULLUP.
    pinMode(8, INPUT_PULLUP);

    // Enable the ICU and interrupt.
    enableTimer1ICU();   
}


void loop() {
    // The usual flash built in LED every 5 seconds.
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(5000);
}
