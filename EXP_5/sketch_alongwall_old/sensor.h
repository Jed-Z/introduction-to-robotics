/* 用红外传感器探测前方是否有障碍物（bool） */
bool getBarrier(int RED_PIN=12) {
    static bool is_inited = false;
    if(!is_inited) {
        pinMode(RED_PIN, INPUT);
        is_inited = true;
    }
    
    // 返回true代表有障碍物
    return digitalRead(RED_PIN) == LOW;
}

/* 从超声传感器获取距离（厘米） */
unsigned long getDistance(int TRIG_PIN=8, int ECHO_PIN=10) {
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
