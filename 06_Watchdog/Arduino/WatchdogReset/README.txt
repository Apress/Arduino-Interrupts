ResetInterrupts

The sketch in this folder demonstrates the ability to determine how the Arduino was last (re-)started.

The sketch itself simply lights one or more of  four LEDs to indicate which of the four possible restat manners, occurred. At this stage of the book, the Watchdog, WDT, has now been added to the sketch and will light an LED if indeed, the watchdog reset the board.

Reasons for restarting are:

* Power turned on.
* Reset button pressed.
* Watchdog restarted the system.
* A power brown out caused the system to be reset.

There is a Fritzing project showing the breadboard layout. 

In case you don't have Fritzing, there is a PNG file showing the breadboard layout as well.

