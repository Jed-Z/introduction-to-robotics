void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    // strengthening
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(i);
            digitalWrite(LED_BUILTIN, LOW);
            delay(10 - i);
        }
    }

    // weakening
    for (int i = 10; i > 0; i--) {
        for (int j = 0; j < 10; j++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(i);
            digitalWrite(LED_BUILTIN, LOW);
            delay(10 - i);
        }
    }
}
