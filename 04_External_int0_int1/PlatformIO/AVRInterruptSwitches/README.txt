InterruptSwitches

This code uses interrupts, INT0 and INT1, the two available interrupts on an ATmega328P. Two switches are attached to pins D2 and D3 and these are tied to an interrupt routine to turn a (green) LED on and off. PD2 turns on while PD3 turns off the LED.

The main loop spends its time flashing and a really s l o w rate, a red LED on PB5. The LED is flashed at a rate of 5 seconds on then 5 seconds off. While this is ongoing the switches to turn the green LED, on PB0, on and off respond almost immediately - and certainly too quick for a mere human to notice a delay!

There is a Fritzing project showing the breadboard layout. I have used an MC14490P Hex Contact Bounce Eliminator IC to debounce the switches. This works better than using an RC filter or similar, or by doing it in software.

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.
