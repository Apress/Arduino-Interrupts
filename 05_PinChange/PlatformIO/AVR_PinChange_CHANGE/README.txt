AVR_PinChange_CHANGE

This sketch uses a pin change interrupt on PD2 to turn an LED (green) on and off when the switch attached to that pin is pressed. Another switch is attached to PD3, but is notused in this sketch. 

The main loop will be flashing a red LED, on pin PB5, at 5 second intervals.

The switch(es) are debounced with an MC14490C hex switch debouncer.

This code demonstrates the pin change interrupt features of the ATmega328P. The switch is processed "immediately" and doesn't have to wait for the long delay() to finish - as 
they would if polled.

The interrupt is configurted to look for a CHANGE stimulus on pin PD2 in order to be triggered.



There is a Fritzing project showing the breadboard layout. I have used an MC14490P Hex Contact Bounce Eliminator IC to debounce the switches. This works better than using an RC filter or similar, or by doing it in software.

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.


Norman Dunbar
13 February 2020
