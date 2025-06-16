Timer1BlinkAdjustedAgain

This sketch uses Timer1's overflow interrupt to toggle an LED on pin D8 every second. This differs from Timer1BlinkAdjusted as the ISR now uses AVR C++ to toggle the LED rather than Arduino's "digitalWrite" because those calls take far too long and cause the calculated adjustment to still give a wrong flash frequency.

There is a Fritzing project showing the breadboard layout. 

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.

