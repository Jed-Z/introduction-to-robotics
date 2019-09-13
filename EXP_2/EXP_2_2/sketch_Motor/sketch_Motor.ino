#include <EEPROM.h>
#include <Servo.h>

const int LEFT_EN = 11;      // 左边电机使能
const int LEFT_SPDCTR = 5;   // 左边电机速度控制
const int RIGHT_EN = 3;      // 右边电机使能
const int RIGHT_SPDCTR = 6;  // 右边电机速度控制

Servo myservo;

void setup() {
    pinMode(LEFT_EN, OUTPUT);
    pinMode(RIGHT_EN, OUTPUT);
    pinMode(LEFT_SPDCTR, OUTPUT);
    pinMode(RIGHT_SPDCTR, OUTPUT);
    digitalWrite(LEFT_EN, LOW);
    analogWrite(LEFT_SPDCTR, 96);
    digitalWrite(RIGHT_EN, LOW);
    analogWrite(RIGHT_SPDCTR, 96);

    myservo.attach(9);
    myservo.write(94);
}

void loop() {
    myservo.write(91);
    delay(100);
    myservo.write(96);
    delay(100);
}
