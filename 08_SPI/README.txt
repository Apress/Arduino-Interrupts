The Fritzing projects and the image files in this parent directory contain the breadboard layouts for all the projects in the Arduino and PlatformIO subdirectories with the same name as the afore mentioned images and Fritzing projects. 

Putting the project and image files here saves duplicating them in each appropriate project for no apparent gain.

Sketches covered here are:

* SPI_LEDs which uses SPI to send data to a Texas Instruments SN74HC595 shift register. Once all 8 bits of data have arrived, the shift register will copy the data onto its output pins, lighting up a set of eight LEDs. The Arduino in the sketch is configured as an SPI controller. The breadboard layout is SPI_LEDs.fzz/SPI_LEDs.png.

* SPI_LEDs_peripheral which uses, in my case, an Arduino Duemilanove as the SPI controller, and an UNO configured as an SPI peripheral device. The UNO will, optionally, copy all data recivced over SPI to the Serail Monitor and also, toe 8 LEDs attached to pins D0-D7. The breadboard layout is SPI_LEDS_peripheral.fzz/SPI_LEDS_peripheral.png.

NOTE: The layout files SPI_NO_LEDS_peripheral.fzz and SPI_NO_LEDS_peripheral.png are supplied for anyone who doesn't have enough LEDs, or can't be bothered (!) to wire them all up! It is the same as SPI_LEDs_peripheral. There isn't a separate sketch for these layout files.

