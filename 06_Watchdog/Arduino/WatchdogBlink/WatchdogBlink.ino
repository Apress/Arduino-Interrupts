//=======================================================
// A sketch to configure the Watchdog Timer Interrupt to 
// flash the build in LED every second. 
//
// Norman Dunbar
// 13th February 2020
//=======================================================

#include "avr/wdt.h"

void WDT_Interrupt(uint8_t timeout) {
    // Save global interrupt flag
    uint8_t oldSREG = SREG;

    // Set the prescaler bits.
    uint8_t wdt_setting;
    timeout = (timeout > 9) ? 9 : timeout;
    wdt_setting = (timeout > 7) ? (1 << WDP3) : 0;
    wdt_setting |= (timeout & 7);

    // Disable Interrupts.
    cli();

    // Reset WDT.
    wdt_reset();

    // Clear RESET by WDT flag.
    MCUSR &= ~(1 << WDRF);

    // Do the timed sequence.
    WDTCSR |= ((1 << WDCE) | (1 << WDE));

    // Enable interrupts but not the reset.
    WDTCSR = (wdt_setting) | (1 << WDIE);

    // REstore global interrupt flag.
    SREG = oldSREG;
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    WDT_Interrupt(WDTO_1S);
}


void loop() {
    while (true) ;
}

ISR(WDT_vect) {
    digitalWrite(13, !digitalRead(13));
}
