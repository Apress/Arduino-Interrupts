The sketches in the Arduino folder are:

EEPROM_icsp_init: 

Shows how to initialise data in the EEPROM using an ICSP
device and uploading the "eep" file using avrdude. This sketch uses the
EEMEM attribute to force EEPROM data into an "EEP" file at compile time.

EEPROMinit: 

Shows a sketch which simply writes the required data to the 
EEPROM. The loop() function here reads the data back and displays it, but 
that just shows that the data were written correctly. This sketch is 
intended to be used once to initialise data in the EEPROM, then further 
sketches can be uploaded to read and use the EEPROM data.

EEPROMinterrupt: 

Shows an Arduino sketch that reads data from the EEPROM, under interrupt control and displays the data on the Serial Monitor. (Itself under interrupt control as well!)

EEPROMpolling: 

Shows an Arduino sketch which reads and writes to the EEPROM using the AVRlib functions. Does not use interrupts.


In the PlatformIO folder, we have:

EEMEMupload:

An AVR application to initialise EEPROM data with a single byte, the number of times the builtin LED should be flashed each time in the loop.

To upload this sketch you will need an ICSP device. That will safely upload from the IDE, however, it will not upload the EEPROM data in the corresponding "eep" file.

EEPROMinterrupt: 

An AVR application to read and write to/from the EEPROM under interrupt control. The data read from the EEPROM is written to the USART which is itself under interrupt control - two sets of interrupts for the price of one! This is a conversion of the Arduino EEPROMinterrupt sketch discussed above.


