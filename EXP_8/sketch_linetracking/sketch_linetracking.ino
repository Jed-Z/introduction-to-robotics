#include <Servo.h>
#include "define.h"

#define BLACK LOW
#define WHITE HIGH

/* 用全局变量保存小车的状态 */
Servo myservo;

int left_direction, right_direction;
int servo_degree;

void setSpeed(int right, int left) {
    // 左轮
    if(left<=0) {
        digitalWrite(LDIRECTION_PIN, LOW);
        analogWrite(LSPEED_PIN, -left);
    }
    else {
        digitalWrite(LDIRECTION_PIN, HIGH);
        analogWrite(LSPEED_PIN, -left);
    }

    // 右轮
    if(right<=0) {
        digitalWrite(RDIRECTION_PIN, LOW);
        analogWrite(RSPEED_PIN, -right);
    }
    else {
        digitalWrite(RDIRECTION_PIN, HIGH);
        analogWrite(RSPEED_PIN, -right);
    }
}


/* 同时改变两个车轮的方向：LOW-前进，HIGH-后退 */
void setDirection(int direction) {
    digitalWrite(LDIRECTION_PIN, direction);
    digitalWrite(RDIRECTION_PIN, direction);

    // 更新状态
    left_direction = direction;
    right_direction = direction;
}


/* 改变舵机角度 */
void setServo(int degree) {
    myservo.write(degree);
}


int privious = 0;  // 记录原来的运动方向：左、中、右分别表示为-1、0、1
int speed_norm = 200;
int speed_fast = 150, speed_slow = -150;
int servo_left = 120, servo_right = 60;

void setup() {
    // 初始化三个巡线传感器
    pinMode(10, INPUT);
    pinMode(12, INPUT);
    pinMode(13, INPUT);

    // 初始化电机
    pinMode(LDIRECTION_PIN, OUTPUT);
    pinMode(RDIRECTION_PIN, OUTPUT);
    pinMode(LSPEED_PIN, OUTPUT);
    pinMode(RSPEED_PIN, OUTPUT);
    
    // 初始化舵机
    myservo.attach(SERVO_PIN);
    setServo(94);
    myservo.write(94);

    // 设定初始运动模式
    setDirection(HIGH);  // 倒车
    setSpeed(speed_norm, speed_norm);     // 初始速度
}


void loop() {
    int L = digitalRead(13);
    int M = digitalRead(12);
    int R = digitalRead(10);
    
    if(L==WHITE && M==BLACK && R==WHITE) {  // 直走
        setServo(94);
        setSpeed(speed_norm, speed_norm);
    }
    else if(L==WHITE && M==WHITE && R==BLACK) {  // 右转
        privious = 1;
        setServo(servo_right);
        setSpeed(speed_fast, speed_slow);
    }
    else if(L==BLACK && M==WHITE && R==WHITE) {  // 左转
        privious = -1;
        setServo(servo_left);
        setSpeed(speed_slow, speed_fast);
    }
    else if(L==WHITE && M==WHITE && R==WHITE) {  // 偏离轨道后修正
        if(privious<=0) {
            setServo(servo_left);
            setSpeed(speed_slow, speed_fast);
        }
        else {
            setServo(servo_right);
            setSpeed(speed_fast, speed_slow);
        }
    }
    else if(L==BLACK && M==BLACK && R==BLACK) {  // 反向
        if(privious>0) {
            setServo(servo_left);  // 向左回轨道
        }
        else {
            setServo(servo_right);  // 向右回轨道
        }
    }
    else{
        setServo(94);
        setSpeed(speed_norm, speed_norm);
    }
}
