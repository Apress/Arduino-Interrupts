/*==================================================== 
 * Debounce a switch, passed as a pin number. 
 * --------------------------------------------------- 
 * Based on code by Warren Gay in his book on FreeRTOS
 * published by Elektor.
 * --------------------------------------------------- 
 * 
 * Norman Dunbar 
 * 16 October 2021.
 *====================================================
 */

/*----------------------------------------------------
 * Switch on D2. Results to Serial. Switch is pulled 
 * down with a 10K resistor as per the following: 
 * --------------------------------------------------- 
 *         5 V
 *         ---
 *          ^
 *          |   
 *          +-->o-----o Switch
 *               | O |
 *          +-->o-----o<--------- Arduino Pin D2
 *          |
 *          V
 *         10 K
 *          |
 *          V
 *         --- 
 *         GND
 * --------------------------------------------------- 
 */

// Which pin has the switch attached?
#define SWITCH_PIN 2

// Masks for the pressed/released bit patterns
#define ALL_ONES 0xFFFFFFFF
#define ALL_ZEROS 0x00000000

// Debugging stuff. Set DEBUG to 1 if you 
// need to see debugging info in the Serial
// Monitor. Set to zero to turn it off.
#define DEBUG 0

#if(DEBUG == 1)
  #define DEBUG_PRINT   Serial.print
  #define DEBUG_PRINTLN Serial.println
#else
  #define DEBUG_PRINT
  #define DEBUG_PRINTLN
#endif


/*----------------------------------------------------
 * Debounce returns TRUE if the switch is pressed or
 * FALSE if released. 
 *----------------------------------------------------*/
bool debounce(uint8_t pinNumber, bool pullUpEnabled) {
    uint8_t switchState = 0;      // Current state
    uint32_t stateHistory = 0;    // 32 State history

    // Loop around until we read the same value from
    // the switch, 32 times in succession.
    while (1) {
        // Get 0 or 1 from the switch.
        switchState = !!(digitalRead(pinNumber));

        // Add it to the stateHistory.
        stateHistory = (stateHistory << 1) | switchState;

        // If we have read 32 consecutive high or low
        // values from the pin, we are done.
        if (stateHistory == ALL_ONES || stateHistory == ALL_ZEROS)
            // Return true if switch is pressed, false if released.
            return ((stateHistory == ALL_ONES) != pullUpEnabled);
    }
}



void setup() {
    pinMode(SWITCH_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(9600);
    Serial.println("Press the switch ...");
}


void loop() {
    // The previous state of the switch when last read.
    static bool switchPreviousState = false;

    // Current state of the LED, true = on.
    static bool ledState = false;


    // Debounce the switch on selected pin. Flag says
    // switch is pulled down by an external resistor.
    //
    // Change this if you have an INPUT_PULLUP pin or
    // have the switch pulled up by an external resistor.
    bool switchState = debounce(SWITCH_PIN, false);
        
    // Print details only if we have a change of state.
    if (switchState != switchPreviousState) {
        switchPreviousState = switchState;
        DEBUG_PRINT("Switch state: ");
        DEBUG_PRINT(switchState);
        
        // And toggle the LED if switch pressed.
        if (switchState) {
            ledState = !ledState;
            digitalWrite(LED_BUILTIN, ledState);
        }

        // Report LED state, if debugging.
        DEBUG_PRINT(" LED: ");
        DEBUG_PRINTLN(ledState ? "ON" : "OFF");
    }
}
