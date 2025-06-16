// Using polling on D2 and D3 to turn an LED (green) on (D2)
// and off (D3) when the corresponding switch is pressed. The
// main loop will be running "work" code with long delays - 
// flashing a red LED at 5 second intervals.
//
// This code shows how polling a switch, when long running 
// "work" is in progress leads to long delays in responding.
// In order to get a switch to respond, you have to hold it
// down until the red LED is finished its 5 second delays.
// Not very good.
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
    // LED pins are obvioulsy outputs.
    pinMode(ledFlash, OUTPUT);
    pinMode(ledOnOff, OUTPUT);

    // D2 and D3 are inputs with pullup (to save me using
    // external resistors.)
    pinMode(onPin, INPUT_PULLUP);
    pinMode(offPin, INPUT_PULLUP);
}


// loop() is doing "work" which may run for some time. As we
// are polling the switches after "working" (ie, flashing the
// LED) then there will be a massive delay in responding to
// the switches if the "work" lasted any length of time.
void loop() {
    digitalWrite(ledFlash, HIGH);
    delay(5000);

    // Poll switches. The code is in the middle but it
    // makes no difference as there will always be a
    // 10 second loop time, no matter where I place the
    // code to poll.
    if (digitalRead(onPin) == HIGH)
        digitalWrite(ledOnOff, HIGH);
        
    if (digitalRead(offPin) == HIGH)
        digitalWrite(ledOnOff, LOW);
        

    digitalWrite(ledFlash, LOW);
    delay(5000);
    
} 
