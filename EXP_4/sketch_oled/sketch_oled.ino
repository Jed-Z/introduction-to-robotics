#include "define.h"
#include <Servo.h>

SSD1306 oled(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, 0);
int read_int;
double voltage;

const int LEFT_EN = 11;      // 左边电机使能
const int LEFT_SPDCTR = 5;   // 左边电机速度控制
const int RIGHT_EN = 3;      // 右边电机使能
const int RIGHT_SPDCTR = 6;  // 右边电机速度控制

Servo myservo;
int left_speed = 96;
int right_speed = 96;
int servo_degree = 94;

void setup()
{
    pinMode(A0, INPUT);    
    oled.ssd1306_init(SSD1306_SWITCHCAPVCC);
    oled.clear(); // clears the screen and buffer

    // init motor and servo
    pinMode(LEFT_EN, OUTPUT);
    pinMode(RIGHT_EN, OUTPUT);
    pinMode(LEFT_SPDCTR, OUTPUT);
    pinMode(RIGHT_SPDCTR, OUTPUT);
    digitalWrite(LEFT_EN, LOW);
    analogWrite(LEFT_SPDCTR, left_speed);
    digitalWrite(RIGHT_EN, LOW);
    analogWrite(RIGHT_SPDCTR, right_speed);

    myservo.attach(9);
    myservo.write(servo_degree);
}

void loop() {
    oled.clear(); // clears the screen and buffer
    oled.clear();
    read_int = analogRead(A0);
    voltage = read_int * 5.371 / 100.0;

    // show voltage
    String read_str(voltage);
    read_str += " V.";
    oled.drawstring(0, 0, (char*)read_str.c_str());

    // show speed and degree information
    oled.drawstring(0, 3, (char*)(String("Left speed:  ") + String(left_speed)).c_str());
    oled.drawstring(0, 4, (char*)(String("Right speed: ") + String(right_speed)).c_str());
    oled.drawstring(0, 5, (char*)(String("Servo degree: ") + String(servo_degree)).c_str());

    // if battery is low, give warning
    if(voltage < 7.4) {
        oled.drawstring(0, 1, (char*)"Low battery!");
        left_speed = 0;
        right_speed = 0;
        myservo.detach();
    }
    else {
        left_speed = 96;
        right_speed = 96;
    }

    // refresh motor and display
    analogWrite(LEFT_SPDCTR, left_speed);
    analogWrite(RIGHT_SPDCTR, right_speed);
    oled.display();     //display the buffer
    delay(1000);
}
