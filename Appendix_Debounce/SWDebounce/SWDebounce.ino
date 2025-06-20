// The number of the pushbutton pin
const int buttonPin = 2;

// The number of the LED pin
const int ledPin = 13;

// The current state of the output pin
int ledState = HIGH;

// The current reading from the input pin    
int buttonState;

// The previous reading from the input pin
int lastButtonState = LOW;

// The last time the output pin was toggled
unsigned long lastDebounceTime = 0;

// The debounce time; increase if the output flickers
unsigned long debounceDelay = 50;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  // Read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // Check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // Reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Whatever the reading is at, it's been there for 
    // longer than the debouncedelay, so take it as
    // the actual current state.

    // If the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // Only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  // Set the LED:
  digitalWrite(ledPin, ledState);

  // Save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
