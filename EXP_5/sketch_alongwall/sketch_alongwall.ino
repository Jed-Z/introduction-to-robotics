#include <Servo.h>
#include "define.h"
#include "sensor.h"

/* 用全局变量保存小车的状态 */
// SSD1306 oled(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, 0);
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
//    oled.ssd1306_init(SSD1306_SWITCHCAPVCC);  // 初始化显示屏
    // oled.clear();

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
}


void loop() {
    // oled.clear();

    // 显示电压
    // oled.drawstring(0, 0, (char*)(String(getVoltage()) + " V.").c_str());

    // 显示左右电机速度以及舵机角度
    // oled.drawstring(0, 2, (char*)( String("Movement:") + String(left_speed) + String(",") +
                                   // String(right_speed) + String(",") + String(servo_degree)
                                // ).c_str());

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
        setServo(102);
    }
    else if(distance > 20) {
        setServo(86);
    }
    else {
        setServo(94);
    }
    
    // 显示传感器
    bool barrier = getBarrier();
    // oled.drawstring(0, 4, (char*)String(barrier ? "True" : "False").c_str());
    // oled.drawstring(0, 5, (char*)(String(distance) + String(" cm.")).c_str());
    
    if(barrier) {
        setDirection(HIGH);
        setSpeed(70);
        delay(1000);
        setDirection(LOW);
        setSpeed(0);
        delay(1000);
        setServo(180);
        delay(500);
        setSpeed(70);
        delay(2500 * 1.25);
        setSpeed(0);
        delay(500);
        setServo(94);
        delay(500);
        setSpeed(70);
    }

    if(distance > 60) {
        setSpeed(0);
        setServo(25);
        delay(500);
        setSpeed(70);
        delay(1000);
        setSpeed(0);
        delay(500);
        setServo(94);
        delay(500);
        setSpeed(70);
    }


    // oled.display();
//    delay(200);
}
