Timer1AnalogComparatorICU

This sketch uses the Analog Comparator, connected to the
Timer/counter1 Input Capture Unit, to light one LED on
D8 when the ACO toggles, plus, gets the ICU Interrupt to
turn on another LED on D9 when it sees a FALLING edge
from the ACO bit.

The main loop will be flashing a red LED at 5 second intervals.

There is a Fritzing project showing the breadboard layout. 

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.

