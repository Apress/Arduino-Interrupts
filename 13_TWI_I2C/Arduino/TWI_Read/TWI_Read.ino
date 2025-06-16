//------------------------------------------------------------
// A sketch to use direct register access to read data from 
// an LM75A temperature sensor.
//
// No interrupts are used. Polling only.
//
// The Arduino is running in "controller-receiver" mode.
//
// Norman Dunbar
// 14th August 2020
//------------------------------------------------------------

#include "twi_defines.h"

#define LM75A_ADDRESS 0x4F

// Default prescaler = 1. Atmel says leave alone!
#define PSCALER 1

// Calculate TWBR from desired SCL frequency.
#define SCL_HZ_TO_TWBR(F) (0.5 * PSCALER) * ((F_CPU/(F)) - 16)


void setup() {
    Serial.begin(9600);
    Serial.println("LM75A - Temperature Measurement\n");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // VERY VERY IMPORTANT!
    // Set  the SCL clock speed to 200 KHz.
    TWBR = SCL_HZ_TO_TWBR(200000);
}


void waitForStatus(byte requiredStatus) {

    // Wait for the TWI to respond.
    while (!(TWCR & (1 << TWINT)));

    // Grab & print the status byte.
    byte crxStatus = CRX_STATUS;
    //Serial.println(crxStatus, HEX);

    // Did we expect that status?
    if (crxStatus != requiredStatus) {
        Serial.print("Invalid status, expected 0x");
        Serial.print(requiredStatus, HEX);
        Serial.print(", received 0x");
        Serial.println(crxStatus, HEX);

        
        // Light the LED on errors.
        digitalWrite(LED_BUILTIN, HIGH);

        // Try to stop TWI communications.
        //Serial.print("Sending STOP....Status = 0x");
        TWCR = CRX_STOP;
        
        // Loop the loop forever.
        while (1) ;
    }
}


void loop() {
    byte tempData[2];
    
    // Send start condition.
    //Serial.print("Sending START....Status = 0x");
    TWCR = CRX_START;
    waitForStatus(CRX_START_SENT);

    // Send LM75A read address.
    //Serial.print("Sending LM75A Address = 0x"); 
    TWDR = (((LM75A_ADDRESS) << 1) | 1);
    //Serial.print(TWDR, HEX);
    //Serial.print("...Status = 0x");
    TWCR = CRX_SEND_READ_ADDRESS;
    waitForStatus(CRX_SLAR_ACK_RCVD);

    // Read two bytes, first with ack, second with nack.
    //Serial.print("Reading byte 1....Status = 0x");
    TWCR = CRX_READ_ACK;
    waitForStatus(CRX_DATA_ACK_SENT);
    tempData[0] = TWDR;
    
    //Serial.print("Reading byte 2....Status = 0x");
    TWCR = CRX_READ_NACK;
    waitForStatus(CRX_DATA_NACK_SENT);
    tempData[1] = TWDR;

    //Serial.print("Sending STOP....Status = 0x");
    TWCR = CRX_STOP;


    // Did we get a temperature?
    Serial.print(" ");
    Serial.print(tempData[0]);
    Serial.println((tempData[1] & 0x80) ? ".5 C" : ".0 C");

    delay(2500);
}
