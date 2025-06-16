InterruptDisabled

This code shows how the flag bits to signal that an interrupt stimulus was received, will be set even though the particular interrupt was never enabled. The example uses INT0 and INT1 and configures them to watch for a CHANGE (going from HIGH to LOW or LOW to HIGH) on pins D2 and D3 will set the EIFR register's flags for INT0 and INT1 interrupts.

There is a Fritzing project showing the breadboard layout. I have used an MC14490P Hex Contact Bounce Eliminator IC to debounce the switches. This works better than using an RC filter or similar, or by doing it in software.

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.
