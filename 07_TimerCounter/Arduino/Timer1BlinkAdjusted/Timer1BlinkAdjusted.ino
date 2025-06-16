//=============================================================
// An Arduino Sketch to flash an LED on pin D8
// uisng Timer/counter 1 in normal mode and with
// a 256 prescaler.
// 
// NOTE: This will mess up PWM on pins D9 and D10.
//
// Norman Dunbar
// 22 February 2020.
//=============================================================

ISR(TIMER1_OVF_vect) {
    // Adjust the flash rate to every second.
    TCNT1 = 3036;

    // Flash D8 every 1.048576 seconds.
    digitalWrite(8, !(digitalRead(8)));
}


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
    TCCR1B |= ((1 << CS12) | (0 << CS11) | (0 << CS10));
}


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(8, OUTPUT);
    enableTimer1();
}


void loop() {
    // Flash D13 every 5 seconds.
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(5000);
}
