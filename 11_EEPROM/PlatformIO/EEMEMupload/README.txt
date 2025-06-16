EEMEMupload:

An AVR application to initialise EEPROM data with a single byte, the number of times the builtin LED should be flashed each time in the loop. Thei is carried out using the EEMEM attribute within the source code.

To upload this sketch you will need an ICSP device. This will safely upload from the PlatformIO IDE (Usually VSCode), however, it will not upload the EEPROM data in the corresponding "eep" file, that requires a separate upload also available from within the IDE.

Or, on the command line:

pio run -t upload
pio run -t uploadeep


