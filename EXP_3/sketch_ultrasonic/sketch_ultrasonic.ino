#define TRIG_PIN A1
#define ECHO_PIN A2
const int SOUND_SPEED = 340;  // sound speed in m/s

unsigned long interval;  // time interval in microseconds
unsigned long distance;  // distance in centimeters

void setup() {
    Serial.begin(9600);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop() {
    /* send a 10us HIGH pulse to trigger */
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    /* measure and convert */
    interval = float(pulseIn(ECHO_PIN, HIGH));  // microseconds
    distance = interval * SOUND_SPEED / 20000;  // centimeters

    /* feedback */
    Serial.print("Interval = ");
    Serial.print(interval);//串口输出等待时间的原始数据
    Serial.print("; Distance = ");
    Serial.print(distance);//串口输出距离换算成cm的结果
    Serial.println("cm");

    delay(500);
}
