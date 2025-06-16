/*============================================================
 * Using SPI on a controller Duemilanove to send a message to 
 * an Uno. The Uno will receive each byte and display the 
 * binary value on a set of 8 LEDs attached to pins D0 through
 * D7.
 *===========================================================*/

// Define CONTROLLER as 1 or 0 to set things up correctly
// for each Arduino board.
#define CONTROLLER 0

// Define USE_SERIAL as 1 to use the SERIAL Monitor, or 0
// to just flash the LEDs.
#define USE_SERIAL 1




#if CONTROLLER
    // A message to be flashed on the peripheral's LEDs.
    const char *message = "The quick brown fox jumps"
                          " over the lazy dog!\n";
#else
    // Store a single character from SPI.
    char receivedByte = 0; 
    bool newByteReceived = false;   
#endif

// Pin definitions. These are not setup in <avr/io.h> and
// its siblings. These are the pins that must be configured
// as only MISO is done automatically when this device is 
// an SPI controller.
#define DDB_SCK    13       // Clock pin.
#define DDB_MISO   12       // Data in.
#define DDB_MOSI   11       // Data out.
#define DDB_SS     10       // Controller's SS pin.

// Short is the delay between characters.
#define THE_DELAY 100


#if CONTROLLER
// The SPI interrupt handler triggers when a byte has been
// transmitted and received. All we have to do here is 
// disconnect the UNO from SPI.
ISR(SPI_STC_vect) {
    // Disable the Uno from SPI.
    digitalWrite(DDB_SS, HIGH);
}
#else
ISR(SPI_STC_vect) {
    // When the peripheral receives a byte, save it.
    receivedByte = SPDR;
    newByteReceived = true;
    
    // D0 to D7 are all output for the LEDs. 
    for (byte p = 0; p < 8; p++) {
          digitalWrite(p, (!!(receivedByte & (1 << p))));
    }
}
#endif

void setupSPI(bool controller) {
    // Disable global interrupts.
    noInterrupts();

    // Enable SPI as a controller;
    // Shift data MSB first;
    // F_CPU / 128 = 125 KHz;
    // SPI interrupts are enabled.
    // Phase and polarity are 0|0 or 1|1
    // as we need to clock data on a rising edge.
      SPCR = (1 << SPE)  | 
             (1 << DORD) |
             (1 << SPR1) | (1 << SPR0) | 
             (0 << CPOL) | (0 << CPHA) |
             (1 << SPIE);
          
      if (controller) {
          SPCR |= (1 << MSTR);
      }      

    // Clear SPI interrupt flag, if set. Volatile to stop
    // compiler optimising it away.
    volatile uint8_t ignoreMe = SPSR;
    ignoreMe = SPDR;

    // Enable global interrupts.
    interrupts();
}


void setup() {
#if CONTROLLER
    // MOSI, SCK and SS need to be output. MISO is 
    // correctly configured already automatically.
    // WARNING: If the SS pin for the Arduino is 
    // not an output pin, then SPI will not work!   
    pinMode(DDB_SCK, OUTPUT);
    pinMode(DDB_MOSI, OUTPUT);
    pinMode(DDB_SS, OUTPUT);

    // Make sure our SS is pulled high. This
    // disconnects the Uno from SPI. 
    digitalWrite(DDB_SS, HIGH);

    // Now configure the Duemilanove as a controller.
    setupSPI(true);
#else
    // MOSI, SCK and SS are automatically configured
    // as inputs. Only MISO needs to be configured as 
    // an output 
    pinMode(DDB_MISO, OUTPUT);
    
    // Set up the Uno as a peripheral.
    setupSPI(false);

#if USE_SERIAL
    // Fire up the Serial Monitor.
    Serial.begin(9600);
#endif

    // D0 to D7 are all output for the LEDs. 
    for (byte p = 0; p < 8; p++) {
        pinMode(p, OUTPUT); 
    }
#endif    
}


void loop() {
#if CONTROLLER
    // Copy some data to SPI. (Need to lose constness!)
    char *p = (char *)message;
       
    while (*p) { 
        // Enable the Uno.
        digitalWrite(DDB_SS, LOW);

        // Copy each byte of the message to SPI.
        SPDR = *p;
        delay(THE_DELAY);

        // Don't forget to increment to the next character!
        p++;

        // The interrupt handler will disconnect between each byte.
    }    
#else
    // The ISR will output the received byte to the LEDs. The loop
    // will print the received data byte to Serial Monitor, if 
    // requested. If a space is received, turn off the LEDS.
    if (newByteReceived) {      
        PRINT(receivedByte);
        if (receivedByte == 0x20) {
            PORTD = 0;
        }
        newByteReceived = false;
    }
#endif
}
