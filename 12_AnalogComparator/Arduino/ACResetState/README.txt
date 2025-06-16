ACResetState

This sketch demonstrates the fact that even without any form
of configuration, the Analog Comparator in an ATmega328P is
enabled to trigger an interrupt when ACO is set when the
voltage of D7 is <= the voltage on D6, or is cleared when
the opposite is true.

Run the sketch, open the serial monitor and play with the pot 
setting to see the ACO bit does indeed toggle even with no
setup whatsoever.

There is a Fritzing project showing the breadboard layout. It is the same one as used for the AnalogComparator.ino sketch.

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.


Norman Dunbar
13 February 2020
