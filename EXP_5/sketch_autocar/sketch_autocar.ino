#include "define.h"
#include <Servo.h>

/* 常量 */
const int LEFT_EN = 11;      // 左边电机使能
const int LEFT_SPDCTR = 5;   // 左边电机速度控制
const int RIGHT_EN = 3;      // 右边电机使能
const int RIGHT_SPDCTR = 6;  // 右边电机速度控制

/* 全局变量 */
SSD1306 oled(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, 0);
int read_int;
double voltage;

Servo myservo;
int left_speed = 96;
int right_speed = 96;
int servo_degree = 94;

/* 用红外传感器探测前方是否有障碍物（bool） */
bool getBarrier(int RED_PIN=4) {
    static bool is_inited = false;
    if(!is_inited) {
        pinMode(RED_PIN, INPUT);
        is_inited = true;
    }
    
    // 返回true代表有障碍物
    return digitalRead(RED_PIN) == LOW;
}

/* 从超声传感器获取距离（厘米） */
unsigned long getDistance(int TRIG_PIN=8, int ECHO_PIN=9) {
    static bool is_inited = false;
    if(!is_inited) {
        pinMode(TRIG_PIN, OUTPUT);
        pinMode(ECHO_PIN, INPUT);
        is_inited = true;
    }
    
    const int SOUND_SPEED = 340;
    
    /* send a 10us HIGH pulse to trigger */
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    /* measure and convert */
    unsigned long interval = float(pulseIn(ECHO_PIN, HIGH));  // microseconds
    unsigned long distance = interval * SOUND_SPEED / 20000;  // centimeters

    return distance > 1000 ? 9999 : distance;  // 太大则认为无效
}

void basicScreen() {
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
    if(voltage < 7.4) {
        oled.drawstring(48, 0, (char*)"Low battery!");
        left_speed = 0;
        right_speed = 0;
        servo_degree = 0;
    }
    else {  // 电压恢复，重新开始运转
        left_speed = 96;
        right_speed = 96;
        servo_degree = 100;
    }

    // 显示传感器
    oled.drawstring(0, 4, (char*)String(getBarrier() ? "True" : "False").c_str());
    oled.drawstring(0, 5, (char*)(String(getDistance()) + String(" cm.")).c_str());
    
    // 写入电机和舵机的值（无论是否变化都写）
    analogWrite(LEFT_SPDCTR, left_speed);
    analogWrite(RIGHT_SPDCTR, right_speed);
    myservo.write(servo_degree);

    oled.display();
    delay(250);      // 显示屏刷新率：4 Hz
}

void setup()
{
    pinMode(A0, INPUT);  // 初始化电压口
    oled.ssd1306_init(SSD1306_SWITCHCAPVCC);  // 初始化显示屏
    oled.clear();

    // 初始化电机
    pinMode(LEFT_EN, OUTPUT);
    pinMode(RIGHT_EN, OUTPUT);
    pinMode(LEFT_SPDCTR, OUTPUT);
    pinMode(RIGHT_SPDCTR, OUTPUT);
    digitalWrite(LEFT_EN, LOW);
    //    analogWrite(LEFT_SPDCTR, left_speed);
    digitalWrite(RIGHT_EN, LOW);
    //    analogWrite(RIGHT_SPDCTR, right_speed);

    // 初始化舵机
    myservo.attach(9);
    //    myservo.write(servo_degree);
    
    // 初始化串口（可选）
    Serial.begin(9600);
}

void loop() {
    basicScreen();
}
