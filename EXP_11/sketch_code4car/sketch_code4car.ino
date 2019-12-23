#include <PinChangeInt.h>
#include <SSD1306.h>
#include <Servo.h>
#include "define.h"

SSD1306 oled(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, 0);
Servo myservo;

String input_str = "";
String input_strBuffer = "";  // a String to hold incoming data

// 在OLED上显示内容，通过函数重载支持字符串和数字
void oledShow(uint8_t row, uint8_t col, String str) {
    oled.drawstring(col, row, (char *)str.c_str());
}
void oledShow(uint8_t row, uint8_t col, int num) {
    String str(num);
    oled.drawstring(col, row, (char *)str.c_str());
}
void oledShow(uint8_t row, uint8_t col, double num) {
    String str(num);
    oled.drawstring(col, row, (char *)str.c_str());
}


/* 改变舵机角度 */
int servo_degree = 94;
void setServo(uint8_t degree) {
    myservo.write(degree);
    servo_degree = degree;
}

int left_speed = 0, right_speed = 0;
/* 改变电机速度，电机前轮，舵机后轮 */
void setSpeed(int left, int right) {
    // 左轮
    if(left<=0) {
        digitalWrite(RDIRECTION_PIN, LOW);
        analogWrite(RSPEED_PIN, -left);
    }
    else {
        digitalWrite(RDIRECTION_PIN, HIGH);
        analogWrite(RSPEED_PIN, 255-left);
    }
    left_speed = left;

    // 右轮
    if(right<=0) {
        digitalWrite(LDIRECTION_PIN, LOW);
        analogWrite(LSPEED_PIN, -right);
    }
    else {
        digitalWrite(LDIRECTION_PIN, HIGH);
        analogWrite(LSPEED_PIN, 255-right);
    }
    right_speed = right;
}

void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        if (inChar == '\n' || inChar == '\r') {
            input_str = input_strBuffer;
            input_strBuffer = "";
            break;
        }
        input_strBuffer += inChar;
    }
}


volatile long Velocity_L, Velocity_R;
void READ_ENCODER_L() {
    if (digitalRead(ENCODER_L) == LOW) {
        if (digitalRead(DIRECTION_L) == LOW)
            Velocity_L--;
        else
            Velocity_L++;
    } else {
        if (digitalRead(DIRECTION_L) == LOW)
            Velocity_L++;
        else
            Velocity_L--;
    }
}

void READ_ENCODER_R() {
    if (digitalRead(ENCODER_R) == LOW) {
        if (digitalRead(DIRECTION_R) == LOW)
            Velocity_R++;
        else
            Velocity_R--;
    } else {
        if (digitalRead(DIRECTION_R) == LOW)
            Velocity_R--;
        else
            Velocity_R++;
    }
}

void setup() {
    // Init OLED screen
    oled.ssd1306_init(SSD1306_SWITCHCAPVCC);
    oled.clear();  // clears the screen and buffer

    // Init motor control
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);

    // Init servo
    myservo.attach(SERVO);

    // Init encoder
    pinMode(ENCODER_L, INPUT);
    pinMode(DIRECTION_L, INPUT);
    pinMode(ENCODER_R, INPUT);
    pinMode(DIRECTION_R, INPUT);

    pinMode(A0, INPUT);

    // Init interrupt
    attachInterrupt(0, READ_ENCODER_R, CHANGE);
    attachPinChangeInterrupt(4, READ_ENCODER_L, CHANGE);

    delay(1000);
    Serial.begin(115200);
    Serial.println("[+] Setup done.");
}

void loop() {
    oled.clear();

    // 显示串口收到的字符串
    oledShow(0, 0, input_str);
    // 显示电压
    oledShow(2, 0, String(analogRead(0)*5.371/100) + "V");
    // 显示电机速度和舵机角度
    oledShow(4, 0, servo_degree);
    oledShow(5, 0, right_speed);
    oledShow(5, 35, left_speed);

    int com_pos = input_str.indexOf(',');
    int next_servo_degree = input_str.substring(0, com_pos).toInt();
    String temp_str = input_str.substring(com_pos+1);
    
    com_pos = temp_str.indexOf(',');
    int next_left_speed = temp_str.substring(0, com_pos).toInt();
    temp_str = temp_str.substring(com_pos+1);

    int next_right_speed = temp_str.toInt();

    setServo(next_servo_degree);
    setSpeed(next_left_speed, next_right_speed);
    
    oled.display();
}
