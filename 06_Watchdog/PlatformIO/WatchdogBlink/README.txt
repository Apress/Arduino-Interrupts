WatchdogBlink

This sketch uses the watchdog timer interrupt to flash the built in LED every second. The code is plain AVR C++ and not using the Arduino Language.

The main loop does nothing. After setup has initialised the watchdog, the WDT interrupt will fire every second, and the ISR will toggle the LED.

