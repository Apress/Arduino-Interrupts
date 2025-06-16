// Using pin change interrupt on D2 to turn an LED (green) on
// and off when the corresponding switch is pressed.
//
// The main loop will be running "work" code with long delays
// - flashing a red LED at 5 second intervals.
//
// This code demonstrates the pin change interrupt library
// by Nico Hood. The switche is processed "immediately" and 
// doesn't have to wait for the long delay() to finish - as 
// they would if polled.
//
// Norman Dunbar
// 13 February 2020

// We need this to use the PinChangeInterrupt library.
#include "PinChangeInterrupt.h"


// D13 is flashed for 5 seconds by loop().
const byte ledFlash = 13;

// D8 is controlled by interrupts. D2 = on/off.
const byte ledOnOff = 8;

// D2 is the switch pin.
const byte onOffPin  = 2;



void setup() {
    // LED pins are obviously outputs.
    pinMode(ledFlash, OUTPUT);
    pinMode(ledOnOff, OUTPUT);

    // D2 is inputs with pullup (to save me using
    // external resistors.)
    pinMode(onOffPin, INPUT_PULLUP);

    // D2 has an ISR to turn the LED on and off.
    // LOW:     Does not work. 
    // CHANGE:  LED toggles when switch pressed and released.
    // FALLING: LED latches on or off when switch pressed.
    // RISING:  LED latches on or off when switch released.
    //
    attachPCINT(digitalPinToPCINT(onOffPin), toggleLED, CHANGE);
}


void loop() {
    digitalWrite(ledFlash, HIGH);
    delay(5000);
    digitalWrite(ledFlash, LOW);
    delay(5000);    
} 


void toggleLED() {
    digitalWrite(ledOnOff, !digitalRead(ledOnOff));
}
