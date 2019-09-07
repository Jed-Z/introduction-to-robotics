#include <EEPROM.h>

long read_int = 0;
long brightness = 0;

void myPwm(int pin, unsigned long value, unsigned long duration=1, unsigned long max_val=10);


void setup() {
    // init
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    Serial.setTimeout(50);
    
    brightness = EEPROM.read(0);  // load brightness value from EEPROM
    Serial.println("[+] Brightness is an integer between 0 and 10.");
    Serial.print("Brightness: ");
    Serial.println(brightness, DEC);
}


void loop() {
    while (Serial.available() > 0) {
        read_int = Serial.parseInt();
        if(read_int >= 0 && read_int <= 10) {
            brightness = read_int;
            Serial.print("Brightness: ");
            Serial.println(brightness, DEC);
            EEPROM.write(0, read_int);  // save brightness value to EEPROM
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
        // 0 means only LOW, so there's no need to write HIGH
        if (value > 0) {
            digitalWrite(pin, HIGH);
            delayMicroseconds(value);
        }
        // max_val means only HIGH, so there's no need to write LOW
        if (value < max_val) {
            digitalWrite(pin, LOW);
            delayMicroseconds(max_val - value);
        }
    }
}
