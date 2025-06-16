/*============================================================
 * Using SPI to drive an 74HC595 shift register which is in
 * turn, driving a set of 560R resistors to eight LEDs.
 *
 * Arduino ----------> 74HC595
 * ----------------------------
 * PB0/D8/14       --> 12/RCLK
 * PB1/D9/15       --> 13/~OE (active low)
 * PB3/D11/MOSI/17 --> 14/SER
 * PB5/D13/SCK/19  --> 11/SRCLK  
 * 5V              --> 16/VCC
 * GND             -->  8/GND

 * 74HC595 
 * -------
 *  
 * QA/15  --> 560R --> LED
 * QB/1   --> 560R --> LED
 * QC/2   --> 560R --> LED
 * QD/3   --> 560R --> LED
 * QE/4   --> 560R --> LED
 * QF/5   --> 560R --> LED
 * QG/6   --> 560R --> LED
 *
 * ~OE/13 --> 10K --> VCC
 * ~MR/10 --> 10K --> VCC
 *
 * NONE of the 74HC595 tutorials on t'web mention 
 * these pullups. How do those tutorials work?
 *===========================================================*/

//------------------------------------------------------------
// Pin definitions. These are not setup in <avr/io.h> and
// its siblings. These are the pins that must be configured
// as only MISO is done automatically when this device is 
// an SPI controller.
//------------------------------------------------------------
#define DDB_SCK    13       // Clock pin.
#define DDB_MOSI   11       // Data out.
#define DDB_SS     10       // Controller's SS pin.

//------------------------------------------------------------
// And extras for the 74HC595.
//------------------------------------------------------------
#define HC595_OE   9        // D9/PB1 = 74HC595 output enable.
#define HC595_CS   8        // D8/PB0 = 74HC595's CS pin.


//------------------------------------------------------------
// SHORT_DELAY is the delay between characters.
//------------------------------------------------------------
#define SHORT_DELAY 100

//------------------------------------------------------------
// The SPI interrupt handler triggers when a byte has been
// transmitted and received. All we have to do here is 
// disconnect the 74HC595 from SPI and latch the new data
// onto the output pins to light up the LEDS.
//------------------------------------------------------------
ISR(SPI_STC_vect) {
    // Read the received data here. As we are not expecting
    // any data from the 74HC595, we simply ignore it. We 
    // don't even have to read it, but we do anyway.
    // Volatile to stop compiler optimising it away.
    volatile uint8_t ignoreMe = SPDR;

    // Disable the 74HC595 from SPI.
    digitalWrite(HC595_CS, HIGH);

    // Then latch the new data onto the 74HC595's output pins.
    digitalWrite(HC595_OE, LOW);
}


void setupSPI() {
    // Disable global interrupts.
    noInterrupts();

    // Enable SPI as a controller;
    // Shift data MSB first;
    // F_CPU / 128 = 125 KHz;
    // SPI interrupts are enabled.
    // Phase and polarity are 0|0 or 1|1
    // as we need to clock data on a rising edge.
      SPCR = (1 << SPE)  | (1 << MSTR) |
             (1 << DORD) |
             (1 << SPR1) | (1 << SPR0) | 
             (0 << CPOL) | (0 << CPHA) |
             (1 << SPIE);
          
    // Clear SPI interrupt flag, if set. Volatile to stop
    // compiler optimising it away.
    volatile uint8_t ignoreMe = SPSR;
    ignoreMe = SPDR;

    // Enable global interrupts.
    interrupts();
}


void displayNumber(uint8_t value) {
    // Enable the 74HC595 by pulling its CS pin low...
    digitalWrite(HC595_CS, LOW);

    // while disabling its output latch ~OE by pulling
    // it high. Prevents flicker as data are loaded.
    digitalWrite(HC595_OE, HIGH);

    // Load the value into the data register.
    SPDR = value;
}



void setup() {
    // MOSI, SCK and SS need to be output. MISO is 
    // correctly configured already automatically.
    // WARNING: If the SS pin for the Arduino is 
    // an OUTPUT pin, then SPI will not work!   
    pinMode(DDB_SCK, OUTPUT);
    pinMode(DDB_MOSI, OUTPUT);
    pinMode(DDB_SS, OUTPUT);

    // Make sure our SS is pulled high.
    digitalWrite(DDB_SS, HIGH);

    // Ensure 74HC595 is disconnected from SPI. Pull CS high.
    digitalWrite(HC595_CS, HIGH);

    // Disable 74HC595's output pins before SPI is setup.
    digitalWrite(HC595_OE, HIGH);
    setupSPI();
}


void loop() {
    // Display 0 - 255 with a delay between each digit.
    for (uint8_t i = 0; i <= 255; i++) {
        displayNumber(i);
        delay(SHORT_DELAY);
    }
}
