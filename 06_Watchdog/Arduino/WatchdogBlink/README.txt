WatchdogBlink

This sketch uses the watchdog timer interrupt to flash the built in LED every second.

The main loop does nothing. After setup has initialised the watchdog, the WDT interrupt will fire every second, and the ISR will toggle the LED.


There is NOT a Fritzing project showing the breadboard layout as the builtin LED is all that is required. There is not an image either, it is not needed for this sketch.

