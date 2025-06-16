Timer1BlinkAdjusted

This sketch uses Timer1's overflow interrupt to toggle an LED on pin D8 every second. This differs from Timer1Blink as there is an adjustment in the ISR to get the blink happening every second, rather than just "roughly" every second. Unfortunately, for reasons which will become clear in Timer1BlickAdjustedAgain, the delay is still not accurate!

There is a Fritzing project showing the breadboard layout. 

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.

