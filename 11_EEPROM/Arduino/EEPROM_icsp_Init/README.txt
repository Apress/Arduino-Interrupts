EEPROM_icsp_init: 

Shows how to initialise data in the EEPROM using an ICSP
device and uploading the "eep" file using avrdude. This sketch uses the
EEMEM attribute to force EEPROM data into an "EEP" file at compile time.

The Arduino IDE cannot upload EEPROM data using a bootloader or an ICSP. It must be uploaded manually using avrdude.


