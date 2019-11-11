#include "define.h"
#include <PinChangeInt.h>
#include <Servo.h>
#include <MsTimer2.h>

Servo myservo;
int left_direction, right_direction;
int left_speed, right_speed;  // 设置的真实转速，不用考虑方向
int servo_degree;
int wheel_degree = 0;


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
    servo_degree = degree;
}


void handlerR() {
  if (digitalRead(ENCODER_R) == LOW)  //如果是下降沿触发的中断
  {
    if (digitalRead(DIRECTION_R) == LOW)
      wheel_degree++;  //根据另外一相电平判定方向
    else
      wheel_degree--;
  }
  else  //如果是上升沿触发的中断
  {
    if (digitalRead(DIRECTION_R) == LOW)
      wheel_degree--;  //根据另外一相电平判定方向
    else
      wheel_degree++;
  }
}


void setup() {
    // 初始化编码器引脚，获取轮子转过的角度
    pinMode(ENCODER_L, INPUT);
    pinMode(DIRECTION_L, INPUT);
    pinMode(ENCODER_R, INPUT);
    pinMode(DIRECTION_R, INPUT);
    attachInterrupt(0, handlerR, CHANGE);                     //开启外部中断 编码器接口1
    attachPinChangeInterrupt(DIRECTION_R, handlerR, CHANGE);  //开启外部中断 编码器接口2

    Serial.begin(9600);
    Serial.setTimeout(50);

    // 初始化电机
    pinMode(LDIRECTION_PIN, OUTPUT);
    pinMode(RDIRECTION_PIN, OUTPUT);
    pinMode(LSPEED_PIN, OUTPUT);
    pinMode(RSPEED_PIN, OUTPUT);
    setDirection(LOW);
    setSpeed(70);

    // 定时器
    MsTimer2::set(10, getWheelDegree);
    MsTimer2::start();
}

/*
    pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
    e(k)代表本次偏差
    e(k-1)代表上一次的偏差  以此类推
    pwm代表增量输出
    在我们的速度控制闭环系统里面，只使用PI控制
    pwm+=Kp[e(k)-e(k-1)]+Ki*e(k)
*/

float Bias, Last_bias, Pwm;
int Incremental_PI_A(int Encoder, int Target) {
    float Velocity_KP = 1.0;
    float Velocity_KI = 1.0;
    Bias = Encoder - Target;  //计算偏差
    Pwm += Velocity_KP * (Bias - Last_bias) + Velocity_KI * Bias;  //增量式PI控制器
    if (Pwm > 255) Pwm = 255;    //限幅
    if (Pwm < -255) Pwm = -255;  //限幅
    Last_bias = Bias;  // 保存上一次偏差
    return Pwm;        // 增量输出
}


int real_speed;  // 通过编码器计算出的车轮实际速度
void getWheelDegree() {
    static int old_wheel_degree = 0;
    real_speed = wheel_degree - old_wheel_degree;
    old_wheel_degree = wheel_degree;
}


void loop () {
    delay(100);
    Serial.println(real_speed, DEC);  // 可用串口绘图器观察
    setSpeed(Incremental_PI_A(real_speed, 70));
}
