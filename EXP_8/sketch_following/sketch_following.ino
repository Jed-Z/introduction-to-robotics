#include <Servo.h>
#include "define.h"
#include "sensor.h"

/* 用全局变量保存小车的状态 */
Servo myservo;

int left_direction, right_direction;
int left_speed, right_speed;  // 真实转速，不用考虑方向
int servo_degree;

/* 同时改变两个车轮的转速，参数为期望的真实转速 */
void setSpeed(int speed) {
    int left_velocity = left_direction==LOW ? speed : 255-speed;
    int right_velocity = right_direction==LOW ? speed : 255-speed;
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
    myservo.write(degree);
}


void setup() {
    pinMode(A0, INPUT);  // 初始化电压口

    // 初始化电机
    pinMode(LDIRECTION_PIN, OUTPUT);
    pinMode(RDIRECTION_PIN, OUTPUT);
    pinMode(LSPEED_PIN, OUTPUT);
    pinMode(RSPEED_PIN, OUTPUT);
    
    setDirection(LOW);
    setSpeed(70);
    
    // 初始化舵机
    myservo.attach(SERVO_PIN);
    setServo(94);

    Serial.begin(9600);
}


void loop() {
    int distance = getDistance();
    if(distance > 35) {
        setDirection(LOW);
        setSpeed(80);
    }
    else if(distance > 25) {
        setDirection(LOW);
        setSpeed(60);
    }
    else if(distance > 15) {
        setDirection(LOW);
        setSpeed(0);
    }
    else {
        setDirection(HIGH);
        setSpeed(70);
    }
    
    Serial.println(distance, DEC);
}
