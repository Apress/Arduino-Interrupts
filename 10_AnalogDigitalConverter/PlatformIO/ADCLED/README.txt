ADCLED

This sketch uses the ADC in free running mode to repeatedly read the value from a potentiometer attached to pin PC0. 

Timer1 is configured in Phase Correct PWM mode 3 with TOP = 1023. This matches the values read from the ADC so no adjustment is required, unlike the Arduino sketch.

7The value read from the potentiometer is written directly to PWM pin PB1, which happens to be controller by Timer1. This dims and brightens the LED based on the setting of the potentiometer.

There is a Fritzing project showing the breadboard layout. 

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.


Norman Dunbar
13 February 2020
