PollingSwitches

This code does not use interrupts. Instead it polls two switches which are attached to pins D2 and D3 to turn a (green) LED on and off. D2 turns on while D3 turns off the LED.

The main loop spends its time flashing and a really s l o w rate, a red LED. The LED is flashed at a rate of 5 seconds on then 5 seconds off. While this is ongoing the switches to turn the green LED on and off will not respond in a manageable time. The basically need to be held down for at most 10 seconds to get a response.

There is a Fritzing project showing the breadboard layout. I have used an MC14490P Hex Contact Bounce Eliminator IC to debounce the switches. This works better than using an RC filter or similar, or by doing it in software.

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.
