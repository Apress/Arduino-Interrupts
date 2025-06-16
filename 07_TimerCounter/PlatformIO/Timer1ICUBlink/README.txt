Timer1ICUBlink

This sketch uses Timer1's Input Capture Unit's interrupt to toggle an LEDs on pin PB1 wherever a button is pressed. The button is connected to the ICU and the interrupt simply toggles the LED when an "event" arrives. The event in this case, being the button press on pin PB0/ICP1 which is the ICU's input pin.

The built in LED flashes every 5 seconds.

There is a Fritzing project showing the breadboard layout. 

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.

