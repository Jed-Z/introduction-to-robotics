/* 电源电压 */
double getVoltage() {
    int read_int = analogRead(A0);              // 读电压原始值
    double voltage = read_int * 5.371 / 100.0;  // 换算为伏特
    return voltage;
}

/* 红外传感器：探测前方是否有障碍物（bool） */
bool getBarrier(int RED_PIN=12) {
    static bool is_inited = false;
    if(!is_inited) {
        pinMode(RED_PIN, INPUT);
        is_inited = true;
    }
    
    // 返回true代表有障碍物
    return digitalRead(RED_PIN) == LOW;
}

/* 超声传感器：获取距离（厘米） */
unsigned long getDistance(int TRIG_PIN=A1, int ECHO_PIN=A2) {
    static bool is_inited = false;
    if(!is_inited) {
        pinMode(TRIG_PIN, OUTPUT);
        pinMode(ECHO_PIN, INPUT);
        is_inited = true;
    }
    
    const int SOUND_SPEED = 340;  // 音速
    
    /* send a 10us HIGH pulse to trigger */
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    /* measure and convert */
    unsigned long interval = float(pulseIn(ECHO_PIN, HIGH));  // 微秒
    unsigned long distance = interval * SOUND_SPEED / 20000;  // 厘米

    return distance > 1000 ? 9999 : distance;  // 太大则认为无效
}
