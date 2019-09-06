long read_int = 0;
long brightness = 0;

void myPwm(int pin, unsigned long value, unsigned long duration=1, unsigned long max_val=10);


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    Serial.setTimeout(50);
    Serial.println("[+] Brightness is an integer between 0 and 10.");
}


void loop() {
    while (Serial.available() > 0) {
        read_int = Serial.parseInt();
        if(read_int >= 0 && read_int <= 10) {
            brightness = read_int;
            Serial.print("Brightness: ");
            Serial.println(brightness, DEC);
        }
        else {
            Serial.println("[-] Invalid value!");
        }
    }
    myPwm(13, brightness, 100);
}


/* A simple PWM function implemented by Group 10.
 *  Parameters:
 *    pin: the Arduino pin to write to
 *    value: number of time units with a HIGH value
 *    duration: milliseconds that PWN signal lasts
 *    max_val: total number of time units in a cycle
 */
void myPwm(int pin, unsigned long value, unsigned long duration, unsigned long max_val) {
    unsigned long end_time = millis() + duration;
    while(millis() < end_time) {
        digitalWrite(pin, HIGH);
        delayMicroseconds(value);
        digitalWrite(pin, LOW);
        delayMicroseconds(max_val - value);
    }
}
