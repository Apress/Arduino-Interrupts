USARTSketch

This sketch shows how to use interrupts, in a manner similar to how it is done in the Arduino Language, to send and receive data to and from the USART when writing plain AVR C++ sketches.

This sketch, by necessity, also uses a circular buffer, as does the Arduino, to buffer data received but not yet read by the sketch, and data transmitted but not yet sent out by the USART hardware.

The printf() function is also used in this example.

There are no Fritzing project files as the sketch only uses the Serial Monitor.
