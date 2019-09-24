/*
    红外传感器和巡线传感器通用此代码。
*/
#define RED_PIN 4

int read_int;
void setup() {
    Serial.begin(9600);
    pinMode(RED_PIN, INPUT);
}

void loop() {
    read_int = digitalRead(RED_PIN);
    if (read_int == HIGH) {
        Serial.println("HIGH");
    }
    else if (read_int == LOW) {
        Serial.println("LOW");
    }
    delay(100);
}
