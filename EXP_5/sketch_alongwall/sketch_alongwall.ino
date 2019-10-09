#include <Servo.h>
#include "define.h"
#include "sensor.h"

/* 常量 */
const int LEFT_EN = 11;      // 左边电机使能
const int LEFT_SPDCTR = 5;   // 左边电机速度控制
const int RIGHT_EN = 3;      // 右边电机使能
const int RIGHT_SPDCTR = 6;  // 右边电机速度控制

/* 全局变量 */
SSD1306 oled(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, 0);
Servo myservo;
int left_speed = 70;
int right_speed = 70;
int servo_degree = 94;
int read_int;
double voltage;
int state = 0;

void setup() {
    pinMode(A0, INPUT);  // 初始化电压口
    oled.ssd1306_init(SSD1306_SWITCHCAPVCC);  // 初始化显示屏
    oled.clear();

    // 初始化电机
    pinMode(LEFT_EN, OUTPUT);
    pinMode(RIGHT_EN, OUTPUT);
    pinMode(LEFT_SPDCTR, OUTPUT);
    pinMode(RIGHT_SPDCTR, OUTPUT);
    digitalWrite(LEFT_EN, LOW);
    analogWrite(LEFT_SPDCTR, left_speed);
    digitalWrite(RIGHT_EN, LOW);
    analogWrite(RIGHT_SPDCTR, right_speed);

    // 初始化舵机
    myservo.attach(9);
    myservo.write(servo_degree);
    
    // 初始化串口（可选）
    Serial.begin(9600);
}

void loop() {
    oled.clear();
    
    read_int = analogRead(A0);           // 读电压原始值
    voltage = read_int * 5.371 / 100.0;  // 换算为伏特

    // 显示电压
    String read_str(voltage);
    read_str += " V.";
    oled.drawstring(0, 0, (char*)read_str.c_str());

    // 显示左右电机速度以及舵机角度
    oled.drawstring(0, 2, (char*)( String("Movement:") + String(left_speed) + String(",") +
                                   String(right_speed) + String(",") + String(servo_degree)
                                ).c_str());

    // 若电压低则显示警告，并停止电机
    //    int cache_left_speed, cache_right_speed, cache_servo_degree;
    //    bool battery_refresh = false;
    //    if(voltage < 7.4 and battery_refresh) {
    //        battery_refresh = true;
    //        cache_left_speed = left_speed;
    //        cache_right_speed = right_speed;
    //        cache_servo_degree = servo_degree;
    //        oled.drawstring(48, 0, (char*)"Low battery!");
    //        left_speed = 0;
    //        right_speed = 0;
    //    }
    //    else {  // 电压恢复，重新开始运转
    //        battery_refresh = false;
    //        left_speed = cache_left_speed;
    //        right_speed = cache_right_speed;
    //        servo_degree = cache_servo_degree;
    //    }

    int distance = getDistance();
    if(distance < 10) {
        myservo.write(102);
    }
    else if(distance > 20) {
        myservo.write(86);
    }
    else {
        myservo.write(94);
    }
    
    // 显示传感器
    bool barrier = getBarrier();
    oled.drawstring(0, 4, (char*)String(barrier ? "True" : "False").c_str());
    oled.drawstring(0, 5, (char*)(String(distance) + String(" cm.")).c_str());
    
    if(barrier) {
        digitalWrite(LEFT_EN, HIGH);
        digitalWrite(RIGHT_EN, HIGH);
        analogWrite(LEFT_SPDCTR, 255-70);
        analogWrite(RIGHT_SPDCTR, 255-70);
        delay(1000);
        digitalWrite(LEFT_EN, LOW);
        digitalWrite(RIGHT_EN, LOW);
        analogWrite(LEFT_SPDCTR, 0);
        analogWrite(RIGHT_SPDCTR, 0);
        delay(1000);
        servo_degree = 180;
        myservo.write(servo_degree);
        delay(500);
        analogWrite(LEFT_SPDCTR, 70);
        analogWrite(RIGHT_SPDCTR, 70);
        delay(2500 * 1.25);
        analogWrite(LEFT_SPDCTR, 0);
        analogWrite(RIGHT_SPDCTR, 0);
        delay(500);
        myservo.write(94);
        delay(500);
        analogWrite(LEFT_SPDCTR, 70);
        analogWrite(RIGHT_SPDCTR, 70);
    }

    if(distance > 60) {
        analogWrite(LEFT_SPDCTR, 0);
        analogWrite(RIGHT_SPDCTR, 0);
        servo_degree = 25;
        myservo.write(servo_degree);
        delay(500);
        analogWrite(LEFT_SPDCTR, 70);
        analogWrite(RIGHT_SPDCTR, 70);
        delay(1000);
        analogWrite(LEFT_SPDCTR, 0);
        analogWrite(RIGHT_SPDCTR, 0);
        delay(500);
        myservo.write(94);
        delay(500);
        analogWrite(LEFT_SPDCTR, 70);
        analogWrite(RIGHT_SPDCTR, 70);
    }


    oled.display();
//    delay(200);
}
