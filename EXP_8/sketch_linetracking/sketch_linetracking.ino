#include <Servo.h>
#include "define.h"
#include "sensor.h"

#define BLACK LOW
#define WHITE HIGH

/* 用全局变量保存小车的状态 */
Servo myservo;

int left_direction, right_direction;
int left_speed, right_speed;  // 真实转速，不用考虑方向
int servo_degree;

/* 同时改变两个车轮的转速，参数为期望的真实转速 */
void setSpeed(int speed, int turn) {
    int left_velocity = left_direction==LOW ? speed : 255-speed;
    int right_velocity = right_direction==LOW ? speed : 255-speed;

    if(turn==-1){
        left_velocity -= 90;
    }
    else if(turn==1){
        right_velocity -= 90;
    }
    analogWrite(LSPEED_PIN, left_velocity);
    analogWrite(RSPEED_PIN, right_velocity);
    
    // 更新状态
    left_speed = speed;
    right_speed = speed;
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
//    myservo.write(degree);
}


void setup() {
    pinMode(A0, INPUT);  // 初始化电压口
    pinMode(10, INPUT);
    pinMode(12, INPUT);
    pinMode(13, INPUT);
    
    // 初始化电机
    pinMode(LDIRECTION_PIN, OUTPUT);
    pinMode(RDIRECTION_PIN, OUTPUT);
    pinMode(LSPEED_PIN, OUTPUT);
    pinMode(RSPEED_PIN, OUTPUT);
    
    setDirection(HIGH);
    setSpeed(70, 0);
    
    // 初始化舵机
    myservo.attach(SERVO_PIN);
    setServo(94);
    myservo.write(94);

    Serial.begin(9600);
}

int privious = 0;
void loop() {
    //delay(200);
    int L = digitalRead(13);
    int M = digitalRead(12);
    int R = digitalRead(10);
    
    if(L==WHITE && M==BLACK && R==WHITE) {  // 直走
        setServo(94);
        setSpeed(45, 0);
    }
    else if(L==WHITE && M==WHITE && R==BLACK) {  // 右转
        privious = 1;
//        setSpeed(0);
        //delay(500);
        setServo(30);
        //delay(500);
        setSpeed(45, 1);
    }
    else if(L==BLACK && M==WHITE && R==WHITE) {  // 左转
        privious = -1;
//        setSpeed(0);
        //delay(500);
        setServo(180);
        //delay(500);
        setSpeed(45, -1);
    }
    else if(L==WHITE && M==WHITE && R==WHITE) {  //
        if(privious<=0) {
            setServo(180);  // 向左回轨道
        }
        else {
            setServo(30);  // 向右回轨道
        }
    }
    else if(L==BLACK && M==BLACK && R==BLACK) {  // 反向
        if(privious>0) {
            setServo(180);  // 向左回轨道
        }
        else {
            setServo(30);  // 向右回轨道
        }
    }
    else{
        setSpeed(45, 0);
    }
}
