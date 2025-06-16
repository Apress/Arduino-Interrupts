// Using interrupts on D2 and D3 to turn an LED (green) on
// (D2) and off (D3) when the corresponding switch is pressed.
// The main loop will be running "work" code with long delays
// - flashing a red LED at 5 second intervals.
//
// This code demonstrates the external interrupts with the 
// Arduino. The switches are processed "immediately" and 
// don't have to wait for the long delay() to finish - as 
// they would if polled.
//
// Norman Dunbar
// 22 January 2020


// D13 is flashed for 5 seconds by loop().
const byte ledFlash = 13;

// D8 is controlled by interrupts. D2 = on, D3 = off.
const byte ledOnOff = 8;

// D2 and D3 are switch pins.
const byte onPin  = 2;
const byte offPin = 3;


void setup() {
    // LED pins are obviously outputs.
    pinMode(ledFlash, OUTPUT);
    pinMode(ledOnOff, OUTPUT);

    // D2 and D3 are inputs with pullup (to save me using
    // external resistors.)
    pinMode(onPin, INPUT_PULLUP);
    pinMode(offPin, INPUT_PULLUP);

    // D2 and D3 have an ISR each to turn the LED on and off.
    attachInterrupt(digitalPinToInterrupt(onPin), 
                    ledOn, FALLING);

    attachInterrupt(digitalPinToInterrupt(offPin), 
                    ledOff, FALLING);
}


void loop() {
    digitalWrite(ledFlash, HIGH);
    delay(5000);
    digitalWrite(ledFlash, LOW);
    delay(5000);
    
} 


void ledOn() {
    digitalWrite(ledOnOff, HIGH);
}


void ledOff() {
    digitalWrite(ledOnOff, LOW);
}
