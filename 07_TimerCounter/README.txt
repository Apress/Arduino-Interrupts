The Fritzing projects and the image files in this parent directory contain the breadboard layouts for all the projects in the Arduino and PlatformIO subdirectories with the same name as the afore mentioned images and Fritzing projects. 

Putting the project and image files here saves duplicating them in each appropriate project for no apparent gain.

Sketches covered here are:

* Timer1Blink which uses Timer1's overflow intrerrupt to blink an LED. Also used for Timer1BlinkAdjusted and Timer1BlinkAdjustedAgain. All of those sketches use the same breadboard layout. (Timer1Blink.fzz/Timer1Blink.png)

* Timer1CompareMatch which uses Timer1's compare match A and B interrupts to toggle a pair of LEDs. The breadboard layout is Timer1CompareMatch.fzz/Timer1CompareMatch.png.

* Timer1ICUBlink which uses Timer1's Input Capture Event interrupt to toggle an additional LED when an event is captured on pin D8/PB0. The layout files are Timer1ICUBlink.fzz and Timer1ICUBlink.png.


