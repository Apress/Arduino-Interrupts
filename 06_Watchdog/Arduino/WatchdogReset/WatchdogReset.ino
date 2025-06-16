//=======================================================
// Test the MCUSR register after a reset to determine the
// reason the reset occurred. This version forces a WDT
// reset due to a timeout - just to be sure we can light that 
// last LED!
//
// There are 4 potential reset causes:
// 
// 1. Power on Reset;
// 2. RESET button pressed, RESET pin grounded;
// 3. Brown Out Detected reset;
// 4. Watchdog Timer reset.
//
// Norman Dunbar
// 13th February 2020
//=======================================================

#define POWER_ON_RESET(x) ((x) & (1 << PORF))
#define POWER_ON_RESET_PIN 3

#define EXTERNAL_RESET(x) ((x) & (1 << EXTRF))
#define EXTERNAL_RESET_PIN 4

#define WATCHDOG_RESET(x) ((x) & (1 << WDRF))
#define WATCHDOG_RESET_PIN 5

#define BOD_RESET(x)      ((x) & (1 << BORF))
#define BOD_RESET_PIN      6

#include "avr/wdt.h"


void lightLED(uint8_t led) {
        digitalWrite(led, HIGH);
}


void setup() {
    // Get current MCUSR & reset it.
    uint8_t mcusr = MCUSR;
    MCUSR = 0;

    wdt_reset();
    wdt_enable(WDTO_8S);


    pinMode(POWER_ON_RESET_PIN, OUTPUT);
    pinMode(EXTERNAL_RESET_PIN, OUTPUT);
    pinMode(WATCHDOG_RESET_PIN, OUTPUT);
    pinMode(BOD_RESET_PIN, OUTPUT);

    Serial.begin(9600);
    delay(1000);

    Serial.print("MCUSR = 0b");
    Serial.println(mcusr, BIN);

    if (POWER_ON_RESET(mcusr)) {
        Serial.println("Power on reset detected.");
        lightLED(POWER_ON_RESET_PIN);
    }

    if (EXTERNAL_RESET(mcusr)) {
        Serial.println("External reset detected.");
        lightLED(EXTERNAL_RESET_PIN);
    }

    if (WATCHDOG_RESET(mcusr)) {
        wdt_disable();
        Serial.println("Watchdog reset detected.");
        lightLED(WATCHDOG_RESET_PIN);
    }

    if (BOD_RESET(mcusr)) {
        Serial.println("Brown out reset detected.");
        lightLED(BOD_RESET_PIN);
    }

    // This will trigger the WDT to reset the board.
    delay(10000);
}

void loop() {
  // Not much going on here...
}
