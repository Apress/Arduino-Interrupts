// EEPROM_icsp_Init.
//
// A sketch to create an eep file with a single byte, to be 
// uploaded to the EEPROM on an Arduino Uno. The data byte will
// be used to update a pre-initialised variable with a new 
// value.
//
// The code is suitable for an Arduino or PlatformIO sketch
// -- the latter with a suitable "platformio.ini" file. See 
// below.
//
// The before and after values will be displayed on the Serial
// Monitor.
//
// If the after value is 255, then it didn't work (and your 
// EEPROM is blank!).
//
// ===========
// ARDUINO IDE
// ===========
//
// To upload this sketch you will need an ICSP device which 
// will safely upload from the IDE, however, it will not upload
// the EEPROM data in the corresponding "eep" file, only the 
// sketch.
//
// To upload the EEPROM data, in the eep file, you need to 
// compile and upload (using programmer) in verbose mode. This 
// can be set individually in the settings, for both compile 
// and upload but here, only upload requires verbose mode.
//
// Once the sketch is uploaded, look in the log window and 
// scroll back to find the "Global variables use nnnn bytes" 
// line. The line after that one is the one that calls avrdude
// to upload the sketch.
//
// Copy the entire line, and paste it into a notepad/text 
// editor. It will look something like this, all on a single 
// line:
//
// /home/norman/avrdude -C/home/norman/avrdude.conf \
//     -v -patmega328p -carduino -P/dev/ttyUSB0 -b115200 -D \
//     -Uflash:w:/tmp/.../EEPROM_icsp_Init.ino.hex:i 
//
// (The backslash at the end of each line is the Linux 
// continuation character, meaning that the following line is 
// deemed to be part of this line.)
//
// Now, edit the last part of the line from this:
//
// -Uflash:w:/tmp/.../EEPROM_icsp_Init.ino.hex:i 
//   ^^^^^                                 ^^^
//
// to this:
//
// -Ueeprom:w:/tmp/.../EEPROM_icsp_Init.ino.eep:i
//   ^^^^^^                                 ^^^ 
//
// All we are doing is replacing "-Uflash" with "-Ueeprom" and 
// "hex:i" with "eep:i". That is all.
//
// Copy that whole line, and paste into a commandline session. 
// (Windows users, do not worry!) and execute it.
//
// Points to note:
//
// If the option -carduino appears in the command line, you are 
// using the Arduino bootloader and while no errors will be 
// displayed, it will not work. The EEPROM data will not be 
// uploaded as the bootloader can't upload it but returns "no 
// errors" for all eeprom uploads. Ask me how I know!
//
// You must use an ICSP device to program the EEPROM data. It's
// best to "upload using prohgrammer" the sketch as well as 
// you can copy the command for avrdude and modify it to upload
// the EEPROM data as well.
//
// The Arduino IDE defaults to wiping the entire chip, flash
// and EEPROM when using some/all ICSP programmers. This is 
// not good as it means that your EEPROM data is wiped when 
// uploading a sketch, and the sketch is wiped out when 
// uploading EEPROM data. Duh!
//
// You need to find "programmers.txt" and add a "program 
// extra params" for your particular ICSP device. Mine is 
// USBTiny, so I need to add:
//
// usbtiny.program.extra_params= -D
//
// This prevents the sketch from wiping  the EEPROM and vice 
// versa.
//
// ==========
// PlatformIO
// ==========
//
// Use something resembling the folloiwng for the 
// "platformio.ini" file. This will use a USBtiny ICSP device
// as the programmer. Adjust to suit your own device.
//
// [env:uno]
// platform = atmelavr
// board = uno
// framework = arduino
//
// ; Required to use USBtiny ICSP when uploading code or EEPROM
// ; data.
// upload_protocol = usbtiny
//
// To compile from the command line:
//
// Connect your ICSP device and:
//
// pio run
// pio run -t upload
// pio run -t uploadeep
//
// Now connect directly to the Uno, bypassing the ICSP device,
// and:
//
// pio device monitor
//
// You should see the before and after values displayed. The 
// default is 9600 baud, 8 bit, 1 stop, no parity. Adjust as
// desired if you changed the settings in the sketch.
//
// Platformio uploads EEPROM data without wiping the whole
// chip *unless* you specify the -e option to erase the
// whole chip. 
//
// Norman Dunbar
// 13 March 2020.

// PlatformIO needs this for the Arduino framework
// to be used.
#include "Arduino.h"

// Interesting. This gets optimised away if it isn't marked 
// used volatile etc.
uint8_t eepromDataByte __attribute__ ((used)) EEMEM = 77;

uint8_t dataByte = 2;

void setup() {
    Serial.begin(9600);
    
    Serial.print("Before: ");
    Serial.println(dataByte);
    
    dataByte = eeprom_read_byte(&eepromDataByte);
    
    Serial.print("After: ");
    Serial.println(dataByte);
}

void loop() {
// Nothing to see here!
}


