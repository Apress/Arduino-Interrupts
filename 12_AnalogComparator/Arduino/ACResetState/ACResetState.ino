//============================================================
// This code demonstrates the fact that even without any form
// of configuration, the Analog Comparator in an ATmega328P is
// enabled to trigger an interrupt when ACO is set when the
// voltage of D7 is <= the voltage on D6, or is cleared when
// the opposite is true.
//
// It simply reads the ACO bit of the ACSR register every time
// through the loop() to see if the AC interrupt bit is set or
// otherwise, and reports back to the serial monitor.
//
// Connect D6 to the Arduino's 3v3 output;
// Connect a 10k (or similar) potentiometer to VCC and GND with
// the middle pin connected to D7.
//
// Run the sketch, open the serial monitor and play with the pot 
// setting to see the ACO bit does indeed toggle even with no
// setup whatsoever.
//
// Norman Dunbar
// 20 February 2020
//============================================================



void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.print("ACO = ");
    Serial.println(!!(ACSR & (1 << ACO)));

    // Clear the interrupt flag.
    ACSR |= (1 << ACI);}
