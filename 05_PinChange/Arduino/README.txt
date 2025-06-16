PinChangeSwitch

This sketch uses a pin change interrupt on D2 to turn an LED (green) on and off when the switch attached to that pin is pressed.

The main loop will be flashing a red LED at 5 second intervals.

The switch(es) are debounced with an MC14490C hex switch debouncer.

This code demonstrates the pin change interrupt library by Nico Hood. The switch is processed "immediately" and doesn't have to wait for the long delay() to finish - as 
they would if polled.

There is a Fritzing project showing the breadboard layout. 

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.


Norman Dunbar
13 February 2020
