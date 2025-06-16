void setup() {
    Serial.begin(9600);

    // Configure an interrupt on D2 and D3 
    // but don't enable it.
    // 
    // 00 = LOW
    // 01 = CHANGE
    // 10 = FALLING
    // 11 = RISING
    EICRA = ((0 << ISC11) | (1 << ISC10) | 
             (0 << ISC01) | (1 << ISC00)); 

}

void loop() {
  // See if an "interrupt" occurred on D2 or D3. 
  // Pressing a switch should change EICR but not
  // clear it after release.
  Serial.print("EIFR=");
  Serial.println(EIFR, BIN);
  delay(250);
}
