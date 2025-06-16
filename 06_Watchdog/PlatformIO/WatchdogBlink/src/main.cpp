//=======================================================
// A "sketch" to configure the Watchdog Timer Interrupt  
// to flash the build in LED every second. 
//
// Norman Dunbar
// 13th February 2020
//=======================================================

#include "avr/io.h"
#include "avr/interrupt.h"
#include "avr/wdt.h"

void WDT_Interrupt(uint8_t timeout) {
    uint8_t oldSREG = SREG;

	uint8_t wdt_setting;
	timeout = (timeout > 9) ? 9 : timeout;
	wdt_setting = (timeout > 7) ? (1 << WDP3) : 0;
	wdt_setting |= (timeout & 7);

	cli();
	wdt_reset();


	MCUSR &= ~(1 << WDRF);

	WDTCSR |= ((1 << WDCE) | (1 << WDE));
	WDTCSR = (wdt_setting) | (1 << WDIE);

	SREG = oldSREG;
}


int main() {
    // setup.
    DDRB |= (1 << DDB5);
    WDT_Interrupt(WDTO_1S);

    // This is needed here because the
    // AVR code doesn't do it for you
    // unlike the Arduino.
    sei();

    // Loop. (Does nothing.)
    while (true) ;
}

ISR(WDT_vect) {
    PINB |= (1 << PINB5);
}
