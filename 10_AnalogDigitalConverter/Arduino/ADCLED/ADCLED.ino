//=========================================================
// Small sketch using the ADC interrupt to control an LED
// on pin D9 (PWM) based on the reading from the ADC on pin
// A0. The ADC is started in free running mode so always
// starts a new conversion when the current one is done.
//
// Norman Dunbar
// 20 April 2020
//=========================================================

void setupADC() {
    // Ensure ADC is powered.
    PRR &= ~(1 << PRADC);
    
    // Slow the ADC clock down to 125 KHz
    // by dividing by 128 or 64. 128 is for a 16 MHz Arduino
    // 64 for an 8 MHz NormDuino. Does not cater for other
    // clock speeds here. BEWARE.
   #if F_CPU == 16000000
        ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
   #else
        // Assume 8MHz clock.
        ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
   #endif
    

    // Initialise the ADC to use the
    // internal AVCC 5V reference voltage. 
    ADMUX = (0 << REFS1) | (1 << REFS0);

    // Ensure result is right aligned.
    ADMUX &= ~(1 << ADLAR);
    
    // Use the ADC multiplexer input
    // ADC0 = Arduino pin A0.
    ADMUX |= (0 << MUX3) | (0 << MUX2) | 
             (0 << MUX1) | (0 << MUX0);

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


// Global storage for ADC result.
volatile uint16_t ADCReading = 0;


// The interrupt handler.
ISR(ADC_vect) {
    ADCReading = ADCW;
}

void setup() {
    // D9 has PWM, use it for the LED.
    pinMode(9, OUTPUT);

    // A0 is the input pin for the ADC. 
    setupADC();

    // Start the first ADC conversion.
    startADC();
}

void loop() { 
    // Dim or brighten the LED.
    analogWrite(9, map(ADCReading, 0, 1023, 0, 255));
}
